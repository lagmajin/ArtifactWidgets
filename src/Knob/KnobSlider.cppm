module;
#include <QWidget>
#include <QSlider>
#include <QMouseEvent>
#include <wobjectimpl.h>
module Widget.Knob.Slider;



namespace ArtifactCore {

 W_OBJECT_IMPL(KnobSlider)

class KnobSlider::Impl
{
public:
 QPoint dragStartPos;
 //float startValue = 0.0f;
 //float currentValue = 0.0f;
 //float step = 0.1f;
 //float minValue = 0.0f;
 //float maxValue = 1.0f;
};

 KnobSlider::KnobSlider(QWidget* parent /*= nullptr*/):QWidget(parent)
 {
  setMouseTracking(true);
 }

 KnobSlider::~KnobSlider()
 {

 }

 void KnobSlider::mousePressEvent(QMouseEvent* e)
 {
  impl_->dragStartPos = e->pos();
 }

 void KnobSlider::mouseMoveEvent(QMouseEvent* e)
 {

 }

 void KnobSlider::mouseReleaseEvent(QMouseEvent* e)
 {

 }

 void KnobSlider::wheelEvent(QWheelEvent* e)
 {

 }

 void KnobSlider::paintEvent(QPaintEvent* e)
 {

 }

};