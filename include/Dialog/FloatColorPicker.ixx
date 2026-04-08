module;
#include <QDialog>
#include <wobjectdefs.h>
#include "../Define/DllExportMacro.hpp"

export module FloatColorPickerDialog;

import Widgets.Dialog.Abstract;
import Color.Float;

export namespace ArtifactWidgets
{

 class WIDGET_LIBRARY_DLL_API FloatColorPicker : public AbstractDialog
 {
  W_OBJECT(FloatColorPicker)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit FloatColorPicker(QWidget* parent = nullptr);
  ~FloatColorPicker();

  ArtifactCore::FloatColor getColor() const;
  void setColor(const ArtifactCore::FloatColor& color);
  void setInitialColor(const ArtifactCore::FloatColor& color);

 signals:
  void colorChanged(const ArtifactCore::FloatColor& color) W_SIGNAL(colorChanged, color);
 public slots:
  void accept() override;
  void reject() override;
 };

}

W_REGISTER_ARGTYPE(ArtifactCore::FloatColor)
