#pragma once
#include "AudioIO.h"
#include <atomic>
#include <juce_audio_utils/juce_audio_utils.h>
#include <mutex>
class VoltaProcessor final : public juce::AudioProcessor, private juce::Timer {
public:
  VoltaProcessor();
  ~VoltaProcessor() override;
  juce::AudioProcessorValueTreeState state;
  void prepareToPlay(double, int) override;
  void releaseResources() override {}
  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
  bool isBusesLayoutSupported(const BusesLayout &) const override;
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override { return true; }
  const juce::String getName() const override { return "WONKA"; }
  bool acceptsMidi() const override { return false; }
  bool producesMidi() const override { return false; }
  double getTailLengthSeconds() const override { return 0; }
  int getNumPrograms() override { return 1; }
  int getCurrentProgram() override { return 0; }
  void setCurrentProgram(int) override {}
  const juce::String getProgramName(int) override { return {}; }
  void changeProgramName(int, const juce::String &) override {}
  void getStateInformation(juce::MemoryBlock &) override;
  void setStateInformation(const void *, int) override;
  volta::Settings settings() const;
  void importFile(juce::File);
  void render();
  void serviceRecording();
  void record();
  void stopRecord();
  void preview(bool source);
  void stop();
  void exportFile(juce::File, int, double);
  void preset(int);
  std::shared_ptr<const Clip> source() const;
  std::shared_ptr<const Clip> result() const;
  juce::String status() const;
  void setStatus(juce::String);
  std::atomic<double> bpm{120}, hostRate{48000}, position{0};
  std::atomic<int> hostNum{4}, hostDen{4}, recordState{0};
  std::atomic<bool> hostTempo{false}, hostSignature{false}, playing{false};
  bool isBusy() const {
    return activeJobs.load() > 0 || dirtyRender.load() ||
           recordState.load() != 0;
  }
  std::atomic<int> revision{0};
  void setParam(const char *, float);
  float param(const char *) const;

private:
  void timerCallback() override;
  std::vector<double> lastRenderKey;
  int pendingTicks = 0, lastAutoMode = 0;
  bool sawMode = false;
  std::shared_ptr<const Clip> lastAutoSource;
  static juce::AudioProcessorValueTreeState::ParameterLayout layout();
  void queue(std::function<void()>);
  void publish(std::shared_ptr<const Clip>);
  void load(std::shared_ptr<const Clip>);
  juce::ThreadPool pool{1};
  mutable std::mutex mutex;
  std::shared_ptr<const Clip> input, output, current;
  std::vector<std::shared_ptr<const Clip>> retired;
  juce::String message = "Abra ou arraste WAV / AIFF";
  std::atomic<const Clip *> playback{nullptr}, hazard{nullptr};
  std::atomic<unsigned> playCommand{0};
  unsigned seenCommand = 0;
  double cursor = 0;
  std::mutex captureMutex;
  juce::AudioBuffer<float> capture;
  int captured = 0;
  std::atomic<int> capturedCount{0};
  juce::SmoothedValue<float> volume;
  std::atomic<int> renderGeneration{0}, activeJobs{0};
  std::atomic<bool> dirtyRender{false};
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoltaProcessor)
};
