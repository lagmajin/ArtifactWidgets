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
 };

 void FloatColorPickerButton::Impl::handleShowDialogRequest()
 {
  //auto dialog=new 
 	
 }

 FloatColorPickerButton::Impl::Impl()
 {

 }

 FloatColorPickerButton::Impl::~Impl()
 {

 }

 FloatColorPickerButton::FloatColorPickerButton(QWidget* parent /*= nullptr*/) :QPushButton(parent)
 {
 	

 }

 FloatColorPickerButton::~FloatColorPickerButton()
 {
  delete impl_;
 }

};