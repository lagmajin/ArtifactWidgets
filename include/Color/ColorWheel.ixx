module;

#include <QWidget>

export module Widgets.ColorWheel;


export  namespace ArtifactWidgets
{

 class ColorWheelWidget : public QWidget
 {
 private:
  class Impl;
  Impl* impl_;
 protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
 public:
  explicit ColorWheelWidget(QWidget* parent = nullptr);
  ~ColorWheelWidget();
 


 };






}