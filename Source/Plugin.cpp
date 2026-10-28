#include "Plugin.h"
using namespace juce;
AudioProcessorValueTreeState::ParameterLayout VoltaProcessor::layout() {
  AudioProcessorValueTreeState::ParameterLayout l;
  auto f = [&](const char *id, float lo, float hi, float def) {
    l.add(std::make_unique<AudioParameterFloat>(
        ParameterID{id, 1}, id, NormalisableRange<float>(lo, hi), def));
  };
  auto b = [&](const char *id, bool d) {
    l.add(std::make_unique<AudioParameterBool>(ParameterID{id, 1}, id, d));
  };
  auto choice = [&](const char *id, StringArray a, int d) {
    l.add(std::make_unique<AudioParameterChoice>(ParameterID{id, 1}, id, a, d));
  };
  choice("tremrate", {"1/4", "1/8", "1/16", "4 Hz livre"}, 1);
  choice("mode", {"Reverse", "Throw"}, 0);
  choice("variant", {"Line", "Swell"}, 0);
  choice("division",
         {"1/4", "1/8", "1/8 dotted", "1/8 triplet", "1/16", "1/16 dotted",
          "1/16 triplet", "1/2"},
         2);
  f("space", 0, 1, .5);
  f("length", .05f, 30, 2);
  f("tone", 0, 1, .5);
  f("shape", 0, 1, .5);
  f("tremolo", 0, 1, 0);
  f("bars", .25f, 8, 1);
  f("feedback", 0, .94f, .5);
  f("bpm", 30, 300, 120);
  f("numerator", 1, 12, 4);
  choice("denominator", {"2", "4", "8", "16"}, 1);
  f("start", 0, .9999f, 0);
  f("end", .0001f, 1, 1);
  f("volume", 0, 1, .7f);
  b("sync", true);
  b("musical", false);
  b("tail", true);
  b("original", false);
  b("normalise", false);
  b("protect", true);
  b("bypass", false);
  return l;
}
VoltaProcessor::VoltaProcessor()
    : AudioProcessor(
          BusesProperties()
              .withInput("Input", AudioChannelSet::stereo(), true)
              .withOutput("Output", AudioChannelSet::stereo(), true)),
      state(*this, nullptr, "WONKA", layout()) {
  startTimerHz(10);
}
VoltaProcessor::~VoltaProcessor() {
  stopTimer();
  pool.removeAllJobs(true, -1);
}
float VoltaProcessor::param(const char *id) const {
  return state.getRawParameterValue(id)->load();
}
void VoltaProcessor::setParam(const char *id, float v) {
  if (auto *p = state.getParameter(id))
    p->setValueNotifyingHost(p->convertTo0to1(v));
}
volta::Settings VoltaProcessor::settings() const {
  volta::Settings s;
  s.mode = (int)param("mode");
  s.variant = (int)param("variant");
  s.division = (int)param("division");
  s.tremRate = (int)param("tremrate");
  s.space = param("space");
  s.length = param("length");
  s.tone = param("tone");
  s.shape = param("shape");
  s.tremolo = param("tremolo");
  s.bars = param("bars");
  s.feedback = param("feedback");
  s.start = param("start");
  s.end = param("end");
  bool host = param("sync") > .5f && hostTempo.load();
  bool meter = param("sync") > .5f && hostSignature.load();
  s.bpm = host ? bpm.load() : param("bpm");
  s.numerator = meter ? hostNum.load() : (int)std::round(param("numerator"));
  s.denominator = meter ? hostDen.load() : (2 << (int)param("denominator"));
  s.musical = param("musical") > .5f;
  s.tail = param("tail") > .5f;
  s.original = param("original") > .5f;
  s.normalise = param("normalise") > .5f;
  s.protect = param("protect") > .5f;
  return s;
}
bool VoltaProcessor::isBusesLayoutSupported(const BusesLayout &l) const {
  return (l.getMainOutputChannelSet() == AudioChannelSet::mono() ||
          l.getMainOutputChannelSet() == AudioChannelSet::stereo()) &&
         l.getMainInputChannelSet() == l.getMainOutputChannelSet();
}
void VoltaProcessor::prepareToPlay(double sr, int) {
  std::lock_guard<std::mutex> lock(captureMutex);
  hostRate = sr;
  capture.setSize(2, (int)(60 * sr));
  recordState = 0;
  capturedCount = 0;
  volume.reset(sr, .02);
  volume.setCurrentAndTargetValue(param("volume"));
  playing = false;
}
void VoltaProcessor::processBlock(AudioBuffer<float> &buffer,
                                  MidiBuffer &midi) {
  ScopedNoDenormals nd;
  midi.clear();
  bool got = false, gotMeter = false;
  if (auto *h = getPlayHead())
    if (auto p = h->getPosition()) {
      if (auto b = p->getBpm()) {
        if (*b >= 30 && *b <= 300) {
          bpm = std::round(*b * 1000) / 1000;
          got = true;
        }
      }
      if (auto t = p->getTimeSignature()) {
        gotMeter = true;
        hostNum = std::clamp(t->numerator, 1, 32);
        hostDen = std::max(1, t->denominator);
      }
    }
  hostTempo = got;
  hostSignature = gotMeter;
  int rs = recordState.load();
  if (rs == 1) {
    int expected = 1;
    if (recordState.compare_exchange_strong(expected, 2)) {
      captured = 0;
      rs = 2;
    } else
      rs = recordState.load();
  }
  if (rs == 2) {
    int count =
        std::min(buffer.getNumSamples(), capture.getNumSamples() - captured);
    for (int c = 0; c < 2; ++c)
      capture.copyFrom(c, captured, buffer,
                       std::min(c, buffer.getNumChannels() - 1), 0, count);
    captured += count;
    capturedCount = captured;
    if (captured == capture.getNumSamples())
      recordState = 4;
  }
  if (rs == 3)
    recordState = 4;
  if (param("bypass") > .5f) {
    playing = false;
    return;
  }
  const Clip *clip;
  do {
    clip = playback.load();
    hazard.store(clip);
  } while (clip != playback.load());
  unsigned command = playCommand.load();
  if (command != seenCommand) {
    seenCommand = command;
    cursor = 0;
  }
  if (playing.load() && clip && !clip->audio.empty()) {
    buffer.clear();
    volume.setTargetValue(param("volume"));
    int n = (int)clip->audio[0].size();
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
      if (cursor >= n) {
        playing = false;
        break;
      }
      size_t j = (size_t)cursor, k = std::min(j + 1, (size_t)n - 1);
      float fraction = (float)(cursor - j), gain = volume.getNextValue();
      for (int c = 0; c < buffer.getNumChannels(); ++c) {
        float v = 0;
        if (buffer.getNumChannels() == 1 && clip->audio.size() == 2) {
          for (auto &a : clip->audio)
            v += (a[j] + (a[k] - a[j]) * fraction) * .5f;
        } else {
          auto &a = clip->audio[std::min((size_t)c, clip->audio.size() - 1)];
          v = a[j] + (a[k] - a[j]) * fraction;
        }
        v *= gain;
        buffer.setSample(
            c, i, param("protect") > .5f ? std::clamp(v, -.999f, .999f) : v);
      }
      cursor += clip->rate / hostRate.load();
    }
    position = std::min(1., cursor / n);
  }
  hazard.store(nullptr);
}
void VoltaProcessor::queue(std::function<void()> fn) {
  ++activeJobs;
  pool.addJob([this, fn = std::move(fn)] {
    try {
      fn();
    } catch (const std::exception &e) {
      setStatus("Erro: " + String(e.what()));
    }
    --activeJobs;
  });
}
void VoltaProcessor::setStatus(String m) {
  std::lock_guard<std::mutex> l(mutex);
  message = std::move(m);
}
String VoltaProcessor::status() const {
  std::lock_guard<std::mutex> l(mutex);
  return message;
}
std::shared_ptr<const Clip> VoltaProcessor::source() const {
  std::lock_guard<std::mutex> l(mutex);
  return input;
}
std::shared_ptr<const Clip> VoltaProcessor::result() const {
  std::lock_guard<std::mutex> l(mutex);
  return output;
}
void VoltaProcessor::publish(std::shared_ptr<const Clip> p) {
  std::lock_guard<std::mutex> l(mutex);
  if (current)
    retired.push_back(current);
  current = std::move(p);
  playback.store(current.get());
  retired.erase(
      std::remove_if(retired.begin(), retired.end(),
                     [this](auto &q) { return q.get() != hazard.load(); }),
      retired.end());
}
void VoltaProcessor::load(std::shared_ptr<const Clip> c) {
  playing = false;
  {
    std::lock_guard<std::mutex> l(mutex);
    input = c;
    output.reset();
  }
  ++revision;
  setStatus("Fonte carregada - renderizando");
  render();
}
void VoltaProcessor::importFile(File f) {
  ++renderGeneration;
  stop();
  queue([this, f] { load(readAudioFile(f)); });
}
void VoltaProcessor::render() {
  dirtyRender = false;
  auto src = source();
  if (!src) {
    return;
  }
  auto s = settings();
  int gen = ++renderGeneration;
  setStatus("Processando...");
  queue([this, src, s, gen] {
    if (gen != renderGeneration.load())
      return;
    auto c = std::make_shared<Clip>();
    c->rate = hostRate.load();
    c->name = "WONKA_" + String(s.mode      ? "Throw"
                                : s.variant ? "Swell"
                                            : "Reverse");
    auto a = volta::resample(src->audio, src->rate, c->rate);
    c->audio = volta::render(a, c->rate, s);
    c->buildPeaks();
    if (gen != renderGeneration.load())
      return;
    playing = false;
    {
      std::lock_guard<std::mutex> l(mutex);
      output = c;
    }
    publish(c);
    ++revision;
    setStatus("Pronto - " + String(c->audio[0].size() / c->rate, 2) + " s");
  });
}
void VoltaProcessor::preview(bool src) {
  auto c = src ? source() : result();
  if (!c)
    return;
  playing = false;
  publish(c);
  position = 0;
  ++playCommand;
  playing = true;
}
void VoltaProcessor::stop() { playing = false; }
void VoltaProcessor::record() {
  if (recordState.load() != 0)
    return;
  stop();
  recordState = 1;
  setStatus("Gravando entrada da DAW - max. 60 s");
}
void VoltaProcessor::stopRecord() {
  int r = recordState.load();
  if (r == 1) {
    int expect = 1;
    if (recordState.compare_exchange_strong(expect, 0)) {
      setStatus("Gravacao cancelada");
      return;
    }
  }
  if (recordState.load() == 2)
    recordState = 3;
}
void VoltaProcessor::serviceRecording() {
  int expected = 4;
  if (!recordState.compare_exchange_strong(expected, 5))
    return;
  queue([this] {
    std::lock_guard<std::mutex> lock(captureMutex);
    int n = capturedCount.load();
    if (n <= 0) {
      recordState = 0;
      setStatus("Sem audio gravado");
      return;
    }
    auto c = std::make_shared<Clip>();
    c->rate = hostRate.load();
    c->name = "Gravacao";
    for (int ch = 0; ch < getTotalNumInputChannels(); ++ch)
      c->audio.emplace_back(capture.getReadPointer(ch),
                            capture.getReadPointer(ch) + n);
    for (auto &ch : c->audio)
      for (auto &v : ch)
        if (!std::isfinite(v))
          v = 0;
    c->buildPeaks();
    recordState = 0;
    load(c);
  });
}
void VoltaProcessor::exportFile(File file, int bits, double rate) {
  auto c = result();
  if (!c) {
    setStatus("Renderize primeiro");
    return;
  }
  bool protect = param("protect") > .5f;
  queue([this, file, bits, rate, c, protect] {
    writeAudioFile(*c, file, bits, rate, protect);
    setStatus("WAV salvo: " + file.getFullPathName());
  });
}
void VoltaProcessor::preset(int i) {
  setParam("mode", i >= 3 ? 1 : 0);
  setParam("variant", i == 2 ? 1 : 0);
  setParam("length", i == 0 ? 1 : i == 2 ? 8 : 4);
  setParam("space", i == 0 ? .25f : .8f);
  setParam("tone", i == 4 ? .15f : .6f);
  setParam("feedback", i == 4 ? .7f : .5f);
  setParam("division", i == 3 ? 2 : 1);
  setParam("musical", 0);
  setParam("shape", .5);
  setParam("tremolo", 0);
  render();
}
void VoltaProcessor::getStateInformation(MemoryBlock &block) {
  auto tree = state.copyState();
  auto src = source();
  if (src) {
    tree.setProperty("audio", encodeAudio(*src).toBase64Encoding(), nullptr);
    tree.setProperty("name", src->name, nullptr);
  }
  auto xml = tree.createXml();
  copyXmlToBinary(*xml, block);
}
void VoltaProcessor::setStateInformation(const void *data, int size) {
  ++renderGeneration;
  auto xml = getXmlFromBinary(data, size);
  if (!xml || !xml->hasTagName("WONKA")) {
    setStatus("Sessao invalida");
    return;
  }
  auto tree = ValueTree::fromXml(*xml);
  MemoryBlock bytes;
  if (tree.hasProperty("audio") &&
      !bytes.fromBase64Encoding(tree["audio"].toString())) {
    setStatus("Audio da sessao corrompido");
    return;
  }
  tree.removeProperty("audio", nullptr);
  auto name = tree["name"].toString();
  state.replaceState(tree);
  if (bytes.getSize())
    queue([this, bytes = std::move(bytes), name] {
      load(decodeAudio(bytes, name));
    });
  else {
    stop();
    {
      std::lock_guard<std::mutex> l(mutex);
      input.reset();
      output.reset();
    }
    publish(nullptr);
    ++revision;
    setStatus("Abra ou arraste WAV / AIFF");
  }
}
AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new VoltaProcessor();
}

void VoltaProcessor::timerCallback() {
  serviceRecording();
  auto s = settings();
  auto src = source();
  const bool newSource = src != lastAutoSource;
  if (src && s.mode == 1 &&
      ((newSource && src->autoSelect) ||
       (!newSource && sawMode && lastAutoMode == 0))) {
    setParam("start", (float)src->suggested.first);
    setParam("end", (float)src->suggested.second);
    s = settings();
  }
  lastAutoSource = src;
  lastAutoMode = s.mode;
  sawMode = true;
  std::vector<double> key = {double(s.mode),
                             double(s.variant),
                             double(s.division),
                             double(s.tremRate),
                             double(s.numerator),
                             double(s.denominator),
                             s.bpm,
                             s.length,
                             s.bars,
                             s.space,
                             s.tone,
                             s.shape,
                             s.tremolo,
                             s.feedback,
                             s.start,
                             s.end,
                             double(s.musical),
                             double(s.tail),
                             double(s.original),
                             double(s.normalise),
                             double(s.protect),
                             hostRate.load()};
  if (key != lastRenderKey) {
    lastRenderKey = key;
    pendingTicks = 3;
    if (source())
      dirtyRender = true;
    stop();
  }
  if ((pendingTicks > 0 && --pendingTicks == 0 ||
       pendingTicks == 0 && dirtyRender.load()) &&
      recordState.load() == 0)
    render();
}
