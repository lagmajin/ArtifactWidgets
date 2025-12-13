
module;
#include <QSpinBox>
#include "../Define/DllExportMacro.hpp"
export module DragSpinBox;

export namespace ArtifactWidgets
{
 class WIDGET_LIBRARY_DLL_API DragSpinBox : public QSpinBox
 {
 private:
  class Impl;
  Impl* impl_;
 protected:
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
 public:
  explicit DragSpinBox(QWidget* parent = nullptr);
  ~DragSpinBox();
 };

 class WIDGET_LIBRARY_DLL_API DoubleDragSpinBox :public QDoubleSpinBox
 {
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit DoubleDragSpinBox(QWidget* parent = nullptr);
  ~DoubleDragSpinBox();
 };


};