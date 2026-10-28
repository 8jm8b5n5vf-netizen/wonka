#include "AudioIO.h"
using namespace juce;
void Clip::buildPeaks() {
  suggested = volta::suggest(audio);
  peaks.clear();
  for (auto &channel : audio) {
    std::vector<float> values(4096, 0);
    size_t n = channel.size();
    for (size_t i = 0; i < n; ++i) {
      size_t bin = std::min<size_t>(2047, i * 2048 / n);
      values[bin * 2] = std::min(values[bin * 2], channel[i]);
      values[bin * 2 + 1] = std::max(values[bin * 2 + 1], channel[i]);
    }
    peaks.push_back(std::move(values));
  }
}
std::shared_ptr<Clip> readAudioFile(const File &file) {
  if (!file.hasFileExtension("wav;aif;aiff"))
    throw std::runtime_error("Use WAV ou AIFF");
  AudioFormatManager formats;
  formats.registerBasicFormats();
  std::unique_ptr<AudioFormatReader> r(formats.createReaderFor(file));
  if (!r)
    throw std::runtime_error("Arquivo de audio invalido");
  if (r->numChannels < 1 || r->numChannels > 2 || r->sampleRate < 8000 ||
      r->sampleRate > 192000 || r->lengthInSamples <= 0 ||
      r->lengthInSamples > r->sampleRate * 60)
    throw std::runtime_error("Limite: 60 s, mono/estereo, 8-192 kHz");
  AudioBuffer<float> buffer((int)r->numChannels, (int)r->lengthInSamples);
  if (!r->read(&buffer, 0, buffer.getNumSamples(), 0, true, true))
    throw std::runtime_error("Falha na leitura");
  auto clip = std::make_shared<Clip>();
  clip->rate = r->sampleRate;
  clip->name = file.getFileName();
  for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    clip->audio.emplace_back(buffer.getReadPointer(ch),
                             buffer.getReadPointer(ch) +
                                 buffer.getNumSamples());
  for (auto &channel : clip->audio)
    for (auto &v : channel)
      if (!std::isfinite(v))
        v = 0;
  clip->buildPeaks();
  return clip;
}
void writeAudioFile(const Clip &clip, const File &file, int bits, double rate,
                    bool protect) {
  if (bits != 24 && bits != 32)
    throw std::runtime_error("Use PCM24 ou float32");
  auto audio = volta::resample(clip.audio, clip.rate, rate);
  AudioBuffer<float> buffer((int)audio.size(), (int)audio[0].size());
  for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
    if (protect)
      for (auto &v : audio[ch])
        v = std::clamp(v, -.999f, .999f);
    buffer.copyFrom(ch, 0, audio[ch].data(), buffer.getNumSamples());
  }
  TemporaryFile temp(file);
  auto stream = temp.getFile().createOutputStream();
  if (!stream)
    throw std::runtime_error("Nao foi possivel criar WAV");
  WavAudioFormat format;
  std::unique_ptr<AudioFormatWriter> writer(format.createWriterFor(
      stream.get(), rate, (unsigned)buffer.getNumChannels(), bits, {}, 0));
  if (!writer)
    throw std::runtime_error("Falha no encoder WAV");
  stream.release();
  if (!writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples()))
    throw std::runtime_error("Falha ao escrever WAV");
  writer.reset();
  if (!temp.overwriteTargetFileWithTemporary())
    throw std::runtime_error("Falha ao salvar WAV");
}
MemoryBlock encodeAudio(const Clip &clip) {
  MemoryOutputStream stream;
  stream.writeInt((int)clip.audio.size());
  stream.writeInt((int)clip.audio[0].size());
  stream.writeDouble(clip.rate);
  for (auto &channel : clip.audio)
    for (float v : channel)
      stream.writeFloat(v);
  return stream.getMemoryBlock();
}
std::shared_ptr<Clip> decodeAudio(const MemoryBlock &bytes, String name) {
  MemoryInputStream in(bytes, false);
  int channels = in.readInt(), samples = in.readInt();
  double rate = in.readDouble();
  if (channels < 1 || channels > 2 || !std::isfinite(rate) || rate < 8000 ||
      rate > 192000 || samples < 1 || samples > rate * 60 ||
      bytes.getSize() != 16 + (size_t)channels * samples * 4)
    throw std::runtime_error("Audio da sessao invalido");
  auto c = std::make_shared<Clip>();
  c->autoSelect = false;
  c->rate = rate;
  c->name = std::move(name);
  c->audio = volta::Audio(channels, std::vector<float>(samples));
  for (auto &channel : c->audio)
    for (auto &v : channel) {
      v = in.readFloat();
      if (!std::isfinite(v))
        v = 0;
    }
  c->buildPeaks();
  return c;
}
