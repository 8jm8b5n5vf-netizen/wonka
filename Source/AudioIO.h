#pragma once
#include "Engine.h"
#include <juce_audio_formats/juce_audio_formats.h>
struct Clip {
  volta::Audio audio;
  // Interleaved minimum/maximum values, generated on the worker, never on
  // paint.
  volta::Audio peaks;
  double rate = 48000;
  juce::String name;
  void buildPeaks();
};
std::shared_ptr<Clip> readAudioFile(const juce::File &);
void writeAudioFile(const Clip &, const juce::File &, int bits, double rate,
                    bool protect);
juce::MemoryBlock encodeAudio(const Clip &);
std::shared_ptr<Clip> decodeAudio(const juce::MemoryBlock &, juce::String name);

juce::File fileFromDrop(const juce::String &);
