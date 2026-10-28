#pragma once
#include <array>
namespace volta {
struct Preset {
  const char *name;
  int mode, variant, division;
  float length, space, tone, shape, tremolo, feedback, bars;
  bool musical;
};
inline const std::array<Preset, 17> &presets() {
  static const std::array<Preset, 17> values{
      {{"Vocal lift", 0, 0, 2, 2, .5, .5, .5, 0, .5, 1, false},
       {"Short inhale", 0, 0, 1, .65, .22, .76, .24, 0, .35, .25, false},
       {"Soft entrance", 0, 0, 2, 1.5, .42, .42, .3, 0, .45, 1, false},
       {"Glass reverse", 0, 0, 2, 3, .65, .88, .66, 0, .45, 1, false},
       {"Dark lead-in", 0, 0, 2, 4, .78, .24, .7, 0, .55, 2, false},
       {"One bar lift", 0, 0, 2, 2, .55, .62, .52, 0, .45, 1, true},
       {"Air swell", 0, 1, 2, 3, .64, .8, .44, 0, .5, 1, false},
       {"Long cloud", 0, 1, 2, 8, .92, .48, .7, 0, .5, 4, false},
       {"Velvet swell", 0, 1, 2, 5, .76, .29, .38, 0, .5, 2, false},
       {"Pulsing rise", 0, 1, 2, 4, .7, .64, .58, .65, .5, 2, false},
       {"Two bar bloom", 0, 1, 2, 4, .84, .62, .52, .12, .5, 2, true},
       {"Dotted throw", 1, 0, 2, 3, .16, .7, .28, 0, .5, 1, false},
       {"Dark ping-pong", 1, 0, 1, 5, .3, .26, .44, 0, .72, 2, false},
       {"Triplet trail", 1, 0, 3, 3, .12, .66, .3, 0, .6, 1, false},
       {"Quarter answer", 1, 0, 0, 4, .24, .58, .2, 0, .55, 2, false},
       {"Sixteenth sparks", 1, 0, 4, 2, .1, .85, .5, .18, .64, 1, false},
       {"Two bar echoes", 1, 0, 2, 4, .36, .45, .35, 0, .7, 2, true}}};
  return values;
}
} // namespace volta
