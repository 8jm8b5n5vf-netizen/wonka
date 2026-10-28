#include "Plugin.h"
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
    arrow.startNewSubPath(width - 21.f, height * .43f);
    arrow.lineTo(width - 16.f, height * .58f);
    arrow.lineTo(width - 11.f, height * .43f);
    g.setColour(Colour(0xffa9bac8));
    g.strokePath(arrow, PathStrokeType(1.5f));
  }
  void drawRotarySlider(Graphics &g, int x, int y, int w, int h, float pos,
                        float start, float end, Slider &) override {
    auto r =
        Rectangle<float>((float)x, (float)y, (float)w, (float)h).reduced(18);
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
class Wave final : public Component {
public:
  VoltaProcessor &p;
  bool processed = false;
  std::shared_ptr<const Clip> clip;
  std::function<void()> changed;
  int handle = 0;
  explicit Wave(VoltaProcessor &proc) : p(proc) {}
  void paint(Graphics &g) override {
    auto r = getLocalBounds().toFloat();
    g.setGradientFill(ColourGradient(
        Colour(0xff030507), 0, 0, Colour(0xff22262e), 0, r.getBottom(), false));
    g.fillRoundedRectangle(r, 16);
    g.setColour(Colour(0xff08090c));
    g.fillRoundedRectangle(r.reduced(5), 12);
    g.setColour(Colours::white.withAlpha(.14f));
    g.drawRoundedRectangle(r.reduced(1), 16, 1);
    g.setColour(Colours::black.withAlpha(.85f));
    g.drawRoundedRectangle(r.reduced(4), 13, 3);
    for (int y = 10; y < getHeight() - 10; y += 3) {
      g.setColour(Colours::white.withAlpha(.012f));
      g.drawHorizontalLine(y, 10.f, getWidth() - 10.f);
    }
    if (!clip || clip->audio.empty()) {
      g.setColour(Colour(0xffa8b4c7));
      g.setFont(20);
      g.drawText("ARRASTE UMA VOZ AQUI", getLocalBounds(),
                 Justification::centred);
      return;
    }
    auto area = getLocalBounds().reduced(12, 28);
    int n = (int)clip->audio[0].size();
    int lanes = (int)clip->audio.size();
    for (int c = 0; c < lanes; ++c) {
      float mid = area.getY() + area.getHeight() * (c + .5f) / lanes,
            amp = area.getHeight() * .44f / lanes;
      for (int x = 0; x < area.getWidth(); ++x) {
        int b = x * 2048 / area.getWidth(),
            e = std::min(2048,
                         std::max(b + 1, (x + 1) * 2048 / area.getWidth()));
        float hi = 0, lo = 0;
        for (int i = b; i < e; ++i) {
          hi = std::max(hi, clip->peaks[c][i * 2 + 1]);
          lo = std::min(lo, clip->peaks[c][i * 2]);
        }
        g.setColour(processed ? Colour(0xffaa8cf4) : Colour(0xff62ddd0));
        g.drawVerticalLine(area.getX() + x, mid - hi * amp, mid - lo * amp + 1);
      }
    }
    if (!processed) {
      float b = area.getX() + p.param("start") * area.getWidth(),
            e = area.getX() + p.param("end") * area.getWidth();
      g.setColour(Colour(0xffaa8cf4).withAlpha(.15f));
      g.fillRect(b, (float)area.getY(), e - b, (float)area.getHeight());
      g.setColour(Colour(0xffe3e7ef));
      g.drawVerticalLine((int)b, (float)area.getY(), (float)area.getBottom());
      g.drawVerticalLine((int)e, (float)area.getY(), (float)area.getBottom());
      g.fillRoundedRectangle(b - 4, area.getCentreY() - 18.f, 8, 36, 3);
      g.fillRoundedRectangle(e - 4, area.getCentreY() - 18.f, 8, 36, 3);
    }
    if (p.playing) {
      g.setColour(Colours::white);
      g.drawVerticalLine(area.getX() +
                             (int)(p.position.load() * area.getWidth()),
                         (float)area.getY(), (float)area.getBottom());
    }
    g.setColour(Colour(0xffb9c5d7));
    g.setFont(13);
    g.drawText((processed ? "RESULTADO  " : "FONTE  ") + clip->name + "  |  " +
                   String(n / clip->rate, 2) + " s",
               12, 4, getWidth() - 24, 20, Justification::centredLeft);
  }
  void mouseDown(const MouseEvent &e) override {
    if (processed || !clip)
      return;
    float v = jlimit(0.f, 1.f, (e.position.x - 12) / (getWidth() - 24));
    handle =
        std::abs(v - p.param("start")) < std::abs(v - p.param("end")) ? 1 : 2;
    mouseDrag(e);
  }
  void mouseDrag(const MouseEvent &e) override {
    if (!handle || processed)
      return;
    float v = jlimit(0.f, 1.f, (e.position.x - 12) / (getWidth() - 24));
    if (handle == 1)
      p.setParam("start", std::min(v, p.param("end") - .0001f));
    else
      p.setParam("end", std::max(v, p.param("start") + .0001f));
    repaint();
  }
  void mouseUp(const MouseEvent &) override {
    if (handle && changed)
      changed();
    handle = 0;
  }
};
class DragButton : public TextButton {
public:
  DragButton() : TextButton("Arrastar WAV") {}
  File file;
  void mouseDrag(const MouseEvent &e) override {
    TextButton::mouseDrag(e);
    if (e.getDistanceFromDragStart() > 5 && file.existsAsFile())
      DragAndDropContainer::performExternalDragDropOfFiles(
          {file.getFullPathName()}, false, this);
  }
};
class Editor final : public AudioProcessorEditor,
                     public FileDragAndDropTarget,
                     private Timer {
  VoltaProcessor &p;
  Metal metal;
  Wave wave;
  TextButton open{"Abrir voz"}, rec{"Gravar"}, play{"Ouvir / Parar"},
      compare{"Ouvir fonte"}, save{"Exportar WAV"}, suggest{"Sugerir final"};
  DragButton drag;
  ComboBox mode, variant, division, presets, scale, tab, rate, bits, denom,
      tremRate;
  std::array<TextButton, 4> tabButtons;
  std::array<TextButton, 2> modeButtons;
  ToggleButton sync{"BPM do host"}, musical{"Usar Bars"}, tail{"Incluir cauda"},
      original{"Fonte + efeito"}, normalise{"Normalizar"},
      protect{"Proteger clipping"}, bypass{"Bypass"};
  Slider space, length, small, volume, bpm, feedback, num;
  std::vector<std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>>
      sliders;
  std::vector<std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>>
      combos;
  std::vector<std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>>
      buttons;
  std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
      smallAttachment;
  std::unique_ptr<FileChooser> chooser;
  int lastRevision = -1, tabIndex = 0;
  bool sourcePreview = false;
  File dragFile;
  std::shared_ptr<const Clip> dragClip;
  Label status, durationLabel;

public:
  explicit Editor(VoltaProcessor &proc)
      : AudioProcessorEditor(proc), p(proc), wave(proc) {
    sourcePreview = (bool)p.state.state.getProperty("sourceView", false);
    setLookAndFeel(&metal);
    setWantsKeyboardFocus(true);
    setResizable(true, true);
    setResizeLimits(760, 860, 1440, 1632);
    setSize(900, 1020);
    for (Component *c : std::initializer_list<Component *>{
             &wave,     &open,    &rec,    &play,         &compare,   &save,
             &suggest,  &drag,    &mode,   &variant,      &division,  &presets,
             &scale,    &tab,     &rate,   &bits,         &denom,     &tremRate,
             &sync,     &musical, &tail,   &original,     &normalise, &protect,
             &bypass,   &space,   &length, &small,        &volume,    &bpm,
             &feedback, &num,     &status, &durationLabel})
      addAndMakeVisible(c);
    auto slider = [&](Slider &s, const char *id, bool rotary) {
      s.setSliderStyle(rotary ? Slider::RotaryHorizontalVerticalDrag
                              : Slider::LinearHorizontal);
      s.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
      s.setNumDecimalPlacesToDisplay(2);
      sliders.push_back(
          std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
              p.state, id, s));
      s.textFromValueFunction = [](double value) { return String(value, 2); };
      s.updateText();
    };
    slider(space, "space", true);
    slider(length, "length", true);
    slider(volume, "volume", false);
    slider(bpm, "bpm", false);
    slider(feedback, "feedback", false);
    slider(num, "numerator", false);
    auto combo = [&](ComboBox &c, const char *id, StringArray items) {
      c.addItemList(items, 1);
      combos.push_back(
          std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(
              p.state, id, c));
    };
    combo(tremRate, "tremrate", {"1/4", "1/8", "1/16", "4 Hz livre"});
    combo(mode, "mode", {"REVERSE", "THROW"});
    mode.setVisible(false);
    for (int i = 0; i < 2; ++i) {
      addAndMakeVisible(modeButtons[i]);
      modeButtons[i].setButtonText(i == 0 ? "<<   REVERSE" : "~   THROW");
      modeButtons[i].onClick = [this, i] { p.setParam("mode", (float)i); };
    }
    combo(variant, "variant", {"Line", "Swell"});
    combo(division, "division",
          {"1/4", "1/8", "1/8 pontuada", "1/8 tercina", "1/16", "1/16 pontuada",
           "1/16 tercina", "1/2"});
    combo(denom, "denominator", {"2", "4", "8", "16"});
    auto toggle = [&](ToggleButton &b, const char *id) {
      buttons.push_back(
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
    presets.addItemList({"Reverse curto", "Reverse ambiente", "Swell longo",
                         "Throw pontuado", "Ping-pong escuro"},
                        1);
    presets.setTextWhenNothingSelected("Presets WONKA");
    presets.setSelectedId((int)p.state.state.getProperty("preset", 0),
                          dontSendNotification);
    presets.onChange = [this] {
      p.state.state.setProperty("preset", presets.getSelectedId(), nullptr);
      p.preset(presets.getSelectedItemIndex());
    };
    scale.addItemList({"85%", "100%", "125%", "150%"}, 1);
    scale.setSelectedId((int)p.state.state.getProperty("scale", 2),
                        dontSendNotification);
    scale.onChange = [this] {
      p.state.state.setProperty("scale", scale.getSelectedId(), nullptr);
      double v = scale.getSelectedId() == 1   ? .85
                 : scale.getSelectedId() == 3 ? 1.25
                 : scale.getSelectedId() == 4 ? 1.5
                                              : 1;
      setSize((int)(900 * v), (int)(1020 * v));
    };
    const char *tabNames[] = {"Tone", "Shape", "Tremolo", "Bars"};
    for (int i = 0; i < 4; ++i) {
      addAndMakeVisible(tabButtons[i]);
      tabButtons[i].setButtonText(tabNames[i]);
      tabButtons[i].onClick = [this, i] { tab.setSelectedId(i + 1); };
    }
    tab.setVisible(false);
    tab.addItemList({"Tone", "Shape", "Tremolo", "Bars"}, 1);
    tab.setSelectedId((int)p.state.state.getProperty("tab", 1),
                      dontSendNotification);
    tab.onChange = [this] {
      p.state.state.setProperty("tab", tab.getSelectedId(), nullptr);
      attachSmall();
    };
    attachSmall();
    bits.addItemList({"24-bit PCM", "32-bit float"}, 1);
    bits.setSelectedId(1);
    rate.addItemList({"44100 Hz", "48000 Hz", "88200 Hz", "96000 Hz"}, 1);
    rate.setSelectedId((int)p.state.state.getProperty("exportRate", 2));
    bits.setSelectedId((int)p.state.state.getProperty("exportBits", 1));
    rate.onChange = [this] {
      p.state.state.setProperty("exportRate", rate.getSelectedId(), nullptr);
      dragClip.reset();
    };
    bits.onChange = [this] {
      p.state.state.setProperty("exportBits", bits.getSelectedId(), nullptr);
      dragClip.reset();
    };
    open.onClick = [this] { choose(false); };
    save.onClick = [this] { choose(true); };
    rec.onClick = [this] {
      if (p.recordState.load() == 0)
        p.record();
      else
        p.stopRecord();
    };
    play.onClick = [this] { togglePlay(); };
    compare.onClick = [this] {
      sourcePreview = !sourcePreview;
      p.state.state.setProperty("sourceView", sourcePreview, nullptr);
      compare.setButtonText(sourcePreview ? "Ouvir resultado" : "Ouvir fonte");
      p.stop();
      lastRevision = -1;
    };
    suggest.onClick = [this] {
      if (auto c = p.source()) {
        auto region = volta::suggest(c->audio);
        p.setParam("start", (float)region.first);
        p.setParam("end", (float)region.second);
        sourcePreview = true;
        lastRevision = -1;
      }
    };
    wave.changed = [this] { p.stop(); };
    status.setColour(Label::textColourId, Colour(0xffc3ccda));
    durationLabel.setColour(Label::textColourId, Colour(0xff62ddd0));
    if (scale.onChange)
      scale.onChange();
    timerCallback();
    startTimerHz(20);
  }
  ~Editor() override {
    stopTimer();
    setLookAndFeel(nullptr);
  }
  void attachSmall() {
    smallAttachment.reset();
    const char *ids[] = {"tone", "shape", "tremolo", "bars"};
    tabIndex = std::max(0, tab.getSelectedItemIndex());
    for (int i = 0; i < 4; ++i)
      tabButtons[i].setToggleState(i == tabIndex, dontSendNotification);
    small.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    small.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    small.setNumDecimalPlacesToDisplay(2);
    smallAttachment =
        std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
            p.state, ids[tabIndex], small);
    small.textFromValueFunction = [](double value) { return String(value, 2); };
    small.updateText();
  }
  void togglePlay() {
    if (p.playing)
      p.stop();
    else
      p.preview(sourcePreview);
  }
  bool keyPressed(const KeyPress &k) override {
    if (k == KeyPress::spaceKey) {
      togglePlay();
      return true;
    }
    return false;
  }
  double exportRate() const {
    const double rates[] = {44100, 48000, 88200, 96000};
    return rates[std::max(0, rate.getSelectedItemIndex())];
  }
  void choose(bool exporting) {
    auto home = File::getSpecialLocation(File::userDocumentsDirectory);
    auto target = home.getChildFile(
        "WONKA_" + String(p.param("mode") > .5f ? "Throw" : "Reverse") +
        ".wav");
    chooser = std::make_unique<FileChooser>(
        exporting ? "Exportar WONKA" : "Abrir voz WAV / AIFF",
        exporting ? target : home, exporting ? "*.wav" : "*.wav;*.aif;*.aiff");
    Component::SafePointer<Editor> safe(this);
    chooser->launchAsync(
        exporting ? (FileBrowserComponent::saveMode |
                     FileBrowserComponent::canSelectFiles |
                     FileBrowserComponent::warnAboutOverwriting)
                  : (FileBrowserComponent::openMode |
                     FileBrowserComponent::canSelectFiles),
        [safe, exporting](const FileChooser &fc) {
          if (!safe)
            return;
          auto file = fc.getResult();
          if (file == File{})
            return;
          if (exporting)
            safe->p.exportFile(file.withFileExtension("wav"),
                               safe->bits.getSelectedId() == 1 ? 24 : 32,
                               safe->exportRate());
          else
            safe->import(file);
        });
  }
  void import(File f) {
    sourcePreview = true;
    p.setParam("start", 0);
    p.setParam("end", 1);
    p.importFile(f);
  }
  bool isInterestedInFileDrag(const StringArray &files) override {
    for (auto f : files)
      if (File(f).hasFileExtension("wav;aif;aiff"))
        return true;
    return false;
  }
  void filesDropped(const StringArray &files, int, int) override {
    for (auto f : files)
      if (File(f).hasFileExtension("wav;aif;aiff")) {
        import(File(f));
        break;
      }
  }
  void timerCallback() override {
    compare.setButtonText(sourcePreview ? "Ouvir resultado" : "Ouvir fonte");
    status.setText(p.status(), dontSendNotification);
    rec.setButtonText(p.recordState.load() ? "Parar gravacao" : "Gravar");
    auto s = p.settings();
    for (int i = 0; i < 2; ++i)
      modeButtons[i].setToggleState(s.mode == i, dontSendNotification);
    auto out = p.result();
    double actual =
        out && !out->audio.empty() ? out->audio[0].size() / out->rate : 0;
    double beats = volta::duration(s) * s.bpm / 60;
    durationLabel.setText(
        "Length: " + String(volta::duration(s), 2) + " s  /  " +
            String(beats / (s.numerator * 4. / s.denominator), 2) +
            " compassos  |  " + String(s.bpm, 1) + " BPM " +
            String(s.numerator) + "/" + String(s.denominator) + "  " +
            (p.param("sync") > .5f && p.hostTempo ? "host" : "manual") +
            "  |  WAV: " + String(actual, 2) + " s",
        dontSendNotification);
    tremRate.setVisible(tabIndex == 2);
    tail.setEnabled(s.mode == 0);
    original.setEnabled(s.mode == 1);
    length.setEnabled(!s.musical);
    variant.setEnabled(s.mode == 0);
    division.setEnabled(s.mode == 1);
    feedback.setEnabled(s.mode == 1);
    save.setEnabled(out && !p.isBusy());
    play.setEnabled(p.source() != nullptr && !p.isBusy());
    if (lastRevision != p.revision.load()) {
      lastRevision = p.revision.load();
      wave.processed = !sourcePreview;
      wave.clip = sourcePreview ? p.source() : out;
    }
    if (out && out != dragClip && !p.isBusy()) {
      dragClip = out;
      auto folder = File::getSpecialLocation(File::tempDirectory)
                        .getChildFile("WONKA-drag");
      if (folder.createDirectory()) {
        dragFile =
            folder.getChildFile(out->name + "_" + Uuid().toString() + ".wav");
        drag.file = File{};
        p.exportFile(dragFile, bits.getSelectedId() == 1 ? 24 : 32,
                     exportRate());
      }
    }
    if (dragFile.existsAsFile() && !p.isBusy())
      drag.file = dragFile;
    drag.setEnabled(drag.file.existsAsFile() && !p.isBusy());
    wave.repaint();
  }
  void paint(Graphics &g) override {
    g.fillAll(Colour(0xff090b10));
    g.addTransform(
        AffineTransform::scale(getWidth() / 900.f, getHeight() / 1020.f));
    auto panel = Rectangle<float>(12, 10, 876, 1000);
    g.setGradientFill(ColourGradient(Colour(0xff62636a), 12, 10,
                                     Colour(0xff0b0d12), 888, 1010, false));
    g.fillRoundedRectangle(panel, 25);
    g.setGradientFill(ColourGradient(Colour(0xff36373d), 30, 15,
                                     Colour(0xff1d1e24), 700, 1000, false));
    g.fillRoundedRectangle(panel.reduced(3), 22);
    for (int y = 18; y < 1000; y += 2) {
      g.setColour(Colours::white.withAlpha(y % 4 ? .018f : .008f));
      g.drawHorizontalLine(y, 22, 878);
    }
    auto inset = [&](Rectangle<float> r) {
      g.setGradientFill(ColourGradient(Colour(0xff07080b), r.getX(), r.getY(),
                                       Colour(0xff45464d), r.getRight(),
                                       r.getBottom(), false));
      g.fillRoundedRectangle(r, 15);
      g.setColour(Colour(0xff1c1e24));
      g.fillRoundedRectangle(r.reduced(2), 13);
    };
    inset({22, 93, 856, 52});
    inset({22, 453, 856, 366});
    for (auto point : {Point<float>(27, 25), Point<float>(873, 25),
                       Point<float>(27, 997), Point<float>(873, 997)}) {
      g.setColour(Colour(0xff090a0d));
      g.fillEllipse(point.x - 7, point.y - 6, 14, 14);
      g.setGradientFill(ColourGradient(Colour(0xff74767b), point.x - 5,
                                       point.y - 5, Colour(0xff202126),
                                       point.x + 5, point.y + 5, false));
      g.fillEllipse(point.x - 5, point.y - 5, 10, 10);
      g.setColour(Colour(0xff111216));
      g.drawLine(point.x - 3, point.y + 2, point.x + 3, point.y - 2, 2);
    }
    for (int side : {0, 1})
      for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 6; ++col) {
          float x = side ? 560.f + col * 14 : 260.f + col * 14,
                y = 737.f + row * 10;
          g.setColour(Colour(0xff505159));
          g.fillRoundedRectangle(x, y + 1, 9, 3, 1.5f);
          g.setColour(Colour(0xff07090d));
          g.fillRoundedRectangle(x, y, 9, 3, 1.5f);
        }
    g.setFont(FontOptions(31, Font::bold));
    g.setColour(Colours::black.withAlpha(.8f));
    g.drawText("W O N K A", 51, 26, 270, 40, Justification::centredLeft);
    g.setGradientFill(ColourGradient(Colour(0xffeff2f4), 0, 26,
                                     Colour(0xff87939e), 0, 65, false));
    g.drawText("W O N K A", 50, 25, 270, 40, Justification::centredLeft);
    g.setColour(Colour(0xff85d8cc));
    g.setFont(11);
    g.drawText("VOCAL MOTION / REVERSE + THROW", 51, 63, 300, 20,
               Justification::centredLeft);
    g.setColour(Colour(0xff9ea7b5));
    g.setFont(11);
    g.drawText("SOURCE  /  SELECT  /  RENDER", 33, 155, 380, 26,
               Justification::centredLeft);
    auto label = [&](String t, int x, int y, int w) {
      g.setColour(Colour(0xffb8c3d5));
      g.setFont(14);
      g.drawText(t, x, y, w, 22, Justification::centred);
    };
    label("SPACE", 180, 640, 180);
    label("LENGTH", 540, 640, 180);
    label("PREVIA", 28, 854, 120);
    label("BPM manual", 200, 854, 120);
    label("FEEDBACK", 370, 854, 130);
    label("COMPASSO", 590, 854, 190);
    g.setColour(Colour(0xff64718a));
    g.setFont(12);
    g.drawText("Reverse: alinhe o fim do WAV ao inicio da voz. Arraste depende "
               "do host.",
               30, 986, 840, 20, Justification::centred);
  }
  void resized() override {
    float sx = getWidth() / 900.f, sy = getHeight() / 1020.f;
    auto b = [&](Component &c, int x, int y, int w, int h) {
      c.setBounds(roundToInt(x * sx), roundToInt(y * sy), roundToInt(w * sx),
                  roundToInt(h * sy));
    };
    b(presets, 370, 32, 250, 32);
    b(scale, 635, 32, 90, 32);
    b(bypass, 750, 32, 110, 32);
    b(modeButtons[0], 30, 100, 410, 36);
    b(modeButtons[1], 455, 100, 415, 36);
    b(variant, 455, 153, 150, 32);
    b(division, 620, 153, 250, 32);
    b(wave, 30, 193, 840, 182);
    b(open, 30, 388, 130, 32);
    b(rec, 170, 388, 150, 32);
    b(play, 330, 388, 150, 32);
    b(compare, 490, 388, 160, 32);
    b(suggest, 660, 388, 210, 32);
    b(durationLabel, 30, 430, 840, 25);
    b(space, 175, 455, 190, 180);
    b(length, 535, 455, 190, 180);
    for (int i = 0; i < 4; ++i)
      b(tabButtons[i], 240 + i * 100, 675, 95, 30);
    b(tremRate, 530, 735, 110, 28);
    b(small, 390, 710, 120, 100);
    b(sync, 30, 675, 150, 30);
    b(musical, 30, 710, 150, 30);
    b(tail, 30, 745, 160, 30);
    b(original, 660, 675, 200, 30);
    b(normalise, 660, 710, 180, 30);
    b(protect, 660, 745, 200, 30);
    b(volume, 28, 880, 145, 40);
    b(bpm, 195, 880, 145, 40);
    b(feedback, 365, 880, 145, 40);
    b(num, 575, 880, 120, 40);
    b(denom, 715, 880, 100, 32);
    b(rate, 30, 936, 155, 32);
    b(bits, 195, 936, 160, 32);
    b(save, 365, 936, 175, 32);
    b(drag, 550, 936, 160, 32);
    b(status, 30, 824, 840, 26);
  }
};
} // namespace
AudioProcessorEditor *VoltaProcessor::createEditor() {
  return new Editor(*this);
}
