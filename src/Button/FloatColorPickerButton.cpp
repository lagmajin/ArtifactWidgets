module;
#include <QPushButton>
module FloatColorPickerButton;

import Color.Float;
import FloatColorPickerDialog;

namespace ArtifactWidgets
{
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

 FloatColorPickerButton::FloatColorPickerButton(QWidget* parent /*= nullptr*/) :QPushButton(parent)
 {
 	

 }

 FloatColorPickerButton::~FloatColorPickerButton()
 {
  delete impl_;
 }

};