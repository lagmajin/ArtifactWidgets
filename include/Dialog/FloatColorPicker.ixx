module;
#include <QDialog>
#include <wobjectdefs.h>
#include "../Define/DllExportMacro.hpp"

export module FloatColorPickerDialog;

import Widgets.Common.Abstract;
import Color.Float;

export namespace ArtifactWidgets
{

 class FloatColorPicker : public AbstractDialog
 {
  W_OBJECT(FloatColorPicker)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit FloatColorPicker(QWidget* parent = nullptr);
  ~FloatColorPicker();

  ColorFloat getColor() const;
  void setColor(const ColorFloat& color);

 signals:
  void colorChanged(const ColorFloat& color) W_SIGNAL(colorChanged, color);
 public slots:
  void accept() override;
  void reject() override;
 };

}
