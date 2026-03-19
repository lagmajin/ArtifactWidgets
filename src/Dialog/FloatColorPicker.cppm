module;
#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QSlider>
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
  ColorWheelWidget *colorWheel = nullptr;
  ::ArtifactWidgets::ColorViewLabel *colorPreview = nullptr;

  // RGBAスライダー
  QSlider *redSlider = nullptr;
  QSlider *greenSlider = nullptr;
  QSlider *blueSlider = nullptr;
  QSlider *alphaSlider = nullptr;
  QLabel *redLabel = nullptr;
  QLabel *greenLabel = nullptr;
  QLabel *blueLabel = nullptr;
  QLabel *alphaLabel = nullptr;

  // HSVスライダー
  QSlider *hueSlider = nullptr;
  QSlider *saturationSlider = nullptr;
  QSlider *valueSlider = nullptr;
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
      valueSlider(new QSlider(Qt::Horizontal)), hueLabel(new QLabel("H")),
      saturationLabel(new QLabel("S")), valueLabel(new QLabel("V")),
      hexInput(new QLineEdit()), hexLabel(new QLabel("#")),
      okButton(new QPushButton("OK")), cancelButton(new QPushButton("Cancel")) {
  redSlider->setRange(0, 255);
  greenSlider->setRange(0, 255);
  blueSlider->setRange(0, 255);
  alphaSlider->setRange(0, 255);

  hueSlider->setRange(0, 360);
  saturationSlider->setRange(0, 100);
  valueSlider->setRange(0, 100);

  redSlider->setValue(255);
  greenSlider->setValue(255);
  blueSlider->setValue(255);
  alphaSlider->setValue(255);

  hexInput->setMaxLength(8);
  hexInput->setFixedWidth(80);

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
  redSlider->setValue(static_cast<int>(currentColor.r() * 255.0f));
  greenSlider->setValue(static_cast<int>(currentColor.g() * 255.0f));
  blueSlider->setValue(static_cast<int>(currentColor.b() * 255.0f));
  alphaSlider->setValue(static_cast<int>(currentColor.a() * 255.0f));

  float h, s, v;
  rgbToHSV(currentColor.r(), currentColor.g(), currentColor.b(), h, s, v);
  hueSlider->setValue(static_cast<int>(h));
  saturationSlider->setValue(static_cast<int>(s * 100));
  valueSlider->setValue(static_cast<int>(v * 100));
}

void FloatColorPicker::Impl::updateColorFromRGBSliders() {
  if (updatingFromColor)
    return;
  updatingFromColor = true;

  currentColor.setColor(
      redSlider->value() / 255.0f, greenSlider->value() / 255.0f,
      blueSlider->value() / 255.0f, alphaSlider->value() / 255.0f);

  float h, s, v;
  rgbToHSV(currentColor.r(), currentColor.g(), currentColor.b(), h, s, v);
  hueSlider->setValue(static_cast<int>(h));
  saturationSlider->setValue(static_cast<int>(s * 100));
  valueSlider->setValue(static_cast<int>(v * 100));

  updateHexFromColor();
  updatingFromColor = false;
}

void FloatColorPicker::Impl::updateColorFromHSVSliders() {
  if (updatingFromColor)
    return;
  updatingFromColor = true;

  float h = hueSlider->value();
  float s = saturationSlider->value() / 100.0f;
  float v = valueSlider->value() / 100.0f;

  float r, g, b;
  hsvToRGB(h, s, v, r, g, b);

  currentColor.setColor(r, g, b, currentColor.a());

  redSlider->setValue(static_cast<int>(r * 255));
  greenSlider->setValue(static_cast<int>(g * 255));
  blueSlider->setValue(static_cast<int>(b * 255));

  updateHexFromColor();
  updatingFromColor = false;
}

void FloatColorPicker::Impl::updateHexFromColor() {
  int r = static_cast<int>(currentColor.r() * 255);
  int g = static_cast<int>(currentColor.g() * 255);
  int b = static_cast<int>(currentColor.b() * 255);
  int a = static_cast<int>(currentColor.a() * 255);

  if (a < 255) {
    hexInput->setText(QString("%1%2%3%4")
                          .arg(r, 2, 16, QChar('0'))
                          .arg(g, 2, 16, QChar('0'))
                          .arg(b, 2, 16, QChar('0'))
                          .arg(a, 2, 16, QChar('0')));
  } else {
    hexInput->setText(QString("%1%2%3")
                          .arg(r, 2, 16, QChar('0'))
                          .arg(g, 2, 16, QChar('0'))
                          .arg(b, 2, 16, QChar('0')));
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
  setMinimumSize(450, 500);

  impl_->colorWheel = new ColorWheelWidget(this);
  impl_->colorPreview = new ColorViewLabel();

  // カラーホイールとプレビューのレイアウト
  QHBoxLayout *topLayout = new QHBoxLayout();
  topLayout->addWidget(impl_->colorWheel, 2);
  topLayout->addWidget(impl_->colorPreview, 1);

  // HEX入力
  QHBoxLayout *hexLayout = new QHBoxLayout();
  hexLayout->addWidget(impl_->hexLabel);
  hexLayout->addWidget(impl_->hexInput);
  hexLayout->addStretch();

  // RGBAスライダーグループ
  QGroupBox *rgbaGroup = new QGroupBox("RGBA", this);
  QHBoxLayout *redLayout = new QHBoxLayout();
  redLayout->addWidget(impl_->redLabel);
  redLayout->addWidget(impl_->redSlider);

  QHBoxLayout *greenLayout = new QHBoxLayout();
  greenLayout->addWidget(impl_->greenLabel);
  greenLayout->addWidget(impl_->greenSlider);

  QHBoxLayout *blueLayout = new QHBoxLayout();
  blueLayout->addWidget(impl_->blueLabel);
  blueLayout->addWidget(impl_->blueSlider);

  QHBoxLayout *alphaLayout = new QHBoxLayout();
  alphaLayout->addWidget(impl_->alphaLabel);
  alphaLayout->addWidget(impl_->alphaSlider);

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
  hueLayout->addWidget(impl_->hueSlider);

  QHBoxLayout *satLayout = new QHBoxLayout();
  satLayout->addWidget(impl_->saturationLabel);
  satLayout->addWidget(impl_->saturationSlider);

  QHBoxLayout *valLayout = new QHBoxLayout();
  valLayout->addWidget(impl_->valueLabel);
  valLayout->addWidget(impl_->valueSlider);

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
  connect(impl_->colorWheel, &ColorWheelWidget::colorChanged, this,
          [this](const ArtifactCore::FloatColor &color) {
            impl_->currentColor = color;
            impl_->updateAllFromColor();
            impl_->colorPreview->setColor(color);
            emit colorChanged(color);
          });

  connect(impl_->redSlider, &QSlider::valueChanged, this,
          [this]() { impl_->updateColorFromRGBSliders(); });
  connect(impl_->greenSlider, &QSlider::valueChanged, this,
          [this]() { impl_->updateColorFromRGBSliders(); });
  connect(impl_->blueSlider, &QSlider::valueChanged, this,
          [this]() { impl_->updateColorFromRGBSliders(); });
  connect(impl_->alphaSlider, &QSlider::valueChanged, this,
          [this]() { impl_->updateColorFromRGBSliders(); });

  connect(impl_->hueSlider, &QSlider::valueChanged, this,
          [this]() { impl_->updateColorFromHSVSliders(); });
  connect(impl_->saturationSlider, &QSlider::valueChanged, this,
          [this]() { impl_->updateColorFromHSVSliders(); });
  connect(impl_->valueSlider, &QSlider::valueChanged, this,
          [this]() { impl_->updateColorFromHSVSliders(); });

  connect(impl_->hexInput, &QLineEdit::textChanged, this,
          [this]() { impl_->updateColorFromHex(); });

  connect(impl_->okButton, &QPushButton::clicked, this, &QDialog::accept);
  connect(impl_->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

  // プリセットボタンの接続
  const char *presetColors[] = {"#FF0000", "#FF8000", "#FFFF00", "#80FF00",
                                "#00FF00", "#00FF80", "#00FFFF", "#0080FF",
                                "#0000FF", "#8000FF", "#FF00FF", "#FF0080",
                                "#FFFFFF", "#C0C0C0", "#808080", "#000000"};

  for (int i = 0; i < 16; ++i) {
    connect(impl_->presetButtons[i], &QPushButton::clicked, this,
            [this, presetColors, i]() {
              QString hex = presetColors[i];
              bool ok;
              int r = hex.mid(1, 2).toInt(&ok, 16);
              int g = hex.mid(3, 2).toInt(&ok, 16);
              int b = hex.mid(5, 2).toInt(&ok, 16);
              impl_->currentColor.setColor(r / 255.0f, g / 255.0f, b / 255.0f,
                                           impl_->currentColor.a());
              impl_->updateAllFromColor();
              impl_->colorWheel->setColor(impl_->currentColor);
              impl_->colorPreview->setColor(impl_->currentColor);
              emit colorChanged(impl_->currentColor);
            });
  }

  setStyleSheet(R"(
   FloatColorPicker {
    background-color: #2D2D30;
    color: #D4D4D4;
   }
   QGroupBox {
    border: 1px solid #3E3E42;
    border-radius: 4px;
    margin-top: 8px;
    font-weight: bold;
   }
   QGroupBox::title {
    subcontrol-origin: margin;
    left: 10px;
    padding: 0 3px;
   }
   QSlider::groove:horizontal {
    border: 1px solid #3E3E42;
    height: 4px;
    background: #1E1E1E;
    border-radius: 2px;
   }
   QSlider::handle:horizontal {
    background: #0E639C;
    border: 1px solid #0E639C;
    width: 14px;
    margin: -5px 0;
    border-radius: 7px;
   }
   QPushButton {
    background-color: #3E3E42;
    border: 1px solid #3E3E42;
    color: #D4D4D4;
    padding: 6px 12px;
    border-radius: 4px;
   }
   QPushButton:hover {
    background-color: #4E4E52;
   }
   QPushButton:pressed {
    background-color: #2D2D30;
   }
   QLineEdit {
    background-color: #1E1E1E;
    border: 1px solid #3E3E42;
    color: #D4D4D4;
    padding: 4px;
    border-radius: 2px;
   }
  )");
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

void FloatColorPicker::accept() { QDialog::accept(); }

void FloatColorPicker::reject() { QDialog::reject(); }

} // namespace ArtifactWidgets