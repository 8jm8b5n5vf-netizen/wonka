#include "Engine.h"
#include <iostream>
#include <limits>
using namespace volta;
static int checks = 0;
void check(bool v, const char *name) {
  ++checks;
  if (!v)
    throw std::runtime_error(name);
}
static float energy(const std::vector<float> &a, size_t b, size_t e) {
  float v = 0;
  for (size_t i = b; i < std::min(e, a.size()); ++i)
    v += a[i] * a[i];
  return v;
}
int main() {
  try {
    constexpr int sr = 48000;
    Audio src(2, std::vector<float>(sr));
    for (int i = 0; i < sr; ++i) {
      src[0][i] = .4f * std::sin(i * .04f);
      src[1][i] = .2f * std::cos(i * .03f);
    }
    Settings s;
    s.length = 2;
    s.tail = false;
    auto line = render(src, sr, s);
    check(line.size() == 2, "reverse stereo");
    check(line[0].size() == 96000, "reverse length");
    check(energy(line[0], 0, 96000) > 1, "reverse non-silent");
    check(line[0].front() == 0 && line[0].back() == 0, "edge fades");
    s.variant = 1;
    auto swell = render(src, sr, s);
    check(swell != line, "swell differs from line");
    check(energy(swell[0], 0, 96000) > 0, "swell non-silent");
    s.tail = true;
    auto tail = render(src, sr, s);
    check(tail[0].size() > line[0].size(), "tail included");
    s.musical = true;
    s.bars = 2;
    s.numerator = 3;
    s.denominator = 4;
    s.bpm = 120;
    check(std::abs(duration(s) - 3) < 1e-9, "3/4 bars");
    s.denominator = 8;
    check(std::abs(duration(s) - 1.5) < 1e-9, "3/8 bars");
    check(beatDivision(2) == .75, "dotted");
    check(std::abs(beatDivision(3) - 1. / 3) < 1e-9, "triplet");
    s = Settings{};
    s.mode = 1;
    s.length = 2;
    s.division = 1;
    s.feedback = .94;
    s.space = 0;
    Audio impulse(1, std::vector<float>(480));
    impulse[0][100] = .8f;
    auto ping = render(impulse, sr, s);
    check(ping.size() == 2, "mono expands for ping-pong");
    size_t d = 12000;
    check(energy(ping[0], d, d + 480) > 0, "left first echo");
    check(energy(ping[1], d, d + 480) == 0, "no right first echo");
    check(energy(ping[1], 2 * d, 2 * d + 480) > 0, "right second echo");
    check(ping[0].size() == 96480, "throw length preserves source plus tail");
    for (auto &c : ping)
      for (float v : c)
        if (!std::isfinite(v) || std::abs(v) > 1)
          throw std::runtime_error("unstable feedback");
    check(true, "finite feedback");
    s.original = true;
    auto original = render(impulse, sr, s);
    check(energy(original[0], 0, 480) > 0, "original inclusion");
    auto converted = resample(src, 48000, 44100);
    check(converted.size() == 2 && converted[0].size() == 44100,
          "sample-rate conversion");
    Audio words(1, std::vector<float>(1000));
    for (int i = 100; i < 300; ++i)
      words[0][i] = .5f;
    for (int i = 700; i < 900; ++i)
      words[0][i] = .5f;
    auto sel = suggest(words);
    check(sel.first > .6 && sel.second > .85 && sel.second < .95,
          "final region suggestion");
    s.start = .8;
    s.end = .2;
    bool rejected = false;
    try {
      render(src, sr, s);
    } catch (...) {
      rejected = true;
    }
    check(rejected, "invalid selection rejected");
    s = Settings{};
    s.normalise = true;
    s.tail = false;
    auto norm = render(src, sr, s);
    float peak = 0;
    for (auto &c : norm)
      for (auto v : c)
        peak = std::max(peak, std::abs(v));
    check(std::abs(peak - .98f) < 1e-4f, "explicit normalisation");
    // Exact timing and channel-content regressions.
    s = Settings{};
    s.mode = 1;
    s.space = 0;
    s.length = 2;
    s.bpm = 60;
    s.division = 2;
    auto slow = render(impulse, sr, s);
    check(energy(slow[0], 36000, 36480) > 0 && energy(slow[0], 0, 36000) == 0,
          "dotted echo at 60 BPM");
    s.bpm = 120;
    s.division = 3;
    auto triplet = render(impulse, sr, s);
    check(energy(triplet[0], 8000, 8480) > 0 &&
              energy(triplet[0], 0, 8000) == 0,
          "triplet echo timing");
    Audio antiphase(2, std::vector<float>(480));
    antiphase[0][100] = .5f;
    antiphase[1][100] = -.5f;
    auto anti = render(antiphase, sr, s);
    check(energy(anti[0], 0, anti[0].size()) > 0 &&
              energy(anti[1], 0, anti[1].size()) > 0,
          "stereo antiphase not cancelled");
    Audio high(1, std::vector<float>(48000));
    for (int i = 0; i < 48000; ++i)
      high[0][i] = (float)std::sin(i * 2 * 3.141592653589793 * 20000 / 48000);
    auto lowrate = resample(high, 48000, 16000);
    check(energy(lowrate[0], 100, 15900) / 15800 < .001f,
          "downsample anti-alias filter");
    s = Settings{};
    s.tail = false;
    s.length = .1;
    s.start = .5;
    s.end = 1;
    Audio distinct(2, std::vector<float>(4800));
    distinct[0][3600] = .8f;
    auto selected = render(distinct, sr, s);
    check(energy(selected[0], 0, selected[0].size()) > 0,
          "selection contains signal");
    s.start = 0;
    s.end = .5;
    auto emptyRegion = render(distinct, sr, s);
    check(energy(emptyRegion[0], 0, emptyRegion[0].size()) == 0,
          "selection excludes signal");
    s.start = std::numeric_limits<double>::quiet_NaN();
    bool bad = false;
    try {
      render(distinct, sr, s);
    } catch (...) {
      bad = true;
    }
    check(bad, "NaN parameter rejected");
    s = Settings{};
    s.tail = false;
    distinct[0][100] = std::numeric_limits<float>::quiet_NaN();
    auto finite = render(distinct, sr, s);
    bool valid = true;
    for (auto &channel : finite)
      for (float value : channel)
        valid &= std::isfinite(value);
    check(valid, "NaN sample sanitised");
    std::cout << checks << " checks passed\n";
    return 0;
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
