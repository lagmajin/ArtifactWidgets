//#include "../include/Common/AbstractWidget.hpp"

module;
#include <wobjectimpl.h>
#include <qevent.h>

module AbstractWidget;

import std;

import Widgets.Effects.GlowFrame;

namespace ArtifactWidgets {

 W_OBJECT_IMPL(AbstractWidget)

 class AbstractWidget::Impl {
 private:

 
 public:
  Impl();
  ~Impl();
  GlowFrame* glowFrame = nullptr;
 };

 AbstractWidget::Impl::Impl()
 {

 }

 AbstractWidget::Impl::~Impl()
 {

 }

 AbstractWidget::AbstractWidget(QWidget* parent /*= nullptr*/) :QWidget(parent),impl_(new Impl())
 {
  //impl_->glowFrame = new GlowFrame(this);
  //impl_->glowFrame->setGeometry(rect());
  //impl_->glowFrame->show();
 }

 AbstractWidget::~AbstractWidget()
 {
  delete impl_;
 }

 void AbstractWidget::focusInEvent(QFocusEvent* event)
 {
  //impl_->glowFrame->setFocused(true);
 }

 void AbstractWidget::focusOutEvent(QFocusEvent* event)
 {
  //impl_->glowFrame->setFocused(false);
 }

 void AbstractWidget::resizeEvent(QResizeEvent* event)
 {
  //impl_->glowFrame->resize(event->size());
 }

 void AbstractWidget::setEnableFocusFrameGlow(bool isEnabled/*=true*/)
 {

 }

}