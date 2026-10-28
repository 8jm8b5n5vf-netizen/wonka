#include "Plugin.h"
using namespace juce;
static std::array<double, 22> renderKeyFor(const volta::Settings &s,
                                           double sampleRate) {
  return {double(s.mode),
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
          sampleRate};
}

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
VoltaProcessor::VoltaProcessor(File cacheDirectory)
    : AudioProcessor(
          BusesProperties()
              .withInput("Input", AudioChannelSet::stereo(), true)
              .withOutput("Output", AudioChannelSet::stereo(), true)),
      state(*this, nullptr, "WONKA", layout()) {
  realtimeVolume = state.getRawParameterValue("volume");
  realtimeProtect = state.getRawParameterValue("protect");
  realtimeBypass = state.getRawParameterValue("bypass");
  auto applicationData =
      File::getSpecialLocation(File::userApplicationDataDirectory);
#if JUCE_MAC
  applicationData = applicationData.getChildFile("Application Support");
#endif
  cacheRoot = cacheDirectory == File{}
                  ? applicationData.getChildFile("WonkaAudio/WONKA/Rendered")
                  : cacheDirectory;
  startTimerHz(20);
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
  playGain.reset(sr, .01);
  playGain.setCurrentAndTargetValue(0);
  volume.reset(sr, .02);
  volume.setCurrentAndTargetValue(param("volume"));
  playing = false;
}
void VoltaProcessor::processBlock(AudioBuffer<float> &buffer,
                                  MidiBuffer &midi) {
  ScopedNoDenormals nd;
  midi.clear();
  ++audioCallbacks;
  float in = 0;
  for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    in = std::max(in, buffer.getMagnitude(ch, 0, buffer.getNumSamples()));
  inputPeak = in;
  if (in >= 1)
    inputClipped = true;
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
    if (captured == capture.getNumSamples()) {
      recordingAtLimit = true;
      recordState = 4;
    }
  }
  if (rs == 3)
    recordState = 4;
  if (realtimeBypass->load() > .5f) {
    playing = false;
    outputPeak = in;
    if (in >= 1)
      outputClipped = true;
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
    playGain.setCurrentAndTargetValue(0);
  }
  playGain.setTargetValue(playing.load() ? 1.f : 0.f);
  if ((playing.load() || playGain.isSmoothing()) && clip &&
      !clip->audio.empty()) {
    buffer.clear();
    volume.setTargetValue(realtimeVolume->load());
    int n = (int)clip->audio[0].size();
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
      if (cursor >= n) {
        playing = false;
        break;
      }
      size_t j = (size_t)cursor, k = std::min(j + 1, (size_t)n - 1);
      float fraction = (float)(cursor - j),
            gain = volume.getNextValue() * playGain.getNextValue();
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
            c, i,
            realtimeProtect->load() > .5f ? std::clamp(v, -.999f, .999f) : v);
      }
      cursor += clip->rate / hostRate.load();
    }
    position = std::min(1., cursor / n);
  }
  hazard.store(nullptr);
  float out = 0;
  for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    out = std::max(out, buffer.getMagnitude(ch, 0, buffer.getNumSamples()));
  outputPeak = out;
  if (out >= .999f)
    outputClipped = true;
}
void VoltaProcessor::queue(std::function<void()> fn) {
  ++activeJobs;
  pool.addJob([this, fn = std::move(fn)] {
    try {
      fn();
    } catch (const std::exception &e) {
      phase = Phase::Error;
      dirtyRender = false;
      dragPreparing = false;
      if (recordState.load() == 5)
        recordState = 0;
      setStatus("Erro: " + String::fromUTF8(e.what()));
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
  sourceBlocked = false;
  ++revision;
  setStatus("Fonte carregada - renderizando");
  render();
}
void VoltaProcessor::rejectImport(String reason) {
  sourceBlocked = true;
  ++renderGeneration;
  stop();
  {
    std::lock_guard<std::mutex> lock(mutex);
    output.reset();
  }
  dirtyRender = false;
  phase = Phase::Error;
  setStatus(std::move(reason));
  ++revision;
}
void VoltaProcessor::importFile(File f) {
  if (recordState.load() != 0) {
    setStatus("Pare a gravacao antes de importar");
    return;
  }
  recordingAtLimit = false;
  sourceBlocked = true;
  dirtyRender = false;
  phase = Phase::Loading;
  setStatus("Carregando: " + f.getFileName());
  setParam("start", 0);
  setParam("end", 1);
  ++renderGeneration;
  {
    std::lock_guard<std::mutex> lock(mutex);
    output.reset();
  }
  ++revision;
  stop();
  queue([this, f] { load(readAudioFile(f)); });
}
void VoltaProcessor::render() {
  dirtyRender = false;
  if (sourceBlocked.load())
    return;
  auto src = source();
  if (!src) {
    return;
  }
  auto s = settings();
  const double renderRate = hostRate.load();
  auto key = renderKeyFor(s, renderRate);
  phase = Phase::Rendering;
  int gen = ++renderGeneration;
  setStatus("Processando...");
  queue([this, src, s, gen, key, renderRate] {
    if (gen != renderGeneration.load())
      return;
    auto c = std::make_shared<Clip>();
    c->rate = renderRate;
    c->name = "WONKA_" + String(s.mode      ? "Throw"
                                : s.variant ? "Swell"
                                            : "Reverse");
    auto a = volta::resample(src->audio, src->rate, c->rate);
    c->audio = volta::render(a, c->rate, s);
    c->buildPeaks();
    if (gen != renderGeneration.load() || key != signature()) {
      dirtyRender = true;
      return;
    }
    playing = false;
    {
      std::lock_guard<std::mutex> l(mutex);
      output = c;
      outputKey = key;
    }
    dirtyRender = false;
    phase = Phase::Ready;
    ++revision;
    setStatus(String(recordingAtLimit.load()
                         ? "Pronto - limite de gravacao 60 s atingido - "
                         : "Pronto - ") +
              String(c->audio[0].size() / c->rate, 2) + " s");
  });
}
void VoltaProcessor::preview(bool src) {
  if (phase.load() == Phase::Loading) {
    setStatus("Aguarde a importacao");
    return;
  }
  if (!src && !resultIsCurrent()) {
    setStatus("Atualizando resultado - aguarde");
    return;
  }
  if (recordState.load() != 0) {
    setStatus("Pare a gravacao antes de ouvir");
    return;
  }
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
  if (phase.load() == Phase::Loading) {
    setStatus("Aguarde a importacao antes de gravar");
    return;
  }
  phase = Phase::Recording;
  recordingAtLimit = false;
  capturedCount = 0;
  recordState = 1;
  setStatus("Gravando entrada da DAW - max. 60 s");
}
void VoltaProcessor::stopRecord() {
  int r = recordState.load();
  if (r == 1) {
    int expect = 1;
    if (recordState.compare_exchange_strong(expect, 0)) {
      phase = source() ? Phase::Ready : Phase::Empty;
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
  setParam("start", 0);
  setParam("end", 1);
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
  if (!c || !resultIsCurrent() || recordState.load() != 0) {
    setStatus("Aguarde o render atual antes de exportar");
    return;
  }
  bool protect = param("protect") > .5f;
  queue([this, file, bits, rate, c, protect] {
    if (!resultIsCurrent() || result() != c) {
      setStatus("Render alterado - exportacao cancelada");
      return;
    }
    writeAudioFile(*c, file, bits, rate, protect);
    if (resultIsCurrent())
      phase = Phase::Ready;
    setStatus("WAV salvo: " + file.getFullPathName());
  });
}
void VoltaProcessor::preset(int index) {
  const auto &values = volta::presets();
  const int i = std::clamp(index, 0, (int)values.size() - 1);
  const auto &v = values[i];
  setParam("mode", (float)v.mode);
  setParam("variant", (float)v.variant);
  setParam("division", (float)v.division);
  setParam("length", v.length);
  setParam("space", v.space);
  setParam("tone", v.tone);
  setParam("shape", v.shape);
  setParam("tremolo", v.tremolo);
  setParam("feedback", v.feedback);
  setParam("bars", v.bars);
  setParam("musical", v.musical ? 1.f : 0.f);
  setParam("tail", 1);
  setParam("original", 0);
  setParam("normalise", 0);
  setParam("protect", 1);
  setParam("tremrate", 1);
  state.state.setProperty("preset", i + 1, nullptr);
  render();
}
bool VoltaProcessor::presetModified() const {
  int index = (int)state.state.getProperty("preset", 1) - 1;
  if (index < 0 || index >= 17)
    return true;
  const auto &v = volta::presets()[index];
  const std::pair<const char *, float> fields[] = {
      {"mode", (float)v.mode},
      {"variant", (float)v.variant},
      {"division", (float)v.division},
      {"length", v.length},
      {"space", v.space},
      {"tone", v.tone},
      {"shape", v.shape},
      {"tremolo", v.tremolo},
      {"feedback", v.feedback},
      {"bars", v.bars},
      {"musical", v.musical ? 1.f : 0.f},
      {"tail", 1},
      {"original", 0},
      {"normalise", 0},
      {"protect", 1},
      {"tremrate", 1}};
  for (const auto &f : fields)
    if (std::abs(param(f.first) - f.second) > .0001f)
      return true;
  return false;
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
  sourceBlocked = true;
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
    phase = Phase::Empty;
    sourceBlocked = false;
    dirtyRender = false;
    setStatus("Abra ou arraste WAV / AIFF");
  }
}
AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new VoltaProcessor();
}

VoltaProcessor::Signature VoltaProcessor::signature() const {
  return renderKeyFor(settings(), hostRate.load());
}

bool VoltaProcessor::resultIsCurrent() const {
  const auto key = signature();
  std::lock_guard<std::mutex> lock(mutex);
  return !sourceBlocked.load() && output && input && key == outputKey &&
         phase.load() != Phase::Loading;
}
File VoltaProcessor::preparedDragFile(int bits, double rate) const {
  if (!resultIsCurrent())
    return {};
  std::lock_guard<std::mutex> lock(mutex);
  return output == dragClip && dragKey == outputKey && bits == dragBits &&
                 rate == dragRate
             ? dragFile
             : File{};
}
void VoltaProcessor::prepareDragFile(int bits, double rate) {
  if (!resultIsCurrent() || recordState.load() != 0 || dragPreparing.load())
    return;
  if (preparedDragFile(bits, rate).existsAsFile())
    return;
  auto c = result();
  auto key = signature();
  {
    std::lock_guard<std::mutex> lock(mutex);
    if (dragAttempt == c && attemptedBits == bits && attemptedRate == rate)
      return;
    dragAttempt = c;
    attemptedBits = bits;
    attemptedRate = rate;
  }
  dragPreparing = true;
  const bool protect = param("protect") > .5f;
  queue([this, c, key, bits, rate, protect] {
    if (!resultIsCurrent() || result() != c) {
      dragPreparing = false;
      return;
    }
    auto made = cacheRoot.createDirectory();
    if (made.failed())
      throw std::runtime_error(
          ("Cache WAV: " + made.getErrorMessage()).toStdString());
    auto file =
        cacheRoot.getChildFile(c->name + "_" + Uuid().toString() + ".wav");
    writeAudioFile(*c, file, bits, rate, protect);
    {
      std::lock_guard<std::mutex> lock(mutex);
      dragClip = c;
      dragFile = file;
      dragKey = key;
      dragBits = bits;
      dragRate = rate;
    }
    dragPreparing = false;
  });
}
void VoltaProcessor::timerCallback() {
  serviceRecording();
  if (sourceBlocked.load() || recordState.load() != 0)
    return;
  const auto key = signature();
  if (key != lastRenderKey) {
    lastRenderKey = key;
    pendingTicks = 3;
    if (source()) {
      dirtyRender = true;
      phase = Phase::Rendering;
    }
    stop();
  }
  if ((pendingTicks > 0 && --pendingTicks == 0 ||
       pendingTicks == 0 && dirtyRender.load()) &&
      recordState.load() == 0)
    render();
}
