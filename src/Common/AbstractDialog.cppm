//#include "../../include/Common/AbstractDialog.hpp"

module;
//#include <wobjectdefs.h>
#include <wobjectimpl.h>
#include <QDialog>
#include <QMouseEvent>
#include <QGraphicsEffect>
module Widgets.Dialog.Abstract;



namespace ArtifactWidgets {
 W_OBJECT_IMPL(AbstractDialog)

 
 class AbstractDialog::Impl {
 private:

 public:
  Impl();
  ~Impl();
  QPoint dragPos;
  bool dragging = false;
  bool acceptOnEnter = true;
  bool rejectOnEscape = true;
  bool highlightOnFocus = true;
  QGraphicsDropShadowEffect* shadowEffect = nullptr;
 };

 AbstractDialog::Impl::Impl()
 {

 }

 AbstractDialog::Impl::~Impl()
 {

 }

 AbstractDialog::AbstractDialog(QWidget* parent/*=nullptr*/) :QDialog(parent)
 {
  impl_ = new Impl();
  setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
 }

 AbstractDialog::~AbstractDialog()
 {
  delete impl_;
 }

 void AbstractDialog::keyPressEvent(QKeyEvent* event)
 {
  if (event->key() == Qt::Key_Escape && impl_->rejectOnEscape) {
   reject(); // Cancel 相当
   event->accept();
  }
  else if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) &&
           impl_->acceptOnEnter) {
   accept(); // OK 相当
   event->accept();
  }
  else {
   QDialog::keyPressEvent(event);
  }
 }

 void AbstractDialog::mousePressEvent(QMouseEvent* event)
 {
  if (event->button() == Qt::LeftButton) {
   impl_->dragging = true;
   impl_->dragPos = event->globalPosition().toPoint() - frameGeometry().topLeft();
   event->accept();
   return;
  }
  QDialog::mousePressEvent(event);
 }

 void AbstractDialog::mouseMoveEvent(QMouseEvent* event)
 {
  if (impl_->dragging && (event->buttons() & Qt::LeftButton)) {
   move(event->globalPosition().toPoint() - impl_->dragPos);
   event->accept();
   return;
  }
  QDialog::mouseMoveEvent(event);
 }

 void AbstractDialog::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
   impl_->dragging = false;
   event->accept();
   return;
  }
  QDialog::mouseReleaseEvent(event);
 }

 void AbstractDialog::setAcceptOnEnter(bool enabled /*= true*/)
 {
  impl_->acceptOnEnter = enabled;
 }

 void AbstractDialog::setRejectOnEscape(bool enabled /*= true*/)
 {
  impl_->rejectOnEscape = enabled;
 }

 void AbstractDialog::focusInEvent(QFocusEvent* event)
 {
  QDialog::focusInEvent(event);
 }

 void AbstractDialog::focusOutEvent(QFocusEvent* event)
 {
  QDialog::focusOutEvent(event);
 }

 void AbstractDialog::setAlwaysOnTop(bool enabled)
 {
  Qt::WindowFlags flags = windowFlags();

  if (enabled) {
   flags |= Qt::WindowStaysOnTopHint;
  }
  else {
   flags &= ~Qt::WindowStaysOnTopHint;
  }

  setWindowFlags(flags);

  // フラグ変更後は再表示が必須
  if (isVisible()) 
  {
   show();
  }
 }

 void AbstractDialog::setFrameless(bool enabled /*= true*/)
 {
  Qt::WindowFlags flags = windowFlags();

  if (enabled) {
   flags |= Qt::FramelessWindowHint;
  }
  else {
   flags &= ~Qt::FramelessWindowHint;
  }

  setWindowFlags(flags);

  // フラグ変更後は再表示しないと反映されない
  if (isVisible()) {
   show();
  }
 }

};

