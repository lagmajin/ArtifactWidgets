module;
#include <wobjectdefs.h>
#include <QPushButton>
#include "../Define/DllExportMacro.hpp"

export module FloatColorPickerButton;

import Color.Float;
import FloatColorPickerDialog;

export namespace ArtifactWidgets
{
 class WIDGET_LIBRARY_DLL_API FloatColorPickerButton :public QPushButton {
  W_OBJECT(FloatColorPickerButton)
 private:
  class Impl;
  Impl* impl_;
  void updateButtonStyle();
 signals:
  void colorChanged(const ArtifactCore::FloatColor& color) W_SIGNAL(colorChanged, color);
 public:
  explicit FloatColorPickerButton(QWidget* parent = nullptr);
  ~FloatColorPickerButton();
  ArtifactCore::FloatColor getColor() const;
  void setColor(const ArtifactCore::FloatColor& color);
 };


}
