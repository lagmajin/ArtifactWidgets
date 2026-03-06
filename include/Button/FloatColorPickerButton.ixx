module;
#include <wobjectdefs.h>
#include <QPushButton>
#include "../Define/DllExportMacro.hpp"
#include "../Dialog/FloatColorPicker.ixx"

export module FloatColorPickerButton;

import Color.Float;

export namespace ArtifactWidgets
{
 class WIDGET_LIBRARY_DLL_API FloatColorPickerButton :public QPushButton {
  W_OBJECT(FloatColorPickerButton)
 private:
  class Impl;
  Impl* impl_;
 signals:
  void colorChanged(const ColorFloat& color) W_SIGNAL(colorChanged, color);
 public:
  explicit FloatColorPickerButton(QWidget* parent = nullptr);
  ~FloatColorPickerButton();
  ColorFloat getColor() const;
  void setColor(const ColorFloat& color);
 };


}
