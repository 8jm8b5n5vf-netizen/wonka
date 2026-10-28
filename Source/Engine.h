#pragma once
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>
namespace volta {
using Audio = std::vector<std::vector<float>>;
struct Settings {
  int mode = 0, variant = 0, division = 2, tremRate = 1, numerator = 4,
      denominator = 4;
  double bpm = 120, length = 1, bars = 1, space = .5, tone = .5, shape = .5,
         tremolo = 0, feedback = .5;
  double start = 0, end = 1;
  bool musical = false, tail = true, original = false, normalise = false,
       protect = true;
};
double duration(const Settings &);
double toneFrequency(double normalised, double sampleRate = 48000);
double beatDivision(int);
std::pair<double, double> suggest(const Audio &);
Audio resample(const Audio &, double, double);
Audio render(const Audio &, double, const Settings &);
} // namespace volta
