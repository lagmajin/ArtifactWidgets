module;
#include <QPushButton>
module ColorPickerButton;

import Color.Float;

namespace ArtifactWidgets
{
 class ColorPickerButton::Impl
 {
 private:
 	
 public:
  Impl();
  ~Impl();
 };

 ColorPickerButton::Impl::Impl()
 {

 }

 ColorPickerButton::ColorPickerButton(QWidget* parent /*= nullptr*/) :QPushButton(parent)
 {

 }

 ColorPickerButton::~ColorPickerButton()
 {
  delete impl_;
 }

};