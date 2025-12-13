module;

#include <QMouseEvent>
module DragSpinBox;


namespace ArtifactWidgets {

 class DragSpinBox::Impl
 {
 	
 public:
  QPoint pressPos;
  int    startValue = 0;
  bool   dragging = false;
 };

 DragSpinBox::DragSpinBox(QWidget* parent /*= nullptr*/) :QSpinBox(parent),impl_(new Impl)
 {
  setAlignment(Qt::AlignRight);
  setMaximumWidth(100);
 }

 DragSpinBox::~DragSpinBox()
 {
  delete impl_;
 }

 void DragSpinBox::mouseMoveEvent(QMouseEvent* event)
 {
  impl_->pressPos = event->pos();
  QSpinBox::mousePressEvent(event);
 }

 void DragSpinBox::mousePressEvent(QMouseEvent* event)
 {
  //throw std::logic_error("The method or operation is not implemented.");
 }

 void DragSpinBox::mouseReleaseEvent(QMouseEvent* event)
 {
  //throw std::logic_error("The method or operation is not implemented.");
 }

 class DoubleDragSpinBox::Impl
 {
 public:
  Impl();

 };

 DoubleDragSpinBox::DoubleDragSpinBox(QWidget* parent /*= nullptr*/)
 {
  setAlignment(Qt::AlignRight);
  setMaximumWidth(100);
 }

 DoubleDragSpinBox::~DoubleDragSpinBox()
 {

 }

}