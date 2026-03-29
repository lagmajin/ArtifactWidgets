module;
#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDial>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSignalBlocker>
#include <QAbstractSlider>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>
#include <cmath>
#include <wobjectimpl.h>


module FloatColorPickerDialog;

import Color.Float;
import Widgets.Dialog.Abstract;
import Widgets.ColorWheel;
import Widget.ColorViewLabel;

namespace ArtifactWidgets {

W_OBJECT_IMPL(FloatColorPicker)

class FloatColorPicker::Impl {
public:
  Impl();
  ~Impl();

  ArtifactCore::FloatColor currentColor;
  ArtifactCore::FloatColor initialColor;
  ColorWheelWidget *colorWheel = nullptr;
  ::ArtifactWidgets::ColorViewLabel *colorPreview = nullptr;
  ::ArtifactWidgets::ColorViewLabel *originalPreview = nullptr;
  QPushButton *revertButton = nullptr;

  // RGBAスライダー
  QSlider *redSlider = nullptr;
  QSlider *greenSlider = nullptr;
  QSlider *blueSlider = nullptr;
  QSlider *alphaSlider = nullptr;
  QSpinBox *redSpin = nullptr;
  QSpinBox *greenSpin = nullptr;
  QSpinBox *blueSpin = nullptr;
  QSpinBox *alphaSpin = nullptr;
  QLabel *redLabel = nullptr;
  QLabel *greenLabel = nullptr;
  QLabel *blueLabel = nullptr;
  QLabel *alphaLabel = nullptr;

  // HSV controls
  QSlider *hueSlider = nullptr;
  QSlider *saturationSlider = nullptr;
  QDial *valueDial = nullptr;
  QSpinBox *hueSpin = nullptr;
  QSpinBox *saturationSpin = nullptr;
  QSpinBox *valueSpin = nullptr;
  QLabel *hueLabel = nullptr;
  QLabel *saturationLabel = nullptr;
  QLabel *valueLabel = nullptr;

  // HEX入力
  QLineEdit *hexInput = nullptr;
  QLabel *hexLabel = nullptr;

  // プリセットカラーパレット
  QPushButton *presetButtons[16] = {nullptr};

  // ボタン
  QPushButton *okButton = nullptr;
  QPushButton *cancelButton = nullptr;

  bool updatingFromColor = false;

  void updateSlidersFromColor();
  void updateColorFromRGBSliders();
  void updateColorFromHSVSliders();
  void updateHexFromColor();
  void updateColorFromHex();
  void updateAllFromColor();
  void rgbToHSV(float r, float g, float b, float &h, float &s, float &v);
  void hsvToRGB(float h, float s, float v, float &r, float &g, float &b);
};

FloatColorPicker::Impl::Impl()
    : redSlider(new QSlider(Qt::Horizontal)),
      greenSlider(new QSlider(Qt::Horizontal)),
      blueSlider(new QSlider(Qt::Horizontal)),
      alphaSlider(new QSlider(Qt::Horizontal)), redLabel(new QLabel("R")),
      greenLabel(new QLabel("G")), blueLabel(new QLabel("B")),
      alphaLabel(new QLabel("A")), hueSlider(new QSlider(Qt::Horizontal)),
      saturationSlider(new QSlider(Qt::Horizontal)),
      valueDial(new QDial()), hueLabel(new QLabel("H")),
      saturationLabel(new QLabel("S")), valueLabel(new QLabel("V")),
      hexInput(new QLineEdit()), hexLabel(new QLabel("#")),
      okButton(new QPushButton("OK")), cancelButton(new QPushButton("Cancel")) {
  initialColor = currentColor;
  redSpin = new QSpinBox();
  greenSpin = new QSpinBox();
  blueSpin = new QSpinBox();
  alphaSpin = new QSpinBox();
  hueSpin = new QSpinBox();
  saturationSpin = new QSpinBox();
  valueSpin = new QSpinBox();

  redSlider->setRange(0, 255);
  greenSlider->setRange(0, 255);
  blueSlider->setRange(0, 255);
  alphaSlider->setRange(0, 255);
  redSpin->setRange(0, 255);
  greenSpin->setRange(0, 255);
  blueSpin->setRange(0, 255);
  alphaSpin->setRange(0, 255);

  hueSlider->setRange(0, 360);
  saturationSlider->setRange(0, 100);
  valueDial->setRange(0, 100);
  valueDial->setNotchesVisible(false);
  valueDial->setWrapping(false);
  valueDial->setFixedSize(72, 72);
  hueSpin->setRange(0, 360);
  saturationSpin->setRange(0, 100);
  valueSpin->setRange(0, 100);

  redSlider->setValue(255);
  greenSlider->setValue(255);
  blueSlider->setValue(255);
  alphaSlider->setValue(255);
  redSpin->setValue(255);
  greenSpin->setValue(255);
  blueSpin->setValue(255);
  alphaSpin->setValue(255);

  hexInput->setMaxLength(9);
  hexInput->setFixedWidth(96);
  hexInput->setPlaceholderText(QStringLiteral("#RRGGBB or #RRGGBBAA"));
  hexInput->setValidator(
      new QRegularExpressionValidator(QRegularExpression("^#?[0-9A-Fa-f]{0,8}$"),
                                      hexInput));

  for (QSpinBox *spin : {redSpin, greenSpin, blueSpin, alphaSpin, hueSpin,
                         saturationSpin, valueSpin}) {
    spin->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spin->setFixedWidth(56);
  }

  // プリセットカラー
  const char *presetColors[] = {"#FF0000", "#FF8000", "#FFFF00", "#80FF00",
                                "#00FF00", "#00FF80", "#00FFFF", "#0080FF",
                                "#0000FF", "#8000FF", "#FF00FF", "#FF0080",
                                "#FFFFFF", "#C0C0C0", "#808080", "#000000"};

  for (int i = 0; i < 16; ++i) {
    presetButtons[i] = new QPushButton();
    presetButtons[i]->setFixedSize(24, 24);
    presetButtons[i]->setStyleSheet(
        QString("background-color: %1; border: 1px solid #3E3E42;")
            .arg(presetColors[i]));
  }
}

FloatColorPicker::Impl::~Impl() {}

void FloatColorPicker::Impl::rgbToHSV(float r, float g, float b, float &h,
                                      float &s, float &v) {
  float max = std::max({r, g, b});
  float min = std::min({r, g, b});
  float d = max - min;

  v = max;
  s = max == 0 ? 0 : d / max;

  if (d == 0) {
    h = 0;
  } else {
    if (max == r)
      h = 60 * fmod((g - b) / d, 6);
    else if (max == g)
      h = 60 * ((b - r) / d + 2);
    else
      h = 60 * ((r - g) / d + 4);
    if (h < 0)
      h += 360;
  }
}

void FloatColorPicker::Impl::hsvToRGB(float h, float s, float v, float &r,
                                      float &g, float &b) {
  int hi = static_cast<int>(h / 60) % 6;
  float f = h / 60 - hi;
  float p = v * (1 - s);
  float q = v * (1 - f * s);
  float t = v * (1 - (1 - f) * s);

  switch (hi) {
  case 0:
    r = v;
    g = t;
    b = p;
    break;
  case 1:
    r = q;
    g = v;
    b = p;
    break;
  case 2:
    r = p;
    g = v;
    b = t;
    break;
  case 3:
    r = p;
    g = q;
    b = v;
    break;
  case 4:
    r = t;
    g = p;
    b = v;
    break;
  default:
    r = v;
    g = p;
    b = q;
    break;
  }
}

void FloatColorPicker::Impl::updateSlidersFromColor() {
  const int r = static_cast<int>(std::round(currentColor.r() * 255.0f));
  const int g = static_cast<int>(std::round(currentColor.g() * 255.0f));
  const int b = static_cast<int>(std::round(currentColor.b() * 255.0f));
  const int a = static_cast<int>(std::round(currentColor.a() * 255.0f));

  {
    const QSignalBlocker b1(redSlider);
    const QSignalBlocker b2(greenSlider);
    const QSignalBlocker b3(blueSlider);
    const QSignalBlocker b4(alphaSlider);
    const QSignalBlocker b5(redSpin);
    const QSignalBlocker b6(greenSpin);
    const QSignalBlocker b7(blueSpin);
    const QSignalBlocker b8(alphaSpin);
    redSlider->setValue(r);
    greenSlider->setValue(g);
    blueSlider->setValue(b);
    alphaSlider->setValue(a);
    redSpin->setValue(r);
    greenSpin->setValue(g);
    blueSpin->setValue(b);
    alphaSpin->setValue(a);
  }

  float h, s, v;
  rgbToHSV(currentColor.r(), currentColor.g(), currentColor.b(), h, s, v);
  const int ih = static_cast<int>(std::round(h));
  const int is = static_cast<int>(std::round(s * 100.0f));
  const int iv = static_cast<int>(std::round(v * 100.0f));
  {
    const QSignalBlocker b1(hueSlider);
    const QSignalBlocker b2(saturationSlider);
    const QSignalBlocker b3(valueDial);
    const QSignalBlocker b4(hueSpin);
    const QSignalBlocker b5(saturationSpin);
    const QSignalBlocker b6(valueSpin);
    hueSlider->setValue(ih);
    saturationSlider->setValue(is);
    valueDial->setValue(iv);
    hueSpin->setValue(ih);
    saturationSpin->setValue(is);
    valueSpin->setValue(iv);
  }
}

void FloatColorPicker::Impl::updateColorFromRGBSliders() {
  if (updatingFromColor)
    return;
  updatingFromColor = true;

  currentColor.setColor(
      redSlider->value() / 255.0f, greenSlider->value() / 255.0f,
      blueSlider->value() / 255.0f, alphaSlider->value() / 255.0f);

  updateSlidersFromColor();
  updateHexFromColor();
  updatingFromColor = false;
}

void FloatColorPicker::Impl::updateColorFromHSVSliders() {
  if (updatingFromColor)
    return;
  updatingFromColor = true;

  float h = hueSlider->value();
  float s = saturationSlider->value() / 100.0f;
  float v = valueDial->value() / 100.0f;

  float r, g, b;
  hsvToRGB(h, s, v, r, g, b);

  currentColor.setColor(r, g, b, currentColor.a());

  updateSlidersFromColor();
  updateHexFromColor();
  updatingFromColor = false;
}

void FloatColorPicker::Impl::updateHexFromColor() {
  int r = static_cast<int>(std::round(currentColor.r() * 255));
  int g = static_cast<int>(std::round(currentColor.g() * 255));
  int b = static_cast<int>(std::round(currentColor.b() * 255));
  int a = static_cast<int>(std::round(currentColor.a() * 255));

  const QSignalBlocker blocker(hexInput);
  if (a < 255) {
    hexInput->setText(QString("#%1%2%3%4")
                          .arg(r, 2, 16, QChar('0'))
                          .arg(g, 2, 16, QChar('0'))
                          .arg(b, 2, 16, QChar('0'))
                          .arg(a, 2, 16, QChar('0'))
                          .toUpper());
  } else {
    hexInput->setText(QString("#%1%2%3")
                          .arg(r, 2, 16, QChar('0'))
                          .arg(g, 2, 16, QChar('0'))
                          .arg(b, 2, 16, QChar('0'))
                          .toUpper());
  }
}

void FloatColorPicker::Impl::updateColorFromHex() {
  QString hex = hexInput->text().remove('#');
  if (hex.length() == 6 || hex.length() == 8) {
    bool ok;
    int r = hex.mid(0, 2).toInt(&ok, 16);
    if (!ok)
      return;
    int g = hex.mid(2, 2).toInt(&ok, 16);
    if (!ok)
      return;
    int b = hex.mid(4, 2).toInt(&ok, 16);
    if (!ok)
      return;
    int a = 255;
    if (hex.length() == 8) {
      a = hex.mid(6, 2).toInt(&ok, 16);
      if (!ok)
        return;
    }

    currentColor.setColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    updateSlidersFromColor();
  }
}

void FloatColorPicker::Impl::updateAllFromColor() {
  updateSlidersFromColor();
  updateHexFromColor();
}

FloatColorPicker::FloatColorPicker(QWidget *parent)
    : AbstractDialog(parent), impl_(new Impl()) {
  setWindowTitle("Color Picker");
  setMinimumSize(520, 560);
  setFrameless(false);
  setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
                 Qt::WindowCloseButtonHint);

  impl_->colorWheel = new ColorWheelWidget(this);
  impl_->colorPreview = new ColorViewLabel(this);
  impl_->originalPreview = new ColorViewLabel(this);
  impl_->revertButton = new QPushButton(QStringLiteral("Revert"), this);
  impl_->colorPreview->setMinimumHeight(64);
  impl_->originalPreview->setMinimumHeight(64);

  // カラーホイールとプレビューのレイアウト
  QHBoxLayout *topLayout = new QHBoxLayout();
  QVBoxLayout *previewLayout = new QVBoxLayout();
  auto *prevLabel = new QLabel(QStringLiteral("Previous"), this);
  auto *currLabel = new QLabel(QStringLiteral("Current"), this);
  previewLayout->addWidget(prevLabel);
  previewLayout->addWidget(impl_->originalPreview);
  previewLayout->addSpacing(4);
  previewLayout->addWidget(currLabel);
  previewLayout->addWidget(impl_->colorPreview);
  previewLayout->addStretch();
  previewLayout->addWidget(impl_->revertButton);

  topLayout->addWidget(impl_->colorWheel, 3);
  topLayout->addLayout(previewLayout, 2);

  // HEX入力
  QHBoxLayout *hexLayout = new QHBoxLayout();
  auto *hexText = new QLabel(QStringLiteral("Hex"), this);
  hexLayout->addWidget(hexText);
  hexLayout->addWidget(impl_->hexLabel);
  hexLayout->addWidget(impl_->hexInput);
  hexLayout->addStretch();

  // RGBAスライダーグループ
  QGroupBox *rgbaGroup = new QGroupBox("RGBA", this);
  QHBoxLayout *redLayout = new QHBoxLayout();
  redLayout->addWidget(impl_->redLabel);
  redLayout->addWidget(impl_->redSlider, 1);
  redLayout->addWidget(impl_->redSpin);

  QHBoxLayout *greenLayout = new QHBoxLayout();
  greenLayout->addWidget(impl_->greenLabel);
  greenLayout->addWidget(impl_->greenSlider, 1);
  greenLayout->addWidget(impl_->greenSpin);

  QHBoxLayout *blueLayout = new QHBoxLayout();
  blueLayout->addWidget(impl_->blueLabel);
  blueLayout->addWidget(impl_->blueSlider, 1);
  blueLayout->addWidget(impl_->blueSpin);

  QHBoxLayout *alphaLayout = new QHBoxLayout();
  alphaLayout->addWidget(impl_->alphaLabel);
  alphaLayout->addWidget(impl_->alphaSlider, 1);
  alphaLayout->addWidget(impl_->alphaSpin);

  QVBoxLayout *rgbaLayout = new QVBoxLayout();
  rgbaLayout->addLayout(redLayout);
  rgbaLayout->addLayout(greenLayout);
  rgbaLayout->addLayout(blueLayout);
  rgbaLayout->addLayout(alphaLayout);
  rgbaGroup->setLayout(rgbaLayout);

  // HSVスライダーグループ
  QGroupBox *hsvGroup = new QGroupBox("HSV", this);
  QHBoxLayout *hueLayout = new QHBoxLayout();
  hueLayout->addWidget(impl_->hueLabel);
  hueLayout->addWidget(impl_->hueSlider, 1);
  hueLayout->addWidget(impl_->hueSpin);

  QHBoxLayout *satLayout = new QHBoxLayout();
  satLayout->addWidget(impl_->saturationLabel);
  satLayout->addWidget(impl_->saturationSlider, 1);
  satLayout->addWidget(impl_->saturationSpin);

  QHBoxLayout *valLayout = new QHBoxLayout();
  valLayout->addWidget(impl_->valueLabel);
  valLayout->addWidget(impl_->valueDial);
  valLayout->addWidget(impl_->valueSpin);

  QVBoxLayout *hsvLayout = new QVBoxLayout();
  hsvLayout->addLayout(hueLayout);
  hsvLayout->addLayout(satLayout);
  hsvLayout->addLayout(valLayout);
  hsvGroup->setLayout(hsvLayout);

  // プリセットカラーパレット
  QGroupBox *presetGroup = new QGroupBox("Presets", this);
  QGridLayout *presetLayout = new QGridLayout();
  for (int i = 0; i < 16; ++i) {
    presetLayout->addWidget(impl_->presetButtons[i], i / 8, i % 8);
  }
  presetGroup->setLayout(presetLayout);

  // ボタンレイアウト
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addStretch();
  buttonLayout->addWidget(impl_->okButton);
  buttonLayout->addWidget(impl_->cancelButton);

  // メインレイアウト
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addLayout(topLayout);
  mainLayout->addLayout(hexLayout);
  mainLayout->addWidget(rgbaGroup);
  mainLayout->addWidget(hsvGroup);
  mainLayout->addWidget(presetGroup);
  mainLayout->addLayout(buttonLayout);

  // シグナル接続
  auto reflectAndEmit = [this]() {
    impl_->colorWheel->setColor(impl_->currentColor);
    impl_->colorPreview->setColor(impl_->currentColor);
    emit colorChanged(impl_->currentColor);
  };

  connect(impl_->colorWheel, &ColorWheelWidget::colorChanged, this,
          [this](const ArtifactCore::FloatColor &color) {
            impl_->currentColor = color;
            impl_->updateAllFromColor();
            impl_->colorPreview->setColor(color);
            emit colorChanged(color);
          });

  connect(impl_->redSlider, &QSlider::valueChanged, this, [this, reflectAndEmit]() {
    const QSignalBlocker blocker(impl_->redSpin);
    impl_->redSpin->setValue(impl_->redSlider->value());
    impl_->updateColorFromRGBSliders();
    reflectAndEmit();
  });
  connect(impl_->greenSlider, &QSlider::valueChanged, this, [this, reflectAndEmit]() {
    const QSignalBlocker blocker(impl_->greenSpin);
    impl_->greenSpin->setValue(impl_->greenSlider->value());
    impl_->updateColorFromRGBSliders();
    reflectAndEmit();
  });
  connect(impl_->blueSlider, &QSlider::valueChanged, this, [this, reflectAndEmit]() {
    const QSignalBlocker blocker(impl_->blueSpin);
    impl_->blueSpin->setValue(impl_->blueSlider->value());
    impl_->updateColorFromRGBSliders();
    reflectAndEmit();
  });
  connect(impl_->alphaSlider, &QSlider::valueChanged, this, [this, reflectAndEmit]() {
    const QSignalBlocker blocker(impl_->alphaSpin);
    impl_->alphaSpin->setValue(impl_->alphaSlider->value());
    impl_->updateColorFromRGBSliders();
    reflectAndEmit();
  });

  connect(impl_->redSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this](int value) { impl_->redSlider->setValue(value); });
  connect(impl_->greenSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this](int value) { impl_->greenSlider->setValue(value); });
  connect(impl_->blueSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this](int value) { impl_->blueSlider->setValue(value); });
  connect(impl_->alphaSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this](int value) { impl_->alphaSlider->setValue(value); });

  connect(impl_->hueSlider, &QSlider::valueChanged, this, [this, reflectAndEmit]() {
    const QSignalBlocker blocker(impl_->hueSpin);
    impl_->hueSpin->setValue(impl_->hueSlider->value());
    impl_->updateColorFromHSVSliders();
    reflectAndEmit();
  });
  connect(impl_->saturationSlider, &QSlider::valueChanged, this, [this, reflectAndEmit]() {
    const QSignalBlocker blocker(impl_->saturationSpin);
    impl_->saturationSpin->setValue(impl_->saturationSlider->value());
    impl_->updateColorFromHSVSliders();
    reflectAndEmit();
  });
  connect(impl_->valueDial, QOverload<int>::of(&QAbstractSlider::valueChanged), this, [this, reflectAndEmit]() {
    const QSignalBlocker blocker(impl_->valueSpin);
    impl_->valueSpin->setValue(impl_->valueDial->value());
    impl_->updateColorFromHSVSliders();
    reflectAndEmit();
  });

  connect(impl_->hueSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this](int value) { impl_->hueSlider->setValue(value); });
  connect(impl_->saturationSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this](int value) { impl_->saturationSlider->setValue(value); });
  connect(impl_->valueSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this](int value) { impl_->valueDial->setValue(value); });

  connect(impl_->hexInput, &QLineEdit::editingFinished, this, [this, reflectAndEmit]() {
    impl_->updateColorFromHex();
    reflectAndEmit();
  });
  connect(impl_->hexInput, &QLineEdit::returnPressed, this, [this, reflectAndEmit]() {
    impl_->updateColorFromHex();
    reflectAndEmit();
  });

  connect(impl_->revertButton, &QPushButton::clicked, this, [this, reflectAndEmit]() {
    impl_->currentColor = impl_->initialColor;
    impl_->updateAllFromColor();
    reflectAndEmit();
  });

  connect(impl_->okButton, &QPushButton::clicked, this, &QDialog::accept);
  connect(impl_->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

  // プリセットボタンの接続
  const char *presetColors[] = {"#FF0000", "#FF8000", "#FFFF00", "#80FF00",
                                "#00FF00", "#00FF80", "#00FFFF", "#0080FF",
                                "#0000FF", "#8000FF", "#FF00FF", "#FF0080",
                                "#FFFFFF", "#C0C0C0", "#808080", "#000000"};

  for (int i = 0; i < 16; ++i) {
    connect(impl_->presetButtons[i], &QPushButton::clicked, this,
            [this, presetColors, i, reflectAndEmit]() {
              QString hex = presetColors[i];
              bool ok;
              int r = hex.mid(1, 2).toInt(&ok, 16);
              int g = hex.mid(3, 2).toInt(&ok, 16);
              int b = hex.mid(5, 2).toInt(&ok, 16);
              impl_->currentColor.setColor(r / 255.0f, g / 255.0f, b / 255.0f,
                                           impl_->currentColor.a());
              impl_->updateAllFromColor();
              reflectAndEmit();
            });
  }

  impl_->colorPreview->setColor(impl_->currentColor);
  impl_->originalPreview->setColor(impl_->initialColor);

}

FloatColorPicker::~FloatColorPicker() { delete impl_; }

ArtifactCore::FloatColor FloatColorPicker::getColor() const {
  return impl_->currentColor;
}

void FloatColorPicker::setColor(const ArtifactCore::FloatColor &color) {
  impl_->currentColor = color;
  impl_->updateAllFromColor();
  impl_->colorWheel->setColor(color);
  impl_->colorPreview->setColor(color);
}

void FloatColorPicker::setInitialColor(const ArtifactCore::FloatColor &color) {
  impl_->initialColor = color;
  impl_->currentColor = color;
  impl_->updateAllFromColor();
  impl_->colorWheel->setColor(color);
  impl_->colorPreview->setColor(color);
  impl_->originalPreview->setColor(color);
}

void FloatColorPicker::accept() { QDialog::accept(); }

void FloatColorPicker::reject() { QDialog::reject(); }

} // namespace ArtifactWidgets
