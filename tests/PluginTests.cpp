#include "Plugin.h"
#include <chrono>
#include <iostream>
#include <thread>
#if WONKA_ALLOC_PROBE
#include <cstdlib>
#include <new>
thread_local bool watchAudioHeap = false;
thread_local int audioHeapOperations = 0;
extern "C" void *__real_malloc(size_t);
extern "C" void *__real_calloc(size_t, size_t);
extern "C" void *__real_realloc(void *, size_t);
extern "C" void __real_free(void *);
extern "C" void *__wrap_malloc(size_t size) {
  if (watchAudioHeap)
    ++audioHeapOperations;
  return __real_malloc(size);
}
extern "C" void *__wrap_calloc(size_t n, size_t size) {
  if (watchAudioHeap)
    ++audioHeapOperations;
  return __real_calloc(n, size);
}
extern "C" void *__wrap_realloc(void *p, size_t size) {
  if (watchAudioHeap)
    ++audioHeapOperations;
  return __real_realloc(p, size);
}
extern "C" void __wrap_free(void *p) {
  if (watchAudioHeap && p)
    ++audioHeapOperations;
  __real_free(p);
}
void *operator new(size_t n) {
  if (auto *p = std::malloc(n ? n : 1))
    return p;
  throw std::bad_alloc();
}
void *operator new[](size_t n) { return ::operator new(n); }
void operator delete(void *p) noexcept { std::free(p); }
void operator delete[](void *p) noexcept { std::free(p); }
void operator delete(void *p, size_t) noexcept { std::free(p); }
void operator delete[](void *p, size_t) noexcept { std::free(p); }
#endif
using namespace juce;
static int checks = 0;
void check(bool v, const char *name) {
  ++checks;
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
    VoltaProcessor p(folder.getChildFile("cache"));
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
    VoltaProcessor restored(folder.getChildFile("cache-restored"));
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
          block.setSample(c, i, c == 0 ? .1f : .2f);
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
    // Every preset must establish a clean baseline and preserve the source
    // selection.
    restored.setParam("start", .1f);
    restored.setParam("end", .9f);
    for (int i = 0; i < 17; ++i) {
      restored.preset(i);
      check(!restored.presetModified(), "preset clean baseline");
      check(restored.param("start") > .099f && restored.param("end") > .899f,
            "preset preserves selection");
    }
    restored.setParam("space", .123f);
    check(restored.presetModified(), "preset modified marker");
    restored.render();
    waitFor([&] { return !restored.isBusy(); });
    // Reject a render immediately on a parameter edit, before the debounce
    // timer.
    auto staleFile = folder.getChildFile("must-not-export.wav");
    restored.setParam("tone", .93f);
    check(!restored.resultIsCurrent(),
          "parameter edit immediately invalidates result");
    restored.exportFile(staleFile, 24, 48000);
    check(!staleFile.existsAsFile(), "stale export rejected");
    restored.preview(false);
    check(!restored.playing, "stale preview rejected");
    restored.render();
    waitFor([&] { return restored.resultIsCurrent() && !restored.isBusy(); });
    restored.prepareDragFile(32, 48000);
    waitFor([&] { return !restored.isBusy(); });
    auto cached = restored.preparedDragFile(32, 48000);
    check(cached.existsAsFile(), "drag WAV prepared before gesture");
    {
      auto loaded = readAudioFile(cached);
      check(loaded->audio == restored.result()->audio,
            "float WAV samples match rendered samples");
    }
    restored.setParam("shape", .12f);
    check(restored.preparedDragFile(32, 48000) == File{},
          "stale drag WAV rejected");
    // Preview matches output at unity gain, after the explicit 10 ms preview
    // ramp.
    restored.setParam("bypass", 0);
    restored.setParam("volume", 1);
    restored.setParam("tail", 0);
    restored.setParam("musical", 0);
    restored.setParam("mode", 0);
    restored.setParam("length", .1f);
    restored.render();
    waitFor([&] { return restored.resultIsCurrent() && !restored.isBusy(); });
    auto previewClip = restored.result();
    restored.preview(false);
    AudioBuffer<float> previewBlock(2, (int)previewClip->audio[0].size());
    previewBlock.clear();
    restored.processBlock(previewBlock, midi);
    bool previewMatches = true;
    for (int c = 0; c < 2; ++c)
      for (int i = 960; i < previewBlock.getNumSamples(); ++i)
        previewMatches &=
            std::abs(previewBlock.getSample(c, i) -
                     previewClip->audio[std::min(
                         (size_t)c, previewClip->audio.size() - 1)][i]) < 1e-6;
    check(previewMatches,
          "preview matches export buffer after explicit gain ramps");
    check(restored.outputPeak.load() > 0, "output meter measures preview");
    restored.stop();
    for (int k = 0; k < 4; ++k) {
      block.clear();
      restored.processBlock(block, midi);
    }
    check(restored.outputPeak.load() == 0,
          "output meter reaches silence after stop ramp");
    restored.setParam("bypass", 1);
    for (int c = 0; c < 2; ++c)
      for (int i = 0; i < 256; ++i)
        block.setSample(c, i, 1.2f);
    restored.processBlock(block, midi);
    check(restored.inputPeak.load() > 1 && restored.inputClipped.load() &&
              restored.outputClipped.load(),
          "real meter clipping latches");
#if WONKA_ALLOC_PROBE
    restored.setParam("bypass", 0);
    restored.render();
    waitFor([&] { return restored.resultIsCurrent() && !restored.isBusy(); });
    restored.preview(false);
    audioHeapOperations = 0;
    watchAudioHeap = true;
    restored.processBlock(block, midi);
    watchAudioHeap = false;
    check(audioHeapOperations == 0,
          "audio callback performs no new/malloc/calloc/realloc/free");
    restored.stop();
#endif
    // URI drops preserve escaped spaces and unicode filenames.
    check(fileFromDrop(URL(aiff).toString(false)) == aiff,
          "file URL drop decoding");
    check(fileFromDrop("relative.wav") == File{},
          "reject relative drop descriptions");
    auto unicodeFile = folder.getChildFile(String::fromUTF8("voz teste ç.wav"));
    check(fileFromDrop(URL(unicodeFile).toString(false)) == unicodeFile,
          "unicode file URL decoding");
    VoltaProcessor failed(folder.getChildFile("cache-failed"));
    failed.prepareToPlay(48000, 256);
    failed.importFile(aiff);
    waitFor([&] { return failed.resultIsCurrent() && !failed.isBusy(); });
    failed.importFile(folder.getChildFile("missing.wav"));
    waitFor([&] { return !failed.isBusy(); });
    check(failed.phase.load() == VoltaProcessor::Phase::Error &&
              failed.status().contains("encontrado"),
          "import error has explicit cause");
    std::this_thread::sleep_for(std::chrono::milliseconds(160));
    Timer::callPendingTimersSynchronously();
    waitFor([&] { return !failed.isBusy(); });
    check(!failed.resultIsCurrent() && !failed.result() &&
              failed.phase.load() == VoltaProcessor::Phase::Error,
          "failed import cannot rerender the previous source on timer");
    // The recorder stops at its documented capacity, not at an arbitrary render
    // length.
    VoltaProcessor limit(folder.getChildFile("cache-limit"));
    limit.prepareToPlay(8000, 8000);
    AudioBuffer<float> second(2, 8000);
    second.clear();
    limit.record();
    for (int i = 0; i < 61; ++i)
      limit.processBlock(second, midi);
    check(limit.recordingAtLimit.load(), "recording limit signalled");
    limit.serviceRecording();
    waitFor([&] { return !limit.isBusy(); });
    check(limit.source() && limit.source()->audio[0].size() == 480000,
          "recording capped at exactly 60 seconds");
    check(limit.status().contains("60 s"), "recording limit status retained");
    // Empty session restores must clear the previous imported audio.
    MemoryBlock blank;
    VoltaProcessor empty(folder.getChildFile("cache-empty"));
    empty.getStateInformation(blank);
    restored.setStateInformation(blank.getData(), (int)blank.getSize());
    check(!restored.source() && !restored.result(),
          "empty session clears buffers");
    check(cached.existsAsFile(),
          "prepared WAV retained after replacing session");
    // Exercise real component painting and resizing when a display is
    // available.
    if (std::getenv("DISPLAY") != nullptr) {
      std::unique_ptr<AudioProcessorEditor> editor(p.createEditor());
      check(editor->getWidth() == 360 && editor->getHeight() == 640,
            "compact default size");
      auto *content = editor->getChildComponent(0);
      check(content != nullptr, "content exists");
      auto *drop = dynamic_cast<FileDragAndDropTarget *>(content);
      check(drop != nullptr, "editor accepts file drops");
      drop->filesDropped({"unsupported.mp3"}, 80, 150);
      check(p.phase.load() == VoltaProcessor::Phase::Error && !p.result(),
            "unsupported UI drop has explicit error and invalidates output");
      FileDragAndDropTarget *displayDrop = nullptr;
      for (int i = 0; i < content->getNumChildComponents(); ++i)
        if (dynamic_cast<DragAndDropTarget *>(content->getChildComponent(i)))
          displayDrop = dynamic_cast<FileDragAndDropTarget *>(
              content->getChildComponent(i));
      check(displayDrop != nullptr, "waveform is a direct drop target");
      displayDrop->filesDropped({URL(aiff).toString(false)}, 80, 150);
      waitFor([&] { return p.resultIsCurrent() && !p.isBusy(); });
      editor.reset(p.createEditor());
      check(p.source()->name == aiff.getFileName(),
            "drop loads AIFF source through UI");
      // Selecting tabs must never overwrite another tab's value.
      p.setParam("tone", .37f);
      p.setParam("shape", .61f);
      p.setParam("tremolo", .22f);
      p.setParam("bars", 2.5f);
      auto *surfaceForTabs = editor->getChildComponent(0);
      for (auto name : {"Tone", "Shape", "Tremolo", "Bars"}) {
        bool found = false;
        for (int i = 0; i < surfaceForTabs->getNumChildComponents(); ++i)
          if (auto *button =
                  dynamic_cast<Button *>(surfaceForTabs->getChildComponent(i)))
            if (button->getButtonText() == name) {
              button->onClick();
              found = true;
              break;
            }
        check(found, "tab control is real");
      }
      check(std::abs(p.param("tone") - .37f) < 1e-5 &&
                std::abs(p.param("shape") - .61f) < 1e-5 &&
                std::abs(p.param("tremolo") - .22f) < 1e-5 &&
                std::abs(p.param("bars") - 2.5f) < 1e-5,
            "tab values preserved independently");
      // Return the visible tab to Tone for the generated preview.
      for (int i = 0; i < surfaceForTabs->getNumChildComponents(); ++i)
        if (auto *b =
                dynamic_cast<Button *>(surfaceForTabs->getChildComponent(i)))
          if (b->getButtonText() == "Tone")
            b->onClick();
      p.render();
      waitFor([&] { return p.resultIsCurrent() && !p.isBusy(); });
      editor.reset(p.createEditor());
      bool marked = false;
      auto *presetSurface = editor->getChildComponent(0);
      for (int i = 0; i < presetSurface->getNumChildComponents(); ++i)
        if (auto *c =
                dynamic_cast<ComboBox *>(presetSurface->getChildComponent(i)))
          if (c->getNumItems() == 17)
            marked = c->getText().startsWith("*");
      check(marked, "modified preset marker visible in selected label");
      editor->setSize(360, 640);
      auto image = editor->createComponentSnapshot(editor->getLocalBounds());
      check(image.isValid(), "editor paint");
      auto snapshot =
          File::getCurrentWorkingDirectory().getChildFile("WONKA-preview.png");
      if (snapshot.existsAsFile())
        check(snapshot.deleteFile(), "remove old snapshot");
      auto png = snapshot.createOutputStream();
      check(png != nullptr && PNGImageFormat().writeImageToStream(image, *png),
            "editor snapshot");
      // Secondary options replace the knob area and stay within the same
      // canvas.
      Button *gear = nullptr;
      auto *optionsSurface = editor->getChildComponent(0);
      for (int i = 0; i < optionsSurface->getNumChildComponents(); ++i)
        if (auto *b =
                dynamic_cast<Button *>(optionsSurface->getChildComponent(i)))
          if (b->getButtonText() == "...")
            gear = b;
      check(gear != nullptr, "secondary options button");
      gear->onClick();
      bool optionsFound = false;
      for (int i = 0; i < optionsSurface->getNumChildComponents(); ++i) {
        auto *c = optionsSurface->getChildComponent(i);
        if (c->isVisible() && c->getNumChildComponents() > 10) {
          optionsFound = true;
          for (int j = 0; j < c->getNumChildComponents(); ++j)
            check(c->getLocalBounds().contains(
                      c->getChildComponent(j)->getBounds()),
                  "secondary control fits panel");
        }
      }
      check(optionsFound, "secondary panel visible");
      auto optionsImage =
          editor->createComponentSnapshot(editor->getLocalBounds());
      auto optionsFile =
          File::getCurrentWorkingDirectory().getChildFile("WONKA-options.png");
      optionsFile.deleteFile();
      auto optionsStream = optionsFile.createOutputStream();
      check(optionsStream != nullptr && PNGImageFormat().writeImageToStream(
                                            optionsImage, *optionsStream),
            "options snapshot");
      optionsStream.reset();
      gear->onClick();
      for (double scale : {.7, .85, 1., 1.25}) {
        editor->setSize(roundToInt(360 * scale), roundToInt(640 * scale));
        auto *surface = editor->getChildComponent(0);
        check(std::abs(surface->getTransform().mat00 - scale) < .002,
              "single logical content scale");
        for (int i = 0; i < surface->getNumChildComponents(); ++i) {
          auto *child = surface->getChildComponent(i);
          if (child->isVisible())
            check(surface->getLocalBounds().contains(child->getBounds()),
                  "visible control within logical canvas");
        }
      }
      MemoryBlock uiState;
      p.getStateInformation(uiState);
      editor.reset();
      VoltaProcessor uiRestored(folder.getChildFile("cache-ui"));
      uiRestored.setStateInformation(uiState.getData(), (int)uiState.getSize());
      std::unique_ptr<AudioProcessorEditor> reopened(uiRestored.createEditor());
      check(reopened->getWidth() == 450 && reopened->getHeight() == 800,
            "saved editor size restored");
    }
    std::cout << checks
              << " integration checks passed. Import, render, embedded state, "
                 "preview, PCM24/float32 "
                 "export, recording and bypass passed\n";
    return 0;
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
