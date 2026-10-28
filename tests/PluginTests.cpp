#include "Plugin.h"
#include <chrono>
#include <iostream>
#include <thread>
using namespace juce;
void check(bool v, const char *name) {
  if (!v)
    throw std::runtime_error(name);
}
void waitFor(const std::function<bool()> &ready) {
  for (int i = 0; i < 1000; ++i) {
    if (ready())
      return;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  throw std::runtime_error("Timed out waiting for worker");
}
struct TestPlayHead : AudioPlayHead {
  bool tempo = true;
  Optional<PositionInfo> getPosition() const override {
    PositionInfo info;
    if (tempo)
      info.setBpm(100.);
    info.setTimeSignature(TimeSignature{3, 8});
    return info;
  }
};
int main() {
  ScopedJuceInitialiser_GUI init;
  try {
    const auto folder = File::getSpecialLocation(File::tempDirectory)
                            .getChildFile("volta-test-" + Uuid().toString());
    check(folder.createDirectory().wasOk(), "test directory");
    struct Cleanup {
      File f;
      ~Cleanup() { f.deleteRecursively(); }
    } cleanup{folder};
    auto source = folder.getChildFile("voice.wav");
    AudioBuffer<float> tone(1, 4800);
    for (int i = 0; i < 4800; ++i)
      tone.setSample(0, i, .4f * std::sin(float(i) * .08f));
    {
      auto stream = source.createOutputStream();
      WavAudioFormat format;
      std::unique_ptr<AudioFormatWriter> w(
          format.createWriterFor(stream.get(), 48000, 1, 24, {}, 0));
      check(w != nullptr, "fixture writer");
      stream.release();
      check(w->writeFromAudioSampleBuffer(tone, 0, 4800), "fixture write");
    }
    VoltaProcessor p;
    p.prepareToPlay(48000, 256);
    p.setParam("length", .2f);
    p.setParam("tail", 0);
    p.importFile(source);
    waitFor([&] { return p.result() != nullptr && !p.isBusy(); });
    check(p.source()->audio.size() == 1, "mono import");
    check(p.result()->audio[0].size() == 9600, "background render length");
    MemoryBlock state;
    p.setParam("tone", .27f);
    p.setParam("start", .2f);
    p.getStateInformation(state);
    source.deleteFile();
    VoltaProcessor restored;
    restored.prepareToPlay(48000, 256);
    restored.setStateInformation(state.getData(), (int)state.getSize());
    waitFor([&] { return restored.result() != nullptr && !restored.isBusy(); });
    check(std::abs(restored.param("tone") - .27f) < 1e-5,
          "parameter restoration");
    check(std::abs(restored.param("start") - .2f) < 1e-5,
          "selection restoration");
    check(restored.source()->audio == p.source()->audio,
          "embedded audio survives deleted source");
    AudioBuffer<float> block(2, 256);
    MidiBuffer midi;
    restored.preview(false);
    float energy = 0;
    for (int k = 0; k < 50; ++k) {
      block.clear();
      restored.processBlock(block, midi);
      energy += block.getMagnitude(0, 256);
    }
    check(energy > 0, "preview produces audio");
    check(!restored.playing, "preview finishes");
    for (int bits : {24, 32}) {
      auto f = folder.getChildFile("render" + String(bits) + ".wav");
      restored.exportFile(f, bits, 44100);
      waitFor([&] { return f.existsAsFile() && !restored.isBusy(); });
      AudioFormatManager fm;
      fm.registerBasicFormats();
      std::unique_ptr<AudioFormatReader> r(fm.createReaderFor(f));
      check(r && r->sampleRate == 44100 && r->numChannels == 1,
            "export rate and channels");
      check(r->bitsPerSample == (unsigned)bits, "export depth");
      check(r->usesFloatingPointData == (bits == 32), "float encoding");
      check(r->lengthInSamples == 8820, "export duration");
    }
    restored.record();
    for (int k = 0; k < 10; ++k) {
      for (int c = 0; c < 2; ++c)
        for (int i = 0; i < 256; ++i)
          block.setSample(c, i, .1f);
      restored.processBlock(block, midi);
    }
    restored.stopRecord();
    restored.processBlock(block, midi);
    restored.serviceRecording();
    waitFor([&] {
      return restored.source()->name == "Gravacao" && !restored.isBusy();
    });
    check(restored.source()->audio[0].size() == 2560, "recorded frame count");
    check(restored.source()->audio.size() == 2, "stereo recording");
    restored.setParam("bypass", 1);
    for (int c = 0; c < 2; ++c)
      for (int i = 0; i < 256; ++i)
        block.setSample(c, i, .123f);
    restored.processBlock(block, midi);
    check(std::abs(block.getSample(0, 100) - .123f) < 1e-6,
          "bypass passes input");
    // Host tempo/meter and independent manual fallback.
    TestPlayHead head;
    restored.setPlayHead(&head);
    restored.processBlock(block, midi);
    auto timing = restored.settings();
    check(timing.bpm == 100 && timing.numerator == 3 && timing.denominator == 8,
          "host tempo and meter");
    head.tempo = false;
    restored.setParam("bpm", 140);
    restored.processBlock(block, midi);
    timing = restored.settings();
    check(timing.bpm == 140 && timing.numerator == 3 && timing.denominator == 8,
          "manual BPM with host meter");
    restored.setParam("sync", 0);
    restored.setParam("numerator", 5);
    timing = restored.settings();
    check(timing.numerator == 5 && timing.denominator == 4,
          "manual meter override");
    restored.setPlayHead(nullptr);
    auto aiff = folder.getChildFile("stereo.aiff");
    {
      AudioBuffer<float> stereo(2, 4410);
      for (int i = 0; i < 4410; ++i) {
        stereo.setSample(0, i, .2f * std::sin(i * .07f));
        stereo.setSample(1, i, .3f * std::cos(i * .04f));
      }
      auto stream = aiff.createOutputStream();
      AiffAudioFormat fmt;
      std::unique_ptr<AudioFormatWriter> w(
          fmt.createWriterFor(stream.get(), 44100, 2, 24, {}, 0));
      check(w != nullptr, "AIFF writer");
      stream.release();
      check(w->writeFromAudioSampleBuffer(stereo, 0, 4410), "AIFF fixture");
    }
    auto stereo = readAudioFile(aiff);
    check(stereo->audio.size() == 2 && stereo->rate == 44100 &&
              stereo->audio[0] != stereo->audio[1],
          "AIFF stereo import");
    MemoryBlock broken;
    broken.setSize(16, true);
    bool bad = false;
    try {
      decodeAudio(broken, "bad");
    } catch (...) {
      bad = true;
    }
    check(bad, "invalid embedded audio rejected");
    // Exercise real component painting and resizing when a display is
    // available.
    if (std::getenv("DISPLAY") != nullptr) {
      std::unique_ptr<AudioProcessorEditor> editor(p.createEditor());
      editor->setSize(900, 1020);
      auto image = editor->createComponentSnapshot(editor->getLocalBounds());
      check(image.isValid(), "editor paint");
      auto snapshot =
          File::getCurrentWorkingDirectory().getChildFile("WONKA-preview.png");
      if (snapshot.existsAsFile())
        check(snapshot.deleteFile(), "remove old snapshot");
      auto png = snapshot.createOutputStream();
      check(png != nullptr && PNGImageFormat().writeImageToStream(image, *png),
            "editor snapshot");
      editor->setSize(1200, 1100);
      check(editor->getWidth() == 1200, "editor resize");
    }
    std::cout << "Import, render, embedded state, preview, PCM24/float32 "
                 "export, recording and bypass passed\n";
    return 0;
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
