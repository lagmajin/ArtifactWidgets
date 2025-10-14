module;
#include <QWidget>
#include <wobjectdefs.h>
export module Widget.Knob.Slider;

export namespace ArtifactCore {

 //W_OBJECT(KnobSlider)

 class KnobSlider :public QWidget{
  W_OBJECT(KnobSlider)
 private:
  class Impl;
  Impl* impl_;
 protected:
  void mousePressEvent(QMouseEvent* e) override;
  void mouseMoveEvent(QMouseEvent* e) override;
  void mouseReleaseEvent(QMouseEvent* e) override;
  void wheelEvent(QWheelEvent* e) override;
  void paintEvent(QPaintEvent* e) override;
 public:
  explicit KnobSlider(QWidget* parent = nullptr);
  virtual ~KnobSlider();
 };






};