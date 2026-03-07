module;
#include <QPushButton>
#include <wobjectimpl.h>
module FloatColorPickerButton;

import Color.Float;
import FloatColorPickerDialog;

namespace ArtifactWidgets
{
 W_OBJECT_IMPL(FloatColorPickerButton)

 class FloatColorPickerButton::Impl
 {
 private:

 public:
  Impl();
  ~Impl();
  void handleShowDialogRequest();
  ArtifactCore::FloatColor currentColor;
 };

 void FloatColorPickerButton::Impl::handleShowDialogRequest()
 {
  // FloatColorPicker dialog will be shown when button is clicked
  // This is handled in the button's click handler
 }

 FloatColorPickerButton::Impl::Impl()
 {
  currentColor = ArtifactCore::FloatColor(1.0f, 1.0f, 1.0f, 1.0f); // White by default
 }

 FloatColorPickerButton::Impl::~Impl()
 {

 }

 FloatColorPickerButton::FloatColorPickerButton(QWidget* parent /*= nullptr*/) 
  : QPushButton(parent)
  , impl_(new Impl())
 {
  setText("...");
  setFixedSize(30, 24);
  connect(this, &QPushButton::clicked, this, [this]() {
   FloatColorPicker picker(this);
   picker.setColor(impl_->currentColor);
   if (picker.exec() == QDialog::Accepted) {
    impl_->currentColor = picker.getColor();
    emit colorChanged(impl_->currentColor);
   }
  });
 }

 FloatColorPickerButton::~FloatColorPickerButton()
 {
  delete impl_;
 }

 ArtifactCore::FloatColor FloatColorPickerButton::getColor() const
 {
  return impl_->currentColor;
 }

 void FloatColorPickerButton::setColor(const ArtifactCore::FloatColor& color)
 {
  impl_->currentColor = color;
 }

}
