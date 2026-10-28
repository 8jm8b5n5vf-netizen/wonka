#include "Engine.h"
namespace volta {
double duration(const Settings &s) {
  return std::clamp(s.musical ? s.bars * s.numerator * 4.0 / s.denominator *
                                    60 / std::clamp(s.bpm, 30., 300.)
                              : s.length,
                    .05, 30.);
}
double beatDivision(int i) {
  static const double d[] = {1, .5, .75, 1. / 3, .25, .375, 1. / 6, 2};
  return d[std::clamp(i, 0, 7)];
}
std::pair<double, double> suggest(const Audio &a) {
  if (a.empty() || a[0].empty())
    return {0, 1};
  const int n = (int)a[0].size(), w = std::max(1, n / 500);
  float peak = 0;
  std::vector<float> env;
  for (int i = 0; i < n; i += w) {
    float v = 0;
    for (int j = i; j < std::min(n, i + w); ++j)
      for (auto &c : a)
        v = std::max(v, std::abs(c[j]));
    env.push_back(v);
    peak = std::max(peak, v);
  }
  if (peak < 1e-6f)
    return {0, 1};
  int e = (int)env.size() - 1;
  while (e > 0 && env[e] < peak * .04f)
    --e;
  int b = e;
  while (b > 0 && env[b] > peak * .04f)
    --b;
  if (e - b < 3)
    b = std::max(0, e - 40);
  return {double(b * w) / n, std::min(1., double((e + 1) * w) / n)};
}
Audio resample(const Audio &a, double from, double to) {
  if (!std::isfinite(from) || !std::isfinite(to) || from <= 0 || to <= 0)
    throw std::runtime_error("Invalid sample rate");
  if (a.empty() || a[0].empty())
    return {};
  for (auto &channel : a)
    if (channel.size() != a[0].size())
      throw std::runtime_error("Channel size mismatch");
  if (from == to)
    return a;
  const size_t n =
      std::max<size_t>(1, (size_t)std::llround(a[0].size() * to / from));
  Audio o(a.size(), std::vector<float>(n));
  const double cutoff = std::min(1., to / from) * .94;
  constexpr int taps = 24;
  constexpr double pi = 3.14159265358979323846;
  for (size_t i = 0; i < n; ++i) {
    double p = i * from / to;
    long centre = (long)std::floor(p);
    double weights[taps * 2], sum = 0;
    for (int t = -taps + 1; t <= taps; ++t) {
      double x = (centre + t - p) * cutoff;
      double sinc = std::abs(x) < 1e-12 ? 1. : std::sin(pi * x) / (pi * x);
      double window = .5 + .5 * std::cos(pi * (centre + t - p) / taps);
      weights[t + taps - 1] = sinc * window * cutoff;
      sum += weights[t + taps - 1];
    }
    for (size_t c = 0; c < a.size(); ++c) {
      double value = 0;
      for (int t = -taps + 1; t <= taps; ++t) {
        long j = std::clamp(centre + t, 0L, (long)a[c].size() - 1);
        float sample = a[c][j];
        value += (std::isfinite(sample) ? sample : 0) * weights[t + taps - 1];
      }
      o[c][i] = (float)(value / sum);
    }
  }
  return o;
}
static Audio reverb(const Audio &a, double sr, const Settings &s) {
  Audio o = a;
  const double delays[] = {.0297, .0371, .0411, .0437, .0531, .0617};
  for (size_t c = 0; c < a.size(); ++c) {
    std::fill(o[c].begin(), o[c].end(), 0);
    for (double d : delays) {
      int len =
          std::max(1, (int)(sr * d * (.65 + s.space * 1.6) * (1 + c * .07)));
      std::vector<float> line(len);
      int p = 0;
      float low = 0;
      const float gain =
          (float)std::pow(.001, len / (sr * (.25 + s.space * 5.75)));
      for (size_t i = 0; i < a[c].size(); ++i) {
        float v = line[p];
        low += float(.08 + s.tone * .65) * (v - low);
        line[p] = a[c][i] + low * gain;
        o[c][i] += v / 6;
        p = (p + 1) % len;
      }
    }
    // Two all-pass stages diffuse each comb bank. Space changes their feedback.
    for (double d : {.0047, .0017}) {
      int len = std::max(1, (int)(sr * d * (.7 + s.space)));
      std::vector<float> delay(len);
      int pos = 0;
      float gain = (float)(.2 + s.space * .5);
      for (auto &v : o[c]) {
        float old = delay[pos], y = old - gain * v;
        delay[pos] = v + gain * y;
        v = y;
        pos = (pos + 1) % len;
      }
    }
  }
  if (o.size() == 2)
    for (size_t i = 0; i < o[0].size(); ++i) {
      float l = o[0][i], r = o[1][i];
      o[0][i] = float(l * (1 - s.space * .3) + r * s.space * .3);
      o[1][i] = float(r * (1 - s.space * .3) + l * s.space * .3);
    }
  return o;
}
Audio render(const Audio &input, double sr, const Settings &s) {
  if (input.empty() || input.size() > 2 || sr < 8000 || sr > 192000)
    throw std::runtime_error("Unsupported audio");
  for (double v : {s.start, s.end, s.bpm, s.length, s.bars, s.space, s.tone,
                   s.shape, s.tremolo, s.feedback})
    if (!std::isfinite(v))
      throw std::runtime_error("Invalid parameter");
  if (s.denominator < 1 || s.numerator < 1 || s.space < 0 || s.space > 1 ||
      s.tone < 0 || s.tone > 1 || s.shape < 0 || s.shape > 1 || s.tremolo < 0 ||
      s.tremolo > 1)
    throw std::runtime_error("Parameter outside range");
  size_t n = input[0].size();
  if (!n)
    throw std::runtime_error("Empty audio");
  for (auto &c : input)
    if (c.size() != n)
      throw std::runtime_error("Channel size mismatch");
  size_t b = std::min(n - 1, (size_t)(std::clamp(s.start, 0., 1.) * n)),
         e = std::min(n, (size_t)(std::clamp(s.end, 0., 1.) * n));
  if (e <= b)
    throw std::runtime_error("Empty selection");
  Audio src(input.size());
  for (size_t c = 0; c < src.size(); ++c) {
    src[c].assign(input[c].begin() + b, input[c].begin() + e);
    for (size_t i = 0; i < src[c].size(); ++i) {
      double fade = std::min(
          {1., i / (sr * .005), (src[c].size() - 1 - i) / (sr * .005)});
      src[c][i] = std::isfinite(src[c][i]) ? src[c][i] * (float)fade : 0;
    }
  }
  double seconds = duration(s);
  Audio out;
  if (s.mode == 0) {
    const size_t main = (size_t)std::llround(seconds * sr),
                 extra = s.tail ? (size_t)(sr * (.25 + s.space * 5.75)) : 0;
    auto stretched = resample(src, (double)src[0].size(), (double)main);
    for (auto &c : stretched)
      c.resize(main + extra, 0);
    if (s.variant == 0) {
      for (auto &c : stretched)
        std::reverse(c.begin(), c.begin() + main);
      auto wet = reverb(stretched, sr, s);
      out = stretched;
      for (size_t c = 0; c < out.size(); ++c)
        for (size_t i = 0; i < out[c].size(); ++i)
          out[c][i] = out[c][i] * .45f + wet[c][i] * .8f;
    } else {
      out = reverb(stretched, sr, s);
      for (auto &c : out)
        std::reverse(c.begin(), c.end());
    }
    for (auto &c : out)
      for (size_t i = 0; i < c.size(); ++i)
        c[i] *= (float)std::pow(double(i + 1) / c.size(), .2 + s.shape * 3.8);
  } else {
    size_t delay = std::max<size_t>(
               1, (size_t)(sr * 60 / std::clamp(s.bpm, 30., 300.) *
                           beatDivision(s.division))),
           total = src[0].size() + (size_t)(seconds * sr);
    out = Audio(2, std::vector<float>(total));
    if (s.original)
      for (size_t c = 0; c < 2; ++c)
        std::copy(src[std::min(c, src.size() - 1)].begin(),
                  src[std::min(c, src.size() - 1)].end(), out[c].begin());
    Audio echo = src;
    double gain = 1;
    int k = 1;
    const double fb = std::clamp(s.feedback, 0., .94);
    for (size_t offset = delay; offset < total; offset += delay, ++k) {
      float alpha = (float)(.025 + .75 * s.tone / (1 + k * .15));
      for (auto &c : echo) {
        float low = 0;
        for (auto &v : c) {
          low += alpha * (v - low);
          v = low;
        }
      }
      for (size_t i = 0; i < echo[0].size() && offset + i < total; ++i) {
        const float envelope =
            (float)(gain * std::pow(1. - double(offset) / total, s.shape * 2));
        const int side = (k - 1) % 2;
        out[side][offset + i] += echo[0][i] * envelope;
        if (echo.size() == 2)
          out[1 - side][offset + i] += echo[1][i] * envelope * .25f;
      }
      gain *= fb;
      if (gain < 1e-7)
        break;
    }
    if (s.space > 0) {
      auto ambience = reverb(out, sr, s);
      for (size_t c = 0; c < out.size(); ++c)
        for (size_t i = 0; i < out[c].size(); ++i)
          out[c][i] += (float)(ambience[c][i] * s.space * .3);
    }
  }
  float peak = 0;
  for (auto &c : out) {
    float low = 0;
    for (size_t i = 0; i < c.size(); ++i) {
      float v = std::isfinite(c[i]) ? c[i] : 0;
      low += (float)(.03 + s.tone * .95) * (v - low);
      v = low *
          (float)(1 - s.tremolo * .5 *
                          (1 - std::cos(2 * 3.141592653589793 * i / sr *
                                        (s.tremRate == 3
                                             ? 4.
                                             : s.bpm / 60 *
                                                   (1 << std::clamp(s.tremRate,
                                                                    0, 2))))));
      double edge =
          std::min({1., i / (sr * .005), (c.size() - 1 - i) / (sr * .005)});
      c[i] = v * (float)edge;
      peak = std::max(peak, std::abs(c[i]));
    }
  }
  for (auto &c : out)
    for (auto &v : c) {
      if (s.normalise && peak > 0)
        v *= .98f / peak;
      if (s.protect)
        v = std::clamp(v, -.999f, .999f);
    }
  return out;
}
} // namespace volta
