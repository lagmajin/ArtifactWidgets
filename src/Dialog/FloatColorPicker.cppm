module;
#include <QDialog>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSignalBlocker>
#include <QSlider>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTabBar>
#include <QVBoxLayout>
#include <QColor>
#include <cmath>
#include <wobjectimpl.h>


module FloatColorPickerDialog;

import Color.Float;
import Color.Conversion;
import Widgets.Dialog.Abstract;
import Widgets.ColorWheel;
import Widget.ColorViewLabel;
import EnhancedSlider;

namespace ArtifactWidgets {

W_OBJECT_IMPL(FloatColorPicker)

// ---- helper: make a labeled slider row ---------------------------------
// Returns the containing QHBoxLayout; all widgets are parented to parent.
static QHBoxLayout *makeSliderRow(const QString &labelText,
                                  QSlider *slider,
                                  QWidget *spinbox,
                                  QWidget *parent,
                                  QLabel **outLabel = nullptr,
                                  bool addDegreeLabel = false) {
  auto *row = new QHBoxLayout();
  auto *lbl = new QLabel(labelText, parent);
  lbl->setFixedWidth(14);
  if (outLabel)
    *outLabel = lbl;
  row->addWidget(lbl);
  row->addWidget(slider, 1);
  row->addWidget(spinbox);
  if (addDegreeLabel) {
    auto *deg = new QLabel(QStringLiteral("°"), parent);
    row->addWidget(deg);
  }
  return row;
}

// ---- Impl --------------------------------------------------------------

class FloatColorPicker::Impl {
public:
  ArtifactCore::FloatColor currentColor;
  ArtifactCore::FloatColor initialColor;

  ColorWheelWidget         *colorWheel        = nullptr;
  QSlider                  *brightnessSlider  = nullptr;
  ::ArtifactWidgets::ColorViewLabel *colorPreviewBar = nullptr;

  QTabBar        *tabBar     = nullptr;
  QStackedWidget *tabContent = nullptr;

  // HSB tab
  QSlider       *hSlider  = nullptr;
  QSlider       *sSlider  = nullptr;
  QSlider       *bSlider  = nullptr;
  QSpinBox      *hSpin    = nullptr;
  QDoubleSpinBox *sSpin   = nullptr;
  QDoubleSpinBox *bSpin   = nullptr;

  // RGB tab
  QSlider        *rSlider    = nullptr;
  QSlider        *gSlider    = nullptr;
  QSlider        *rgbBSlider = nullptr;
  QDoubleSpinBox *rSpin      = nullptr;
  QDoubleSpinBox *gSpin      = nullptr;
  QDoubleSpinBox *rgbBSpin   = nullptr;

  // HSL tab
  QSlider        *hslHSlider = nullptr;
  QSlider        *hslSSlider = nullptr;
  QSlider        *hslLSlider = nullptr;
  QSpinBox       *hslHSpin   = nullptr;
  QDoubleSpinBox *hslSSpin   = nullptr;
  QDoubleSpinBox *hslLSpin   = nullptr;

  // Shared alpha
  QSlider        *aSlider = nullptr;
  QDoubleSpinBox *aSpin   = nullptr;

  QLineEdit   *hexInput     = nullptr;
  QPushButton *resetButton  = nullptr;
  QPushButton *okButton     = nullptr;
  QPushButton *cancelButton = nullptr;

  bool updatingFromColor = false;

  void updateColorFromHSBSliders();
  void updateColorFromRGBSliders();
  void updateColorFromHSLSliders();
  void updateHexFromColor();
  void updateColorFromHex();
  void updateAllFromColor();
};

// ---- update helpers -----------------------------------------------------

void FloatColorPicker::Impl::updateHexFromColor() {
  int r = static_cast<int>(std::round(currentColor.r() * 255.0f));
  int g = static_cast<int>(std::round(currentColor.g() * 255.0f));
  int b = static_cast<int>(std::round(currentColor.b() * 255.0f));
  int a = static_cast<int>(std::round(currentColor.a() * 255.0f));
  const QSignalBlocker bl(hexInput);
  hexInput->setText(QString("%1%2%3%4")
                        .arg(r, 2, 16, QChar('0'))
                        .arg(g, 2, 16, QChar('0'))
                        .arg(b, 2, 16, QChar('0'))
                        .arg(a, 2, 16, QChar('0'))
                        .toUpper());
}

void FloatColorPicker::Impl::updateColorFromHex() {
  QString hex = hexInput->text().remove('#');
  if (hex.length() != 6 && hex.length() != 8)
    return;
  bool ok;
  int r = hex.mid(0, 2).toInt(&ok, 16); if (!ok) return;
  int g = hex.mid(2, 2).toInt(&ok, 16); if (!ok) return;
  int b = hex.mid(4, 2).toInt(&ok, 16); if (!ok) return;
  int a = 255;
  if (hex.length() == 8) {
    a = hex.mid(6, 2).toInt(&ok, 16);
    if (!ok) return;
  }
  currentColor.setColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void FloatColorPicker::Impl::updateAllFromColor() {
  if (updatingFromColor)
    return;
  updatingFromColor = true;

  // HSB panel
  ArtifactCore::HSVColor hsv = ArtifactCore::ColorConversion::RGBToHSV(
      currentColor.r(), currentColor.g(), currentColor.b());
  float h = hsv.h, s = hsv.s, v = hsv.v;
  {
    const QSignalBlocker b1(hSlider),  b2(sSlider),  b3(bSlider);
    const QSignalBlocker b4(hSpin),    b5(sSpin),    b6(bSpin);
    hSlider->setValue(static_cast<int>(std::round(h)));
    sSlider->setValue(static_cast<int>(std::round(s * 1000.0f)));
    bSlider->setValue(static_cast<int>(std::round(v * 1000.0f)));
    hSpin->setValue(static_cast<int>(std::round(h)));
    sSpin->setValue(static_cast<double>(s));
    bSpin->setValue(static_cast<double>(v));
  }

  // brightness slider (V channel)
  {
    const QSignalBlocker b(brightnessSlider);
    brightnessSlider->setValue(static_cast<int>(std::round(v * 1000.0f)));
  }

  // RGB panel
  {
    const QSignalBlocker b1(rSlider),   b2(gSlider),   b3(rgbBSlider);
    const QSignalBlocker b4(rSpin),     b5(gSpin),     b6(rgbBSpin);
    rSlider->setValue(static_cast<int>(std::round(currentColor.r() * 1000.0f)));
    gSlider->setValue(static_cast<int>(std::round(currentColor.g() * 1000.0f)));
    rgbBSlider->setValue(static_cast<int>(std::round(currentColor.b() * 1000.0f)));
    rSpin->setValue(static_cast<double>(currentColor.r()));
    gSpin->setValue(static_cast<double>(currentColor.g()));
    rgbBSpin->setValue(static_cast<double>(currentColor.b()));
  }

  // HSL panel
  ArtifactCore::HSLColor hsl = ArtifactCore::ColorConversion::RGBToHSL(
      currentColor.r(), currentColor.g(), currentColor.b());
  float hl = hsl.h, sl = hsl.s, l = hsl.l;
  {
    const QSignalBlocker b1(hslHSlider), b2(hslSSlider), b3(hslLSlider);
    const QSignalBlocker b4(hslHSpin),   b5(hslSSpin),   b6(hslLSpin);
    hslHSlider->setValue(static_cast<int>(std::round(hl)));
    hslSSlider->setValue(static_cast<int>(std::round(sl * 1000.0f)));
    hslLSlider->setValue(static_cast<int>(std::round(l  * 1000.0f)));
    hslHSpin->setValue(static_cast<int>(std::round(hl)));
    hslSSpin->setValue(static_cast<double>(sl));
    hslLSpin->setValue(static_cast<double>(l));
  }

  // Alpha (shared)
  {
    const QSignalBlocker b1(aSlider), b2(aSpin);
    aSlider->setValue(static_cast<int>(std::round(currentColor.a() * 1000.0f)));
    aSpin->setValue(static_cast<double>(currentColor.a()));
  }

  updateHexFromColor();

  updatingFromColor = false;
}

void FloatColorPicker::Impl::updateColorFromHSBSliders() {
  float h = static_cast<float>(hSlider->value());
  float s = static_cast<float>(sSlider->value()) / 1000.0f;
  float v = static_cast<float>(bSlider->value()) / 1000.0f;
  auto rgb = ArtifactCore::ColorConversion::HSVToRGB({h, s, v});
  currentColor.setColor(rgb[0], rgb[1], rgb[2], currentColor.a());
}

void FloatColorPicker::Impl::updateColorFromRGBSliders() {
  currentColor.setColor(
      rSlider->value()    / 1000.0f,
      gSlider->value()    / 1000.0f,
      rgbBSlider->value() / 1000.0f,
      currentColor.a());
}

void FloatColorPicker::Impl::updateColorFromHSLSliders() {
  float h = static_cast<float>(hslHSlider->value());
  float s = static_cast<float>(hslSSlider->value()) / 1000.0f;
  float l = static_cast<float>(hslLSlider->value()) / 1000.0f;
  auto rgb = ArtifactCore::ColorConversion::HSLToRGB({h, s, l});
  currentColor.setColor(rgb[0], rgb[1], rgb[2], currentColor.a());
}

// ---- Constructor --------------------------------------------------------

FloatColorPicker::FloatColorPicker(QWidget *parent)
    : AbstractDialog(parent), impl_(new Impl()) {
  setWindowTitle(QStringLiteral("カラーピッカー — float"));
  setMinimumSize(560, 460);
  setFrameless(false);
  setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
                 Qt::WindowCloseButtonHint);

  auto &d = *impl_;

  // ---- widgets -----------------------------------------------------------

  d.colorWheel = new ColorWheelWidget(this);
  d.colorWheel->setFixedSize(280, 280);

  d.brightnessSlider = new EnhancedSlider(Qt::Vertical, this);
  d.brightnessSlider->setRange(0, 1000);
  d.brightnessSlider->setValue(1000);
  d.brightnessSlider->setFixedWidth(22);
  d.brightnessSlider->setFixedHeight(280);
  d.brightnessSlider->setInvertedAppearance(true);
  d.brightnessSlider->setProperty("artifactFillColor", QColor(255, 208, 92));
  d.brightnessSlider->setProperty("artifactTrackColor", QColor(72, 76, 84));
  d.brightnessSlider->setProperty("artifactHandleColor", QColor(238, 241, 246));

  d.colorPreviewBar = new ColorViewLabel(this);
  d.colorPreviewBar->setFixedHeight(45);

  d.tabBar = new QTabBar(this);
  d.tabBar->addTab(QStringLiteral("HSB"));
  d.tabBar->addTab(QStringLiteral("RGB"));
  d.tabBar->addTab(QStringLiteral("HSL"));
  d.tabBar->setCurrentIndex(0);

  d.tabContent = new QStackedWidget(this);

  // helper lambda: create a QDoubleSpinBox with 3 decimals [0,1]
  auto makeDblSpin = [this](double lo, double hi) -> QDoubleSpinBox * {
    auto *sp = new QDoubleSpinBox(this);
    sp->setRange(lo, hi);
    sp->setDecimals(3);
    sp->setSingleStep(0.001);
    sp->setButtonSymbols(QAbstractSpinBox::NoButtons);
    sp->setFixedWidth(60);
    return sp;
  };
  // helper lambda: create an int QSpinBox
  auto makeIntSpin = [this](int lo, int hi) -> QSpinBox * {
    auto *sp = new QSpinBox(this);
    sp->setRange(lo, hi);
    sp->setButtonSymbols(QAbstractSpinBox::NoButtons);
    sp->setFixedWidth(60);
    return sp;
  };
  // helper lambda: create a horizontal slider
  auto makeHSlider = [this](int lo, int hi) -> QSlider * {
    auto *sl = new EnhancedSlider(Qt::Horizontal, this);
    sl->setRange(lo, hi);
    return sl;
  };
  auto applySliderPalette = [](QSlider* slider, const QColor& fill,
                               const QColor& track = QColor(72, 76, 84),
                               const QColor& handle = QColor(238, 241, 246)) {
    if (!slider) {
      return;
    }
    slider->setProperty("artifactFillColor", fill);
    slider->setProperty("artifactTrackColor", track);
    slider->setProperty("artifactHandleColor", handle);
    slider->update();
  };

  // --- HSB panel ----------------------------------------------------------
  d.hSlider = makeHSlider(0, 360);
  d.sSlider = makeHSlider(0, 1000);
  d.bSlider = makeHSlider(0, 1000);
  d.hSpin   = makeIntSpin(0, 360);
  d.sSpin   = makeDblSpin(0.0, 1.0);
  d.bSpin   = makeDblSpin(0.0, 1.0);

  // Alpha (shared)
  d.aSlider = makeHSlider(0, 1000);
  d.aSpin   = makeDblSpin(0.0, 1.0);
  applySliderPalette(d.hSlider, QColor(255, 149, 64));
  applySliderPalette(d.sSlider, QColor(76, 196, 120));
  applySliderPalette(d.bSlider, QColor(255, 208, 92));
  applySliderPalette(d.aSlider, QColor(156, 168, 184));

  auto *hsbWidget = new QWidget(this);
  auto *hsbLayout = new QVBoxLayout(hsbWidget);
  hsbLayout->setContentsMargins(0, 0, 0, 0);
  hsbLayout->addLayout(makeSliderRow("H", d.hSlider, d.hSpin,   this, nullptr, true));
  hsbLayout->addLayout(makeSliderRow("S", d.sSlider, d.sSpin,   this));
  hsbLayout->addLayout(makeSliderRow("B", d.bSlider, d.bSpin,   this));
  hsbLayout->addStretch();

  // --- RGB panel ----------------------------------------------------------
  d.rSlider    = makeHSlider(0, 1000);
  d.gSlider    = makeHSlider(0, 1000);
  d.rgbBSlider = makeHSlider(0, 1000);
  d.rSpin      = makeDblSpin(0.0, 1.0);
  d.gSpin      = makeDblSpin(0.0, 1.0);
  d.rgbBSpin   = makeDblSpin(0.0, 1.0);
  applySliderPalette(d.rSlider, QColor(230, 92, 92));
  applySliderPalette(d.gSlider, QColor(72, 196, 112));
  applySliderPalette(d.rgbBSlider, QColor(88, 144, 232));

  auto *rgbWidget = new QWidget(this);
  auto *rgbLayout = new QVBoxLayout(rgbWidget);
  rgbLayout->setContentsMargins(0, 0, 0, 0);
  rgbLayout->addLayout(makeSliderRow("R", d.rSlider,    d.rSpin,    this));
  rgbLayout->addLayout(makeSliderRow("G", d.gSlider,    d.gSpin,    this));
  rgbLayout->addLayout(makeSliderRow("B", d.rgbBSlider, d.rgbBSpin, this));
  rgbLayout->addStretch();

  // --- HSL panel ----------------------------------------------------------
  d.hslHSlider = makeHSlider(0, 360);
  d.hslSSlider = makeHSlider(0, 1000);
  d.hslLSlider = makeHSlider(0, 1000);
  d.hslHSpin   = makeIntSpin(0, 360);
  d.hslSSpin   = makeDblSpin(0.0, 1.0);
  d.hslLSpin   = makeDblSpin(0.0, 1.0);
  applySliderPalette(d.hslHSlider, QColor(255, 149, 64));
  applySliderPalette(d.hslSSlider, QColor(76, 196, 120));
  applySliderPalette(d.hslLSlider, QColor(124, 164, 232));

  auto *hslWidget = new QWidget(this);
  auto *hslLayout = new QVBoxLayout(hslWidget);
  hslLayout->setContentsMargins(0, 0, 0, 0);
  hslLayout->addLayout(makeSliderRow("H", d.hslHSlider, d.hslHSpin, this, nullptr, true));
  hslLayout->addLayout(makeSliderRow("S", d.hslSSlider, d.hslSSpin, this));
  hslLayout->addLayout(makeSliderRow("L", d.hslLSlider, d.hslLSpin, this));
  hslLayout->addStretch();

  d.tabContent->addWidget(hsbWidget);  // index 0
  d.tabContent->addWidget(rgbWidget);  // index 1
  d.tabContent->addWidget(hslWidget);  // index 2

  // --- Hex row ------------------------------------------------------------
  d.hexInput = new QLineEdit(this);
  d.hexInput->setMaxLength(8);
  d.hexInput->setFixedWidth(120);
  d.hexInput->setValidator(
      new QRegularExpressionValidator(
          QRegularExpression("^[0-9A-Fa-f]{0,8}$"), d.hexInput));

  auto *hexHashLabel  = new QLabel(QStringLiteral("#"), this);
  auto *hexHintLabel  = new QLabel(QStringLiteral("8桁でアルファ含む"), this);
  {
    QFont f = hexHintLabel->font();
    f.setPointSizeF(f.pointSizeF() * 0.85);
    hexHintLabel->setFont(f);
    QPalette pal = hexHintLabel->palette();
    pal.setColor(QPalette::WindowText, Qt::gray);
    hexHintLabel->setPalette(pal);
  }

  auto *hexRow = new QHBoxLayout();
  hexRow->addWidget(hexHashLabel);
  hexRow->addWidget(d.hexInput);
  hexRow->addWidget(hexHintLabel);
  hexRow->addStretch();

  // --- Buttons ------------------------------------------------------------
  d.resetButton  = new QPushButton(QStringLiteral("リセット"),    this);
  d.cancelButton = new QPushButton(QStringLiteral("キャンセル"),  this);
  d.okButton     = new QPushButton(QStringLiteral("OK"),          this);

  auto *buttonRow = new QHBoxLayout();
  buttonRow->addWidget(d.resetButton);
  buttonRow->addStretch();
  buttonRow->addWidget(d.okButton);
  buttonRow->addWidget(d.cancelButton);

  // ---- layout assembly ---------------------------------------------------

  auto *leftLayout = new QVBoxLayout();
  leftLayout->setContentsMargins(0, 0, 0, 0);
  leftLayout->addWidget(d.colorWheel);

  auto *middleLayout = new QHBoxLayout();
  middleLayout->addLayout(leftLayout);
  middleLayout->addWidget(d.brightnessSlider);

  auto *rightLayout = new QVBoxLayout();
  rightLayout->setContentsMargins(0, 0, 0, 0);
  rightLayout->addWidget(d.colorPreviewBar);
  rightLayout->addWidget(d.tabContent);
  auto *alphaRow = new QHBoxLayout();
  auto *alphaLabel = new QLabel(QStringLiteral("A"), this);
  alphaLabel->setFixedWidth(14);
  alphaRow->addWidget(alphaLabel);
  alphaRow->addWidget(d.aSlider, 1);
  alphaRow->addWidget(d.aSpin);
  rightLayout->addLayout(alphaRow);
  rightLayout->addLayout(hexRow);

  middleLayout->addLayout(rightLayout, 1);

  auto *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(d.tabBar);
  mainLayout->addLayout(middleLayout);
  mainLayout->addLayout(buttonRow);

  // ---- signal connections ------------------------------------------------

  auto emitChanged = [this]() { emit colorChanged(impl_->currentColor); };

  // Tab switching
  connect(d.tabBar, &QTabBar::currentChanged,
          d.tabContent, &QStackedWidget::setCurrentIndex);

  // Color wheel → H + S
  connect(d.colorWheel, &ColorWheelWidget::colorChanged, this,
          [this, emitChanged](const ArtifactCore::FloatColor &wc) {
            if (impl_->updatingFromColor)
              return;
            ArtifactCore::HSVColor wheelHsv = ArtifactCore::ColorConversion::RGBToHSV(wc.r(), wc.g(), wc.b());
            ArtifactCore::HSVColor curHsv = ArtifactCore::ColorConversion::RGBToHSV(
                impl_->currentColor.r(), impl_->currentColor.g(), impl_->currentColor.b());
            auto rgb = ArtifactCore::ColorConversion::HSVToRGB({wheelHsv.h, wheelHsv.s, curHsv.v});
            impl_->currentColor.setColor(rgb[0], rgb[1], rgb[2], impl_->currentColor.a());
            impl_->updateAllFromColor();
            impl_->colorPreviewBar->setColor(impl_->currentColor);
            emitChanged();
          });

  // Brightness slider → V
  connect(d.brightnessSlider, &QSlider::valueChanged, this,
          [this, emitChanged](int val) {
            if (impl_->updatingFromColor)
              return;
            ArtifactCore::HSVColor curHsv = ArtifactCore::ColorConversion::RGBToHSV(
                impl_->currentColor.r(), impl_->currentColor.g(), impl_->currentColor.b());
            auto rgb = ArtifactCore::ColorConversion::HSVToRGB({curHsv.h, curHsv.s, val / 1000.0f});
            impl_->currentColor.setColor(rgb[0], rgb[1], rgb[2], impl_->currentColor.a());
            impl_->updateAllFromColor();
            impl_->colorPreviewBar->setColor(impl_->currentColor);
            emitChanged();
          });

  // HSB sliders → color
  auto hsbSliderChanged = [this, emitChanged]() {
    if (impl_->updatingFromColor) return;
    impl_->updateColorFromHSBSliders();
    impl_->updateAllFromColor();
    impl_->colorWheel->setColor(impl_->currentColor);
    impl_->colorPreviewBar->setColor(impl_->currentColor);
    emitChanged();
  };
  connect(d.hSlider, &QSlider::valueChanged, this, [this, hsbSliderChanged](int v) {
    const QSignalBlocker bl(impl_->hSpin);
    impl_->hSpin->setValue(v);
    hsbSliderChanged();
  });
  connect(d.sSlider, &QSlider::valueChanged, this, [this, hsbSliderChanged](int v) {
    const QSignalBlocker bl(impl_->sSpin);
    impl_->sSpin->setValue(v / 1000.0);
    hsbSliderChanged();
  });
  connect(d.bSlider, &QSlider::valueChanged, this, [this, hsbSliderChanged](int v) {
    const QSignalBlocker bl(impl_->bSpin);
    impl_->bSpin->setValue(v / 1000.0);
    hsbSliderChanged();
  });

  // HSB spinboxes → sliders
  connect(d.hSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this, hsbSliderChanged](int v) {
            const QSignalBlocker bl(impl_->hSlider);
            impl_->hSlider->setValue(v);
            hsbSliderChanged();
          });
  connect(d.sSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this, hsbSliderChanged](double v) {
            const QSignalBlocker bl(impl_->sSlider);
            impl_->sSlider->setValue(static_cast<int>(std::round(v * 1000.0)));
            hsbSliderChanged();
          });
  connect(d.bSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this, hsbSliderChanged](double v) {
            const QSignalBlocker bl(impl_->bSlider);
            impl_->bSlider->setValue(static_cast<int>(std::round(v * 1000.0)));
            hsbSliderChanged();
          });

  // RGB sliders → color
  auto rgbSliderChanged = [this, emitChanged]() {
    if (impl_->updatingFromColor) return;
    impl_->updateColorFromRGBSliders();
    impl_->updateAllFromColor();
    impl_->colorWheel->setColor(impl_->currentColor);
    impl_->colorPreviewBar->setColor(impl_->currentColor);
    emitChanged();
  };
  connect(d.rSlider, &QSlider::valueChanged, this, [this, rgbSliderChanged](int v) {
    const QSignalBlocker bl(impl_->rSpin);
    impl_->rSpin->setValue(v / 1000.0);
    rgbSliderChanged();
  });
  connect(d.gSlider, &QSlider::valueChanged, this, [this, rgbSliderChanged](int v) {
    const QSignalBlocker bl(impl_->gSpin);
    impl_->gSpin->setValue(v / 1000.0);
    rgbSliderChanged();
  });
  connect(d.rgbBSlider, &QSlider::valueChanged, this, [this, rgbSliderChanged](int v) {
    const QSignalBlocker bl(impl_->rgbBSpin);
    impl_->rgbBSpin->setValue(v / 1000.0);
    rgbSliderChanged();
  });

  // RGB spinboxes → sliders
  connect(d.rSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this, rgbSliderChanged](double v) {
            const QSignalBlocker bl(impl_->rSlider);
            impl_->rSlider->setValue(static_cast<int>(std::round(v * 1000.0)));
            rgbSliderChanged();
          });
  connect(d.gSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this, rgbSliderChanged](double v) {
            const QSignalBlocker bl(impl_->gSlider);
            impl_->gSlider->setValue(static_cast<int>(std::round(v * 1000.0)));
            rgbSliderChanged();
          });
  connect(d.rgbBSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this, rgbSliderChanged](double v) {
            const QSignalBlocker bl(impl_->rgbBSlider);
            impl_->rgbBSlider->setValue(static_cast<int>(std::round(v * 1000.0)));
            rgbSliderChanged();
          });

  // HSL sliders → color
  auto hslSliderChanged = [this, emitChanged]() {
    if (impl_->updatingFromColor) return;
    impl_->updateColorFromHSLSliders();
    impl_->updateAllFromColor();
    impl_->colorWheel->setColor(impl_->currentColor);
    impl_->colorPreviewBar->setColor(impl_->currentColor);
    emitChanged();
  };
  connect(d.hslHSlider, &QSlider::valueChanged, this, [this, hslSliderChanged](int v) {
    const QSignalBlocker bl(impl_->hslHSpin);
    impl_->hslHSpin->setValue(v);
    hslSliderChanged();
  });
  connect(d.hslSSlider, &QSlider::valueChanged, this, [this, hslSliderChanged](int v) {
    const QSignalBlocker bl(impl_->hslSSpin);
    impl_->hslSSpin->setValue(v / 1000.0);
    hslSliderChanged();
  });
  connect(d.hslLSlider, &QSlider::valueChanged, this, [this, hslSliderChanged](int v) {
    const QSignalBlocker bl(impl_->hslLSpin);
    impl_->hslLSpin->setValue(v / 1000.0);
    hslSliderChanged();
  });

  // HSL spinboxes → sliders
  connect(d.hslHSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this, hslSliderChanged](int v) {
            const QSignalBlocker bl(impl_->hslHSlider);
            impl_->hslHSlider->setValue(v);
            hslSliderChanged();
          });
  connect(d.hslSSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this, hslSliderChanged](double v) {
            const QSignalBlocker bl(impl_->hslSSlider);
            impl_->hslSSlider->setValue(static_cast<int>(std::round(v * 1000.0)));
            hslSliderChanged();
          });
  connect(d.hslLSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this, hslSliderChanged](double v) {
            const QSignalBlocker bl(impl_->hslLSlider);
            impl_->hslLSlider->setValue(static_cast<int>(std::round(v * 1000.0)));
            hslSliderChanged();
          });

  // Alpha slider
  connect(d.aSlider, &QSlider::valueChanged, this,
          [this, emitChanged](int val) {
            if (impl_->updatingFromColor) return;
            impl_->currentColor.setColor(
                impl_->currentColor.r(), impl_->currentColor.g(),
                impl_->currentColor.b(), val / 1000.0f);
            {
              const QSignalBlocker bl(impl_->aSpin);
              impl_->aSpin->setValue(val / 1000.0);
            }
            impl_->updateHexFromColor();
            impl_->colorPreviewBar->setColor(impl_->currentColor);
            emitChanged();
          });
  connect(d.aSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          [this, emitChanged](double v) {
            const QSignalBlocker bl(impl_->aSlider);
            impl_->aSlider->setValue(static_cast<int>(std::round(v * 1000.0)));
            if (impl_->updatingFromColor) return;
            impl_->currentColor.setColor(
                impl_->currentColor.r(), impl_->currentColor.g(),
                impl_->currentColor.b(), static_cast<float>(v));
            impl_->updateAllFromColor();
            impl_->colorPreviewBar->setColor(impl_->currentColor);
            emitChanged();
          });

  // Hex input
  auto hexFinished = [this, emitChanged]() {
    impl_->updateColorFromHex();
    impl_->updateAllFromColor();
    impl_->colorWheel->setColor(impl_->currentColor);
    impl_->colorPreviewBar->setColor(impl_->currentColor);
    emitChanged();
  };
  connect(d.hexInput, &QLineEdit::editingFinished, this, hexFinished);
  connect(d.hexInput, &QLineEdit::returnPressed,   this, hexFinished);

  // Reset / OK / Cancel
  connect(d.resetButton, &QPushButton::clicked, this,
          [this, emitChanged]() {
            impl_->currentColor = impl_->initialColor;
            impl_->updateAllFromColor();
            impl_->colorWheel->setColor(impl_->currentColor);
            impl_->colorPreviewBar->setColor(impl_->currentColor);
            emitChanged();
          });
  connect(d.okButton,     &QPushButton::clicked, this, &QDialog::accept);
  connect(d.cancelButton, &QPushButton::clicked, this, &QDialog::reject);

  // Initial display
  d.updateAllFromColor();
  d.colorPreviewBar->setColor(d.currentColor);
}

FloatColorPicker::~FloatColorPicker() { delete impl_; }

ArtifactCore::FloatColor FloatColorPicker::getColor() const {
  return impl_->currentColor;
}

void FloatColorPicker::setColor(const ArtifactCore::FloatColor &color) {
  impl_->currentColor = color;
  impl_->updateAllFromColor();
  impl_->colorWheel->setColor(color);
  impl_->colorPreviewBar->setColor(color);
}

void FloatColorPicker::setInitialColor(const ArtifactCore::FloatColor &color) {
  impl_->initialColor = color;
  impl_->currentColor = color;
  impl_->updateAllFromColor();
  impl_->colorWheel->setColor(color);
  impl_->colorPreviewBar->setColor(color);
}

void FloatColorPicker::accept() { QDialog::accept(); }

void FloatColorPicker::reject() { QDialog::reject(); }

} // namespace ArtifactWidgets
