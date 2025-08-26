﻿//#include "../../include/Common/AbstractDialog.hpp"

module;
//#include <wobjectdefs.h>
#include <wobjectimpl.h>
#include <QDialog>
#include <QMouseEvent>
module AbstractDialog;




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
 };


 AbstractDialog::AbstractDialog(QWidget* parent/*=nullptr*/) :QDialog(parent)
 {
  setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
 }

 AbstractDialog::~AbstractDialog()
 {
  delete impl_;
 }

 void AbstractDialog::keyPressEvent(QKeyEvent* event)
 {
  if (event->key() == Qt::Key_Escape) {
   reject(); // Cancel 相当
  }
  else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
   accept(); // OK 相当
  }
  else {
   QDialog::keyPressEvent(event);
  }
 }

 void AbstractDialog::mousePressEvent(QMouseEvent* event)
 {
  if (event->button() == Qt::LeftButton) {
   impl_->dragging = true;
   impl_->dragPos = event->globalPos() - frameGeometry().topLeft();
  }
 }

 void AbstractDialog::mouseMoveEvent(QMouseEvent* event)
 {
  if (impl_->dragging && (event->buttons() & Qt::LeftButton)) {
   move(event->globalPos() - impl_->dragPos);
  }
 }

 void AbstractDialog::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
   impl_->dragging = false;
  }
 }

 void AbstractDialog::setAcceptOnEnter(bool enabled /*= true*/)
 {

 }

 void AbstractDialog::setRejectOnEscape(bool enabled /*= true*/)
 {

 }

};

