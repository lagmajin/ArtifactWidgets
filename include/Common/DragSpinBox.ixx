
module;
#include <QSpinBox>
#include "../Define/DllExportMacro.hpp"
export module DragSpinBox;

export namespace ArtifactWidgets
{
 class WIDGET_LIBRARY_DLL_API DragSpinBox : public QSpinBox
 {
 private:
 public:
  explicit DragSpinBox(QWidget* parent = nullptr);
  ~DragSpinBox();
 };

 class WIDGET_LIBRARY_DLL_API DoubleDragSpinBox :public QDoubleSpinBox
 {
 private:
 public:
  explicit DoubleDragSpinBox(QWidget* parent = nullptr);
  ~DoubleDragSpinBox();
 };


};