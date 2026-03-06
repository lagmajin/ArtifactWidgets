module;
#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <wobjectimpl.h>
#include <QColorDialog>
module FloatColorPickerDialog;

import Color.Float;
import Widgets.Dialog.Abstract;
import Widgets.ColorWheel;
import Widgets.ColorViewLabel;

namespace ArtifactWidgets {

 class FloatColorPicker::Impl
 {
 public:
  Impl();
  ~Impl();

  ColorFloat currentColor;
  ColorWheelWidget* colorWheel = nullptr;
  ColorViewLabel* colorPreview = nullptr;
  QSlider* redSlider;
  QSlider* greenSlider;
  QSlider* blueSlider;
  QSlider* alphaSlider;
  QLabel* redLabel;
  QLabel* greenLabel;
  QLabel* blueLabel;
  QLabel* alphaLabel;
  QPushButton* okButton;
  QPushButton* cancelButton;
  QPushButton* moreColorsButton;

  void updateSlidersFromColor();
  void updateColorFromSliders();
 };

 FloatColorPicker::Impl::Impl()
  : redSlider(new QSlider(Qt::Horizontal))
  , greenSlider(new QSlider(Qt::Horizontal))
  , blueSlider(new QSlider(Qt::Horizontal))
  , alphaSlider(new QSlider(Qt::Horizontal))
  , redLabel(new QLabel("R"))
  , greenLabel(new QLabel("G"))
  , blueLabel(new QLabel("B"))
  , alphaLabel(new QLabel("A"))
  , okButton(new QPushButton("OK"))
  , cancelButton(new QPushButton("Cancel"))
  , moreColorsButton(new QPushButton("More Colors..."))
 {
  redSlider->setRange(0, 255);
  greenSlider->setRange(0, 255);
  blueSlider->setRange(0, 255);
  alphaSlider->setRange(0, 255);

  redSlider->setValue(255);
  greenSlider->setValue(255);
  blueSlider->setValue(255);
  alphaSlider->setValue(255);
 }

 FloatColorPicker::Impl::~Impl()
 {

 }

 void FloatColorPicker::Impl::updateSlidersFromColor()
 {
  redSlider->setValue(static_cast<int>(currentColor.r * 255.0f));
  greenSlider->setValue(static_cast<int>(currentColor.g * 255.0f));
  blueSlider->setValue(static_cast<int>(currentColor.b * 255.0f));
  alphaSlider->setValue(static_cast<int>(currentColor.a * 255.0f));
 }

 void FloatColorPicker::Impl::updateColorFromSliders()
 {
  currentColor.r = redSlider->value() / 255.0f;
  currentColor.g = greenSlider->value() / 255.0f;
  currentColor.b = blueSlider->value() / 255.0f;
  currentColor.a = alphaSlider->value() / 255.0f;
 }

 FloatColorPicker::FloatColorPicker(QWidget* parent)
  : AbstractDialog(parent)
  , impl_(new Impl())
 {
  setWindowTitle("Color Picker");
  setMinimumSize(400, 350);

  impl_->colorWheel = new ColorWheelWidget(this);
  impl_->colorPreview = new ColorViewLabel(this);

  QGroupBox* sliderGroup = new QGroupBox("RGBA", this);

  QHBoxLayout* redLayout = new QHBoxLayout();
  redLayout->addWidget(impl_->redLabel);
  redLayout->addWidget(impl_->redSlider);

  QHBoxLayout* greenLayout = new QHBoxLayout();
  greenLayout->addWidget(impl_->greenLabel);
  greenLayout->addWidget(impl_->greenSlider);

  QHBoxLayout* blueLayout = new QHBoxLayout();
  blueLayout->addWidget(impl_->blueLabel);
  blueLayout->addWidget(impl_->blueSlider);

  QHBoxLayout* alphaLayout = new QHBoxLayout();
  alphaLayout->addWidget(impl_->alphaLabel);
  alphaLayout->addWidget(impl_->alphaSlider);

  QVBoxLayout* sliderLayout = new QVBoxLayout();
  sliderLayout->addLayout(redLayout);
  sliderLayout->addLayout(greenLayout);
  sliderLayout->addLayout(blueLayout);
  sliderLayout->addLayout(alphaLayout);
  sliderGroup->setLayout(sliderLayout);

  QHBoxLayout* buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(impl_->moreColorsButton);
  buttonLayout->addStretch();
  buttonLayout->addWidget(impl_->okButton);
  buttonLayout->addWidget(impl_->cancelButton);

  QHBoxLayout* topLayout = new QHBoxLayout();
  topLayout->addWidget(impl_->colorWheel, 2);
  topLayout->addWidget(impl_->colorPreview, 1);

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->addLayout(topLayout);
  mainLayout->addWidget(sliderGroup);
  mainLayout->addLayout(buttonLayout);

  connect(impl_->redSlider, &QSlider::valueChanged, this, [this]() {
   impl_->updateColorFromSliders();
   emit colorChanged(impl_->currentColor);
  });
  connect(impl_->greenSlider, &QSlider::valueChanged, this, [this]() {
   impl_->updateColorFromSliders();
   emit colorChanged(impl_->currentColor);
  });
  connect(impl_->blueSlider, &QSlider::valueChanged, this, [this]() {
   impl_->updateColorFromSliders();
   emit colorChanged(impl_->currentColor);
  });
  connect(impl_->alphaSlider, &QSlider::valueChanged, this, [this]() {
   impl_->updateColorFromSliders();
   emit colorChanged(impl_->currentColor);
  });

  connect(impl_->okButton, &QPushButton::clicked, this, &QDialog::accept);
  connect(impl_->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

  connect(impl_->moreColorsButton, &QPushButton::clicked, this, [this]() {
   QColorDialog dialog(this);
   dialog.setCurrentColor(QColor(
    static_cast<int>(impl_->currentColor.r * 255),
    static_cast<int>(impl_->currentColor.g * 255),
    static_cast<int>(impl_->currentColor.b * 255),
    static_cast<int>(impl_->currentColor.a * 255)
   ));
   if (dialog.exec() == QDialog::Accepted) {
    QColor color = dialog.currentColor();
    impl_->currentColor.r = color.redF();
    impl_->currentColor.g = color.greenF();
    impl_->currentColor.b = color.blueF();
    impl_->currentColor.a = color.alphaF();
    impl_->updateSlidersFromColor();
    emit colorChanged(impl_->currentColor);
   }
  });

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
  )");
 }

 FloatColorPicker::~FloatColorPicker()
 {
  delete impl_;
 }

 ColorFloat FloatColorPicker::getColor() const
 {
  return impl_->currentColor;
 }

 void FloatColorPicker::setColor(const ColorFloat& color)
 {
  impl_->currentColor = color;
  impl_->updateSlidersFromColor();
 }

 void FloatColorPicker::accept()
 {
  QDialog::accept();
 }

 void FloatColorPicker::reject()
 {
  QDialog::reject();
 }

}
