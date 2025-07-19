module;
#include <QWidget>
export module Widget.Knob.Slider;

export namespace ArtifactCore {

 class KnobSlider :public QWidget{
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit KnobSlider(QWidget* parent = nullptr);
  virtual ~KnobSlider();
 };






};