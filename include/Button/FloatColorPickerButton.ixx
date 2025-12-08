module;
#include <wobjectdefs.h>
#include <QPushButton>

export module ColorPickerButton;

import Color.Float;

export namespace ArtifactWidgets
{
 class WIDGET_LIBRARY_DLL_API FloatColorPickerButton :public QPushButton {
  W_OBJECT(FloatColorPickerButton)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit FloatColorPickerButton(QWidget* parent = nullptr);
  ~FloatColorPickerButton();
 };


};
