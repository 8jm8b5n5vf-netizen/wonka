#include "Plugin.h"
#if WONKA_HAS_LOGO
#include <BinaryData.h>
#endif
using namespace juce;
namespace {
class Metal final : public LookAndFeel_V4 {
  Image face{Image::ARGB, 512, 512, true};

public:
  Metal() {
    Image::BitmapData pixels(face, Image::BitmapData::writeOnly);
    for (int y = 0; y < 512; ++y)
      for (int x = 0; x < 512; ++x) {
        float dx = (x - 255.5f) / 255.5f, dy = (y - 255.5f) / 255.5f,
              r = std::sqrt(dx * dx + dy * dy);
        if (r > 1)
          continue;
        float a = std::atan2(dx, -dy);
        float light = .44f + .2f * std::cos(a - .8f) +
                      .23f * std::cos(2 * a + 1.3f) + .075f * std::sin(3 * a) -
                      .06f * r + .004f * std::sin(r * 740);
        float alpha = std::clamp((1 - r) * 256, 0.f, 1.f);
        pixels.setPixelColour(
            x, y,
            Colour::fromFloatRGBA(std::clamp(light + .02f, 0.f, 1.f),
                                  std::clamp(light + .025f, 0.f, 1.f),
                                  std::clamp(light + .035f, 0.f, 1.f), alpha));
      }
    setColour(ResizableWindow::backgroundColourId, Colour(0xff171b23));
    setColour(Slider::textBoxTextColourId, Colours::white);
    setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    setColour(TextButton::buttonColourId, Colour(0xff292f3d));
    setColour(TextButton::buttonOnColourId, Colour(0xff266b70));
    setColour(ComboBox::backgroundColourId, Colour(0xff252a36));
    setColour(ComboBox::outlineColourId, Colour(0xff464f62));
  }
  void drawButtonBackground(Graphics &g, Button &button, const Colour &,
                            bool hover, bool down) override {
    auto r = button.getLocalBounds().toFloat().reduced(1.5f);
    bool on = button.getToggleState();
    DropShadow(Colours::black.withAlpha(.7f), 5, {0, 3})
        .drawForRectangle(g, r.getSmallestIntegerContainer());
    Colour top = on ? Colour(0xff426979) : Colour(0xff45474e),
           bottom = on ? Colour(0xff203c42) : Colour(0xff202126);
    if (down)
      std::swap(top, bottom);
    if (hover)
      top = top.brighter(.15f);
    g.setGradientFill(
        ColourGradient(top, 0, r.getY(), bottom, 0, r.getBottom(), false));
    g.fillRoundedRectangle(r, 8);
    g.setColour(Colours::black.withAlpha(.8f));
    g.drawRoundedRectangle(r, 8, 1);
    g.setColour(
        (on ? Colour(0xff8ae5d9) : Colours::white).withAlpha(on ? .5f : .2f));
    g.drawRoundedRectangle(r.reduced(1), 7, 1);
  }
  Font getComboBoxFont(ComboBox &) override { return Font(FontOptions(11.f)); }
  void positionComboBoxText(ComboBox &box, Label &label) override {
    label.setBounds(3, 1, box.getWidth() - 18, box.getHeight() - 2);
    label.setFont(getComboBoxFont(box));
  }
  void drawComboBox(Graphics &g, int width, int height, bool down, int, int,
                    int, int, ComboBox &) override {
    auto r = Rectangle<float>(1, 1, (float)width - 2, (float)height - 2);
    g.setGradientFill(ColourGradient(
        Colour(0xff080a0e), 0, 0, Colour(0xff3b3e46), 0, (float)height, false));
    g.fillRoundedRectangle(r, 7);
    g.setColour(Colour(0xff1b1e25));
    g.fillRoundedRectangle(r.reduced(2), 5);
    g.setColour(Colours::white.withAlpha(down ? .25f : .11f));
    g.drawRoundedRectangle(r, 7, 1);
    Path arrow;
    arrow.startNewSubPath(width - 14.f, height * .43f);
    arrow.lineTo(width - 10.f, height * .58f);
    arrow.lineTo(width - 6.f, height * .43f);
    g.setColour(Colour(0xffa9bac8));
    g.strokePath(arrow, PathStrokeType(1.5f));
  }
  void drawRotarySlider(Graphics &g, int x, int y, int w, int h, float pos,
                        float start, float end, Slider &) override {
    auto r = Rectangle<float>((float)x, (float)y, (float)w, (float)h)
                 .reduced(w < 100 ? 14.f : 17.f);
    float d = std::min(r.getWidth(), r.getHeight());
    r = r.withSizeKeepingCentre(d, d);
    auto c = r.getCentre();
    float radius = d * .5f;
    // Cast shadow, recessed socket, knurled grip, machined face.
    DropShadow(Colours::black.withAlpha(.9f), 18, {3, 10}).drawForPath(g, [&] {
      Path p;
      p.addEllipse(r.expanded(5));
      return p;
    }());
    g.setGradientFill(ColourGradient(Colour(0xff060709), c.x, c.y - radius - 10,
                                     Colour(0xff53565d), c.x, c.y + radius + 10,
                                     false));
    g.fillEllipse(r.expanded(10));
    g.setColour(Colour(0xff101114));
    g.fillEllipse(r.expanded(7));
    for (int i = 0; i < 96; ++i) {
      float a = MathConstants<float>::twoPi * i / 96;
      float light = .16f + .24f * (1 - std::sin(a));
      g.setColour(Colours::white.withAlpha(light));
      g.drawLine(c.x + std::sin(a) * (radius + 2),
                 c.y - std::cos(a) * (radius + 2),
                 c.x + std::sin(a) * (radius + 6),
                 c.y - std::cos(a) * (radius + 6), 1);
    }
    for (int i = 0; i <= 32; ++i) {
      float a = start + (end - start) * i / 32;
      bool lit = i <= pos * 32;
      g.setColour((i < 16 ? Colour(0xffa893f2) : Colour(0xff73e3d4))
                      .withAlpha(lit ? .95f : .18f));
      g.fillEllipse(c.x + std::sin(a) * (radius + 15) - 1.4f,
                    c.y - std::cos(a) * (radius + 15) - 1.4f, 2.8f, 2.8f);
    }
    g.setOpacity(1.f);
    g.drawImage(face, r);
    g.setGradientFill(ColourGradient(Colours::white.withAlpha(.7f), r.getX(),
                                     r.getY(), Colours::black.withAlpha(.8f),
                                     r.getRight(), r.getBottom(), false));
    g.drawEllipse(r, 1.6f);
    float a = start + pos * (end - start);
    Point<float> tip(c.x + std::sin(a) * radius * .83f,
                     c.y - std::cos(a) * radius * .83f),
        base(c.x + std::sin(a) * radius * .4f,
             c.y - std::cos(a) * radius * .4f);
    g.setColour(Colours::white.withAlpha(.3f));
    g.drawLine(base.x + 1, base.y + 1, tip.x + 1, tip.y + 1, 4);
    g.setColour(Colour(0xff111317));
    g.drawLine({base, tip}, 3);
  }
};
class RoundButton final : public TextButton {
public:
  bool recording = false, active = false;
  void paintButton(Graphics &g, bool hover, bool down) override {
    auto r = getLocalBounds().toFloat().reduced(2);
    g.setGradientFill(ColourGradient(
        Colour(0xff5b626b), 0, 0, Colour(0xff10141b), 0, r.getBottom(), false));
    g.fillEllipse(r);
    g.setColour(Colours::black.withAlpha(.8f));
    g.fillEllipse(r.reduced(2));
    g.setColour((recording ? Colour(0xffec5576) : Colour(0xff87d9db))
                    .withAlpha(isEnabled() ? 1.f : .22f));
    auto c = r.getCentre();
    if (active)
      g.fillRoundedRectangle(c.x - 4, c.y - 4, 8, 8, 2);
    else if (recording)
      g.fillEllipse(c.x - 4, c.y - 4, 8, 8);
    else {
      Path p;
      p.addTriangle(c.x - 3, c.y - 5, c.x + 5, c.y, c.x - 3, c.y + 5);
      g.fillPath(p);
    }
    if (hover || down) {
      g.setColour(Colours::white.withAlpha(.18f));
      g.drawEllipse(r, 1);
    }
  }
};
class Wave final : public Component,
                   public FileDragAndDropTarget,
                   public DragAndDropTarget {
public:
  VoltaProcessor &p;
  std::shared_ptr<const Clip> clip;
  bool sourceView = false, hover = false;
  int handle = 0;
  bool dragged = false;
  std::function<void()> open;
  std::function<void(const StringArray &)> dropped;
  std::function<File()> prepared;
  explicit Wave(VoltaProcessor &proc) : p(proc) {
    setMouseCursor(MouseCursor::PointingHandCursor);
  }
  Rectangle<int> audioArea() const {
    return getLocalBounds().reduced(10).withTrimmedTop(25).withTrimmedBottom(
        37);
  }
  void paint(Graphics &g) override {
    auto r = getLocalBounds().toFloat();
    g.setGradientFill(ColourGradient(
        Colour(0xff05070b), 0, 0, Colour(0xff41464f), 0, r.getBottom(), false));
    g.fillRoundedRectangle(r, 13);
    g.setColour(Colour(0xff090d13));
    g.fillRoundedRectangle(r.reduced(3), 10);
    g.setColour(Colour(0xff667383).withAlpha(hover ? .8f : .25f));
    g.drawRoundedRectangle(r.reduced(1), 13, 1);
    for (int y = 6; y < getHeight() - 6; y += 3) {
      g.setColour(Colours::white.withAlpha(.008f));
      g.drawHorizontalLine(y, 6, (float)getWidth() - 6);
    }
    const auto phase = p.phase.load();
    if (phase == VoltaProcessor::Phase::Loading ||
        phase == VoltaProcessor::Phase::Rendering) {
      g.setColour(Colour(0xff2e3543));
      g.fillRect(12, 29, getWidth() - 24, 2);
      float travel = (Time::getMillisecondCounter() % 1400) / 1400.f;
      g.setColour(Colour(0xff7cdbd2));
      g.fillRect(12 + roundToInt(travel * (getWidth() - 62)), 29, 38, 2);
    }
    if (!clip || clip->audio.empty()) {
      g.setColour(Colour(0xffe1e7ec));
      g.setFont(14);
      g.drawText("Arraste uma voz aqui",
                 getLocalBounds().reduced(10).translated(0, -8),
                 Justification::centred);
      g.setColour(Colour(0xff8592a4));
      g.setFont(10);
      g.drawText("WAV / AIFF  -  clique para abrir", 15, getHeight() / 2 + 12,
                 getWidth() - 30, 20, Justification::centred);
      return;
    }
    auto area = audioArea();
    int lanes = (int)clip->audio.size();
    g.setColour(Colour(0xff566274).withAlpha(.3f));
    g.drawHorizontalLine(area.getCentreY(), (float)area.getX(),
                         (float)area.getRight());
    float maxPeak = 0;
    for (const auto &channel : clip->peaks)
      for (float value : channel)
        maxPeak = std::max(maxPeak, std::abs(value));
    // Display-only scaling. Samples and export gain are never normalised here.
    float displayGain = maxPeak > .00001f ? std::min(8.f, .9f / maxPeak) : 1.f;
    for (int c = 0; c < lanes; ++c) {
      float mid = area.getY() + area.getHeight() * (c + .5f) / lanes,
            amp = area.getHeight() * .43f / lanes;
      for (int x = 0; x < area.getWidth(); ++x) {
        int b = x * 2048 / area.getWidth(),
            e = std::min(2048,
                         std::max(b + 1, (x + 1) * 2048 / area.getWidth()));
        float hi = 0, lo = 0;
        for (int i = b; i < e; ++i) {
          hi = std::max(hi, clip->peaks[c][i * 2 + 1]);
          lo = std::min(lo, clip->peaks[c][i * 2]);
        }
        float f = (float)x / area.getWidth();
        g.setColour(Colour(0xffa58bdf).interpolatedWith(Colour(0xff70dfcf), f));
        g.drawVerticalLine(area.getX() + x, mid - hi * amp * displayGain,
                           mid - lo * amp * displayGain + 1);
      }
    }
    if (sourceView) {
      float b = area.getX() + p.param("start") * area.getWidth(),
            e = area.getX() + p.param("end") * area.getWidth();
      g.setColour(Colour(0xffa78bdc).withAlpha(.14f));
      g.fillRect(b, (float)area.getY(), e - b, (float)area.getHeight());
      for (float x : {b, e}) {
        g.setColour(Colour(0xffc7cfd9));
        g.drawVerticalLine((int)x, (float)area.getY(), (float)area.getBottom());
        g.fillRoundedRectangle(x - 3, area.getCentreY() - 13.f, 6, 26, 2);
      }
    } else {
      g.setColour(Colour(0xff72daca).withAlpha(.6f));
      g.drawVerticalLine(area.getRight() - 1, (float)area.getY(),
                         (float)area.getBottom());
    }
    if (p.playing) {
      g.setColour(Colours::white.withAlpha(.9f));
      g.drawVerticalLine(area.getX() +
                             (int)(p.position.load() * area.getWidth()),
                         (float)area.getY(), (float)area.getBottom());
    }
    g.setColour(Colour(0xff8d9aaa));
    g.setFont(9);
    g.drawFittedText(clip->name, 12, getHeight() - 47, getWidth() - 24, 12,
                     Justification::centredLeft, 1);
  }
  void mouseDown(const MouseEvent &e) override {
    handle = 0;
    dragged = false;
    if (!clip || !sourceView)
      return;
    auto a = audioArea();
    float b = a.getX() + p.param("start") * a.getWidth(),
          end = a.getX() + p.param("end") * a.getWidth();
    if (std::abs(e.position.x - b) < 10)
      handle = 1;
    else if (std::abs(e.position.x - end) < 10)
      handle = 2;
  }
  void mouseDrag(const MouseEvent &e) override {
    if (e.getDistanceFromDragStart() < 4)
      return;
    if (handle) {
      dragged = true;
      auto a = audioArea();
      float v = jlimit(0.f, 1.f, (e.position.x - a.getX()) / a.getWidth());
      if (handle == 1)
        p.setParam("start", std::min(v, p.param("end") - .0001f));
      else
        p.setParam("end", std::max(v, p.param("start") + .0001f));
      p.stop();
      repaint();
      return;
    }
    if (dragged)
      return;
    dragged = true;
    if (sourceView) {
      p.setStatus(p.resultIsCurrent()
                      ? "Desative Vocal para arrastar o resultado"
                      : "Resultado sendo atualizado - aguarde");
      return;
    }
    auto file = prepared ? prepared() : File{};
    if (file.existsAsFile())
      DragAndDropContainer::performExternalDragDropOfFiles(
          {file.getFullPathName()}, false, this);
    else
      p.setStatus("Aguarde o WAV atual ficar pronto para arrastar");
  }
  void mouseUp(const MouseEvent &) override {
    if (!dragged && !handle && open)
      open();
    handle = 0;
  }
  bool isInterestedInFileDrag(const StringArray &files) override {
    return !files.isEmpty();
  }
  void fileDragEnter(const StringArray &, int, int) override {
    hover = true;
    repaint();
  }
  void fileDragExit(const StringArray &) override {
    hover = false;
    repaint();
  }
  void filesDropped(const StringArray &files, int, int) override {
    hover = false;
    if (dropped)
      dropped(files);
    repaint();
  }
  StringArray descriptions(const SourceDetails &d) const {
    StringArray files;
    if (auto *a = d.description.getArray()) {
      for (auto &v : *a)
        files.add(v.toString());
    } else
      files.add(d.description.toString());
    return files;
  }
  bool isInterestedInDragSource(const SourceDetails &d) override {
    for (auto f : descriptions(d))
      if (fileFromDrop(f).hasFileExtension("wav;aif;aiff"))
        return true;
    return false;
  }
  void itemDropped(const SourceDetails &d) override {
    if (dropped)
      dropped(descriptions(d));
  }
};
class Meter final : public Component, public SettableTooltipClient {
public:
  VoltaProcessor &p;
  float in = -100, out = -100;
  explicit Meter(VoltaProcessor &processor) : p(processor) {}
  void update(bool audioActive) {
    auto db = [](float v) { return Decibels::gainToDecibels(v, -100.f); };
    in = std::max(audioActive ? db(p.inputPeak.load()) : -100.f, in - 2.8f);
    out = std::max(audioActive ? db(p.outputPeak.load()) : -100.f, out - 2.8f);
    repaint();
  }
  void paint(Graphics &g) override {
    for (int lane = 0; lane < 2; ++lane) {
      float value = lane ? out : in;
      int y = lane * 13;
      g.setFont(9);
      g.setColour(Colour(0xff8f9cae));
      g.drawText(lane ? "out" : "in", 0, y, 20, 12, Justification::centredLeft);
      for (int i = 0; i < 49; ++i) {
        float threshold = -60 + i * 60.f / 49;
        g.setColour(
            value > threshold
                ? (i > 45 ? Colour(0xffea6d88)
                          : Colour(0xff938cc6)
                                .interpolatedWith(Colour(0xff6ce2d0), i / 49.f))
                : Colour(0xff363941));
        g.fillRect(23 + i * 5, y + 4, 3, 4);
      }
      bool clipped = lane ? p.outputClipped.load() : p.inputClipped.load();
      g.setColour(clipped ? Colour(0xfff47a8e) : Colour(0xff8f9cae));
      g.drawText(value <= -99 ? "-inf" : String(value, 1), 274, y, 44, 12,
                 Justification::centredRight);
      if (clipped)
        g.fillEllipse(322.f, y + 4.f, 4, 4);
    }
  }
  void mouseDown(const MouseEvent &) override {
    p.inputClipped = false;
    p.outputClipped = false;
  }
};
class Options final : public Component {
public:
  void paint(Graphics &g) override {
    g.setColour(Colour(0xff151a22));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 12);
    g.setColour(Colour(0xff495160));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(1), 12, 1);
    g.setFont(10);
    g.setColour(Colour(0xffb3becd));
    for (auto item : std::initializer_list<std::pair<const char *, Point<int>>>{
             {"BPM manual", {8, 30}},
             {"Compasso", {174, 30}},
             {"Feedback", {8, 86}},
             {"Delay", {174, 86}},
             {"Volume previa", {8, 142}},
             {"Tremolo rate", {174, 142}}})
      g.drawText(item.first, item.second.x, item.second.y, 140, 13,
                 Justification::centredLeft);
  }
};
class Content final : public Component,
                      public FileDragAndDropTarget,
                      private Timer {
public:
  VoltaProcessor &p;
  Metal metal;
  Wave wave;
  Meter meters;
  Options options;
  RoundButton recordButton, playButton;
  TextButton prev{"<"}, next{">"}, line{"Line"}, swell{"Swell"}, vocal{"Vocal"},
      reverse{"<< Reverse"}, throwButton{"~ Throw"}, gear{"..."},
      exportButton{"Salvar WAV"}, authority{"s"};
  ComboBox presets, scale, division, denom, tremRate, rate, bits;
  std::array<TextButton, 4> tabs;
  Slider space, length, small, bpm, num, feedback, volume;
  ToggleButton sync{"Sync host"}, musical{"Usar Bars"}, tail{"Cauda Reverse"},
      original{"Fonte + efeito"}, normalise{"Normalizar"},
      protect{"Proteger clipping"}, bypass{"Bypass"};
  Label info, status;
  std::function<void(double)> resizeRequested;
  String lastStatus;
  int noticeTicks = 0;
  bool sourceView = false, optionsOpen = false, chooserOpen = false;
  int tabIndex = 0, presetId = -1;
  uint64_t lastCallbacks = 0;
  int idleTicks = 0;
  std::unique_ptr<FileChooser> chooser;
  std::vector<std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>>
      sliderAttachments;
  std::vector<std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>>
      comboAttachments;
  std::vector<std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>>
      buttonAttachments;
  std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
      smallAttachment;
  Image logo;
  TooltipWindow tooltip{this, 650};
  explicit Content(VoltaProcessor &processor)
      : p(processor), wave(p), meters(p) {
    setLookAndFeel(&metal);
    setWantsKeyboardFocus(true);
    setSize(360, 640);
#if WONKA_HAS_LOGO
    int logoBytes = 0;
    auto *logoData = BinaryData::getNamedResource(
        BinaryData::namedResourceList[0], logoBytes);
    logo = ImageCache::getFromMemory(logoData, logoBytes);
#endif
    for (auto *c : std::initializer_list<Component *>{
             &wave,        &meters, &recordButton, &playButton, &prev,
             &next,        &line,   &swell,        &vocal,      &reverse,
             &throwButton, &gear,   &presets,      &scale,      &space,
             &length,      &small,  &info,         &status,     &authority,
             &bypass,      &options})
      addAndMakeVisible(c);
    for (auto *c : std::initializer_list<Component *>{
             &division, &denom, &tremRate, &rate, &bits, &bpm, &num, &feedback,
             &volume, &sync, &musical, &tail, &original, &normalise, &protect,
             &exportButton})
      options.addAndMakeVisible(c);
    auto slider = [&](Slider &s, const char *id, bool rotary) {
      s.setSliderStyle(rotary ? Slider::RotaryHorizontalVerticalDrag
                              : Slider::LinearHorizontal);
      s.setTextBoxStyle(Slider::TextBoxBelow, false, 68, 17);
      sliderAttachments.push_back(
          std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
              p.state, id, s));
      s.textFromValueFunction = [](double v) { return String(v, 2); };
      s.updateText();
    };
    slider(space, "space", true);
    slider(length, "length", true);
    slider(bpm, "bpm", false);
    slider(num, "numerator", false);
    slider(feedback, "feedback", false);
    slider(volume, "volume", false);
    space.textFromValueFunction = [](double v) {
      return String(.25 + 5.75 * v, 2) + " s";
    };
    space.updateText();
    length.textFromValueFunction = [](double v) { return String(v, 2) + " s"; };
    length.updateText();
    num.textFromValueFunction = [](double v) {
      return String((int)std::round(v));
    };
    num.updateText();
    volume.textFromValueFunction = [](double v) {
      return v > 0 ? String(Decibels::gainToDecibels(v), 1) + " dB" : "-inf dB";
    };
    volume.updateText();
    auto combo = [&](ComboBox &c, const char *id, StringArray values) {
      c.addItemList(values, 1);
      comboAttachments.push_back(
          std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(
              p.state, id, c));
    };
    combo(division, "division",
          {"1/4", "1/8", "1/8 dotted", "1/8 triplet", "1/16", "1/16 dotted",
           "1/16 triplet", "1/2"});
    combo(denom, "denominator", {"2", "4", "8", "16"});
    combo(tremRate, "tremrate", {"1/4", "1/8", "1/16", "4 Hz free"});
    auto toggle = [&](ToggleButton &b, const char *id) {
      buttonAttachments.push_back(
          std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(
              p.state, id, b));
    };
    toggle(sync, "sync");
    toggle(musical, "musical");
    toggle(tail, "tail");
    toggle(original, "original");
    toggle(normalise, "normalise");
    toggle(protect, "protect");
    toggle(bypass, "bypass");
    for (const auto &preset : volta::presets())
      presets.addItem(preset.name, presets.getNumItems() + 1);
    presets.onChange = [this] {
      if (presets.getSelectedId() > 0)
        p.preset(presets.getSelectedItemIndex());
    };
    prev.onClick = [this] { navigate(-1); };
    next.onClick = [this] { navigate(1); };
    scale.addItemList({"70%", "85%", "100%", "125%"}, 1);
    scale.setSelectedId(3, dontSendNotification);
    scale.onChange = [this] {
      const double values[] = {.7, .85, 1., 1.25};
      if (resizeRequested)
        resizeRequested(values[std::max(0, scale.getSelectedItemIndex())]);
    };
    reverse.onClick = [this] {
      p.setParam("mode", 0);
      sourceView = false;
    };
    throwButton.onClick = [this] {
      p.setParam("mode", 1);
      sourceView = false;
    };
    line.onClick = [this] {
      p.setParam("variant", 0);
      sourceView = false;
    };
    swell.onClick = [this] {
      p.setParam("variant", 1);
      sourceView = false;
    };
    vocal.onClick = [this] {
      sourceView = !sourceView;
      p.stop();
      p.state.state.setProperty("sourceView", sourceView, nullptr);
      refresh();
    };
    const char *names[] = {"Tone", "Shape", "Tremolo", "Bars"};
    for (int i = 0; i < 4; ++i) {
      addAndMakeVisible(tabs[i]);
      tabs[i].setButtonText(names[i]);
      tabs[i].onClick = [this, i] { attachSmall(i); };
    }
    tabIndex = jlimit(0, 3, (int)p.state.state.getProperty("tab", 1) - 1);
    attachSmall(tabIndex);
    rate.addItemList({"44.1 kHz", "48 kHz", "88.2 kHz", "96 kHz"}, 1);
    rate.setSelectedId(
        jlimit(1, 4, (int)p.state.state.getProperty("exportRate", 2)),
        dontSendNotification);
    bits.addItemList({"24-bit PCM", "32-bit float"}, 1);
    bits.setSelectedId(
        jlimit(1, 2, (int)p.state.state.getProperty("exportBits", 1)),
        dontSendNotification);
    rate.onChange = [this] {
      p.state.state.setProperty("exportRate", rate.getSelectedId(), nullptr);
    };
    bits.onChange = [this] {
      p.state.state.setProperty("exportBits", bits.getSelectedId(), nullptr);
    };
    authority.onClick = [this] {
      p.setParam("musical", p.param("musical") > .5f ? 0.f : 1.f);
    };
    gear.onClick = [this] {
      optionsOpen = !optionsOpen;
      refresh();
      resized();
      repaint();
    };
    recordButton.recording = true;
    recordButton.onClick = [this] {
      if (p.recordState.load() == 0)
        p.record();
      else
        p.stopRecord();
    };
    playButton.onClick = [this] { togglePlay(); };
    wave.open = [this] { choose(false); };
    wave.dropped = [this](const StringArray &files) { acceptFiles(files); };
    wave.prepared = [this] {
      return p.preparedDragFile(exportBits(), exportRate());
    };
    exportButton.onClick = [this] { choose(true); };
    info.setFont(FontOptions(10));
    info.setColour(Label::textColourId, Colour(0xffacb8c9));
    info.setInterceptsMouseClicks(false, false);
    status.setFont(FontOptions(9));
    status.setColour(Label::textColourId, Colour(0xff8b99ad));
    status.setInterceptsMouseClicks(false, false);
    sourceView = (bool)p.state.state.getProperty("sourceView", false);
    setTooltips();
    resized();
    refresh();
    startTimerHz(25);
  }
  ~Content() override {
    stopTimer();
    setLookAndFeel(nullptr);
  }
  void setTooltips() {
    wave.setTitle("Importar voz / arrastar resultado");
    vocal.setTooltip("Comparar e selecionar a fonte original (interpretacao "
                     "WONKA do botao Vocal)");
    space.setTooltip("Space: tamanho, difusao e T60 do ambiente");
    length.setTooltip(
        "Length em segundos; s/bars escolhe a autoridade da duracao");
    authority.setTooltip("Escolher segundos (Length) ou compassos (Bars)");
    recordButton.setTooltip(
        "Gravar entrada da DAW. Limite 60 s; o host precisa processar audio.");
    gear.setTooltip("Opcoes de sync, exportacao e ganho");
    presets.setTooltip("17 presets proprios; * indica alteracoes");
    playButton.setTooltip(
        "Ouvir/parar. Espaco funciona quando o editor recebe a tecla.");
    meters.setTooltip(
        "Entrada/saida dBFS. Clique para limpar indicadores de clipping.");
  }
  void attachSmall(int index) {
    smallAttachment.reset();
    tabIndex = jlimit(0, 3, index);
    p.state.state.setProperty("tab", tabIndex + 1, nullptr);
    const char *ids[] = {"tone", "shape", "tremolo", "bars"};
    small.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    small.setTextBoxStyle(Slider::TextBoxBelow, false, 88, 17);
    smallAttachment =
        std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
            p.state, ids[tabIndex], small);
    small.textFromValueFunction = [this](double value) {
      if (tabIndex == 0) {
        double hz = volta::toneFrequency(value, p.hostRate.load());
        return hz >= 1000 ? String(hz / 1000, 1) + " kHz"
                          : String(roundToInt(hz)) + " Hz";
      }
      if (tabIndex == 1)
        return String(.2 + 3.8 * value, 2);
      if (tabIndex == 2)
        return String(roundToInt(value * 100)) + " %";
      return String(value, 2) + " bars";
    };
    small.updateText();
    small.setTooltip(
        tabIndex == 3 ? "Bars: duracao musical. Ative o botao bars sob Length."
                      : "Editar apenas a aba selecionada");
    for (int i = 0; i < 4; ++i)
      tabs[i].setToggleState(i == tabIndex, dontSendNotification);
  }
  void navigate(int delta) {
    int current =
        jlimit(0, 16, (int)p.state.state.getProperty("preset", 1) - 1);
    p.preset((current + delta + 17) % 17);
  }
  int exportBits() const { return bits.getSelectedId() == 2 ? 32 : 24; }
  double exportRate() const {
    const double values[] = {44100, 48000, 88200, 96000};
    return values[jlimit(0, 3, rate.getSelectedItemIndex())];
  }
  void togglePlay() {
    if (p.playing)
      p.stop();
    else
      p.preview(sourceView);
  }
  bool keyPressed(const KeyPress &key) override {
    if (key == KeyPress::spaceKey) {
      togglePlay();
      return true;
    }
    return false;
  }
  void choose(bool exporting) {
    if (chooserOpen)
      return;
    if (!exporting && p.recordState.load() != 0) {
      p.setStatus("Pare a gravacao antes de importar");
      return;
    }
    if (exporting && !p.resultIsCurrent()) {
      p.setStatus("Aguarde o resultado atual antes de exportar");
      return;
    }
    const auto home = File::getSpecialLocation(File::userDocumentsDirectory);
    const auto target = home.getChildFile(
        "WONKA_" + String(p.settings().mode ? "Throw" : "Reverse") + ".wav");
    chooser = std::make_unique<FileChooser>(
        exporting ? "Exportar WONKA" : "Abrir voz WAV / AIFF",
        exporting ? target : home, exporting ? "*.wav" : "*.wav;*.aif;*.aiff",
        true, false, this);
    chooserOpen = true;
    Component::SafePointer<Content> safe(this);
    chooser->launchAsync(
        exporting ? (FileBrowserComponent::saveMode |
                     FileBrowserComponent::canSelectFiles |
                     FileBrowserComponent::warnAboutOverwriting)
                  : (FileBrowserComponent::openMode |
                     FileBrowserComponent::canSelectFiles),
        [safe, exporting](const FileChooser &fc) {
          if (!safe)
            return;
          safe->chooserOpen = false;
          auto file = fc.getResult();
          if (file == File{})
            return;
          if (exporting)
            safe->p.exportFile(file.withFileExtension("wav"),
                               safe->exportBits(), safe->exportRate());
          else
            safe->importFile(file);
        });
  }
  void importFile(File file) {
    sourceView = false;
    p.state.state.setProperty("sourceView", false, nullptr);
    p.importFile(file);
    refresh();
  }
  void acceptFiles(const StringArray &files) {
    for (const auto &item : files) {
      auto file = fileFromDrop(item);
      if (file.hasFileExtension("wav;aif;aiff")) {
        importFile(file);
        return;
      }
    }
    p.rejectImport("Arraste WAV/AIFF do Finder; a regiao da DAW pode nao "
                   "fornecer arquivo compativel");
  }
  bool isInterestedInFileDrag(const StringArray &files) override {
    return !files.isEmpty();
  }
  void filesDropped(const StringArray &files, int, int) override {
    acceptFiles(files);
  }
  void refresh() {
    const auto s = p.settings();
    auto input = p.source(), output = p.result();
    const bool ready = p.resultIsCurrent();
    bool hasSource = input && !input->audio.empty();
    wave.sourceView = sourceView || !ready;
    wave.clip = wave.sourceView ? input : output;
    line.setToggleState(s.variant == 0, dontSendNotification);
    swell.setToggleState(s.variant == 1, dontSendNotification);
    vocal.setToggleState(sourceView, dontSendNotification);
    reverse.setToggleState(s.mode == 0, dontSendNotification);
    throwButton.setToggleState(s.mode == 1, dontSendNotification);
    line.setVisible(s.mode == 0);
    swell.setVisible(s.mode == 0);
    vocal.setEnabled(hasSource);
    playButton.setEnabled(hasSource &&
                          p.phase.load() != VoltaProcessor::Phase::Loading &&
                          p.recordState.load() == 0 && (sourceView || ready));
    playButton.active = p.playing;
    recordButton.active = p.recordState.load() != 0;
    authority.setButtonText(s.musical ? "bars" : "s");
    authority.setToggleState(s.musical, dontSendNotification);
    length.setEnabled(!s.musical);
    tail.setEnabled(s.mode == 0);
    original.setEnabled(s.mode == 1);
    feedback.setEnabled(s.mode == 1);
    division.setEnabled(s.mode == 1);
    exportButton.setEnabled(ready && p.recordState.load() == 0);
    int selected = jlimit(1, 17, (int)p.state.state.getProperty("preset", 1));
    if (presets.getSelectedId() != selected)
      presets.setSelectedId(selected, dontSendNotification);
    if (presetId > 0 && presetId != selected)
      presets.changeItemText(presetId, volta::presets()[presetId - 1].name);
    presetId = selected;
    presets.changeItemText(selected, String(p.presetModified() ? "* " : "") +
                                         volta::presets()[selected - 1].name);
    presets.setSelectedId(selected, dontSendNotification);
    String label;
    if (hasSource) {
      auto shown = wave.clip;
      label = (wave.sourceView ? "Vocal"
               : s.mode        ? "Throw"
               : s.variant     ? "Swell"
                               : "Line");
      if (shown)
        label +=
            "  /  " + String(shown->audio[0].size() / shown->rate, 2) + " s";
    } else
      label = "Sem audio";
    info.setText(label, dontSendNotification);
    auto callbacks = p.audioCallbacks.load();
    if (callbacks == lastCallbacks)
      ++idleTicks;
    else
      idleTicks = 0;
    lastCallbacks = callbacks;
    meters.update(idleTicks < 5);
    String message = p.status();
    if (message != lastStatus) {
      lastStatus = message;
      noticeTicks = 50;
    }
    const bool showNotice = noticeTicks > 0 && !message.startsWith("Pronto");
    if (noticeTicks > 0)
      --noticeTicks;
    if (p.recordState.load() != 0)
      message = "Gravando " + String(p.recordingSeconds(), 1) + " / 60 s";
    if (p.playing) {
      message =
          idleTicks > 20
              ? "Host sem audio: ative transporte / desative Smart disable"
              : "Reproduzindo " + String(sourceView ? "Vocal" : "resultado");
    } else if (ready && p.phase.load() == VoltaProcessor::Phase::Ready &&
               !p.recordingAtLimit.load() && !showNotice) {
      double total = output->audio[0].size() / output->rate;
      double bars = total * s.bpm / 60 / (s.numerator * 4. / s.denominator);
      message = String(total, 2) + " s / " + String(bars, 2) + " bars - " +
                String(s.bpm, 1) + " BPM";
      if (s.musical &&
          s.bars * s.numerator * 4.0 / s.denominator * 60 / s.bpm > 30)
        message = "Limite 30 s - " + message;
    }
    status.setText(message, dontSendNotification);
    status.setTooltip(p.status());
    status.setColour(Label::textColourId,
                     p.phase.load() == VoltaProcessor::Phase::Error
                         ? Colour(0xfff18ba3)
                         : Colour(0xff9ba9bc));
    if (ready && !p.isBusy())
      p.prepareDragFile(exportBits(), exportRate());
    wave.repaint();
  }
  void timerCallback() override { refresh(); }
  void paint(Graphics &g) override {
    g.fillAll(Colour(0xff0b0e13));
    auto panel = getLocalBounds().toFloat().reduced(2);
    g.setGradientFill(ColourGradient(Colour(0xff54565e), 0, 0,
                                     Colour(0xff1a1c23), 350, 640, false));
    g.fillRoundedRectangle(panel, 16);
    g.setColour(Colour(0xff292c34));
    g.fillRoundedRectangle(panel.reduced(2), 14);
    for (int y = 5; y < 636; y += 2) {
      g.setColour(Colours::white.withAlpha(y % 4 ? .016f : .006f));
      g.drawHorizontalLine(y, 6, 354);
    }
    g.setGradientFill(ColourGradient(Colour(0xffb3b5b9), 0, 3,
                                     Colour(0xff737982), 360, 38, false));
    g.fillRoundedRectangle(5, 4, 350, 34, 11);
    g.setOpacity(1.f);
    if (logo.isValid())
      g.drawImageWithin(logo, 12, 9, 58, 22,
                        RectanglePlacement::centred |
                            RectanglePlacement::onlyReduceInSize);
    else {
      g.setColour(Colour(0xff10141a));
      g.setFont(FontOptions(10, Font::bold));
      g.drawText("WONKA", 12, 9, 58, 22, Justification::centred);
    }
    g.setColour(Colours::black.withAlpha(.7f));
    g.fillRoundedRectangle(9, 40, 342, 33, 9);
    if (!optionsOpen) {
      g.setGradientFill(ColourGradient(Colour(0xff363941), 0, 355,
                                       Colour(0xff23262e), 350, 619, false));
      g.fillRoundedRectangle(8, 354, 344, 265, 10);
      g.setColour(Colours::white.withAlpha(.1f));
      g.drawRoundedRectangle(8, 354, 344, 265, 10, 1);
      g.setColour(Colour(0xffb3bfcc));
      g.setFont(12);
      g.drawText("Space", 42, 495, 112, 20, Justification::centred);
      g.drawText("Length", 202, 495, 112, 20, Justification::centred);
      for (int side = 0; side < 2; ++side)
        for (int row = 0; row < 4; ++row)
          for (int col = 0; col < 5; ++col) {
            float x = (side ? 244.f : 55.f) + col * 10, y = 566.f + row * 8;
            g.setColour(Colour(0xff52565f));
            g.fillRoundedRectangle(x, y + 1, 7, 2, 1);
            g.setColour(Colour(0xff090c11));
            g.fillRoundedRectangle(x, y, 7, 2, 1);
          }
    }
    g.setColour(Colour(0xffd2d7dd));
    g.setFont(FontOptions(12, Font::bold));
    g.drawText("W O N K A", 105, 621, 150, 15, Justification::centred);
    g.setFont(8);
    g.setColour(Colour(0xff94a0b2));
    g.drawText(String("v") + VoltaProcessor::version, 262, 622, 40, 14,
               Justification::centredRight);
  }
  void resized() override {
    prev.setBounds(76, 10, 20, 22);
    presets.setBounds(98, 10, 126, 22);
    next.setBounds(226, 10, 20, 22);
    scale.setBounds(251, 10, 50, 22);
    recordButton.setBounds(317, 8, 26, 26);
    reverse.setBounds(12, 43, 167, 27);
    throwButton.setBounds(181, 43, 167, 27);
    wave.setBounds(12, 80, 336, 240);
    info.setBounds(19, 84, 158, 23);
    line.setBounds(178, 87, 45, 20);
    swell.setBounds(226, 87, 49, 20);
    vocal.setBounds(282, 87, 57, 20);
    playButton.setBounds(22, 277, 27, 27);
    status.setBounds(55, 278, 283, 27);
    meters.setBounds(16, 326, 328, 26);
    space.setBounds(32, 358, 132, 137);
    length.setBounds(196, 358, 132, 137);
    authority.setBounds(307, 498, 32, 17);
    for (int i = 0; i < 4; ++i)
      tabs[i].setBounds(47 + i * 67, 522, 66, 21);
    small.setBounds(136, 548, 88, 76);
    bypass.setBounds(11, 622, 82, 16);
    gear.setBounds(310, 621, 24, 17);
    options.setBounds(12, 355, 336, 264);
    options.setVisible(optionsOpen);
    for (auto *c : std::initializer_list<Component *>{&space, &length, &small,
                                                      &authority})
      c->setVisible(!optionsOpen);
    for (auto &tab : tabs)
      tab.setVisible(!optionsOpen);
    sync.setBounds(8, 7, 150, 22);
    musical.setBounds(174, 7, 150, 22);
    bpm.setBounds(8, 43, 146, 36);
    num.setBounds(174, 43, 70, 36);
    denom.setBounds(253, 47, 70, 22);
    feedback.setBounds(8, 99, 146, 36);
    division.setBounds(174, 104, 150, 22);
    volume.setBounds(8, 155, 146, 36);
    tremRate.setBounds(174, 160, 150, 22);
    tail.setBounds(8, 191, 153, 21);
    original.setBounds(174, 191, 153, 21);
    normalise.setBounds(8, 213, 153, 21);
    protect.setBounds(174, 213, 153, 21);
    rate.setBounds(8, 238, 86, 22);
    bits.setBounds(99, 238, 116, 22);
    exportButton.setBounds(220, 238, 108, 22);
  }
};
class Editor final : public AudioProcessorEditor {
  Content content;
  bool initialising = true;

public:
  explicit Editor(VoltaProcessor &p) : AudioProcessorEditor(p), content(p) {
    setWantsKeyboardFocus(true);
    setResizable(true, true);
    setResizeLimits(252, 448, 540, 960);
    getConstrainer()->setFixedAspectRatio(360. / 640.);
    addAndMakeVisible(content);
    content.resizeRequested = [this](double scale) {
      setSize(roundToInt(360 * scale), roundToInt(640 * scale));
    };
    int width = 360;
    if ((int)p.state.state.getProperty("uiSchema", 0) == 2)
      width = jlimit(252, 540, (int)p.state.state.getProperty("uiWidth", 360));
    setSize(width, roundToInt(width * 640. / 360));
    initialising = false;
    resized();
  }
  void resized() override {
    double scale = std::min(getWidth() / 360., getHeight() / 640.);
    content.setTransform(AffineTransform::scale((float)scale));
    content.setTopLeftPosition(0, 0);
    if (!initialising) {
      content.p.state.state.setProperty("uiSchema", 2, nullptr);
      content.p.state.state.setProperty("uiWidth", getWidth(), nullptr);
    }
  }
  bool keyPressed(const KeyPress &key) override {
    return content.keyPressed(key);
  }
};
} // namespace
AudioProcessorEditor *VoltaProcessor::createEditor() {
  return new Editor(*this);
}
