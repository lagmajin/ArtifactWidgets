module;

#include <QWidget>
#include <QTreeView>
#include <QPushButton>
#include <QProgressBar>
#include <wobjectimpl.h>

module Widgets.Render.Queue;

import Render;


namespace ArtifactWidgets {

 using namespace ArtifactCore;

W_OBJECT_IMPL(RenderQueueManagerWidget)


RenderQueueManagerJobPanel::RenderQueueManagerJobPanel(QWidget* parent/*=nullptr*/):QTreeView(parent)
{
 //auto header = header();


}

RenderQueueManagerJobPanel::~RenderQueueManagerJobPanel()
{

}



 class RenderQueueManagerWidget::Impl {
 private:
  RenderQueueManagerJobPanel* jobPanel_;
 public:
  Impl();
  ~Impl();
 };

 RenderQueueManagerWidget::Impl::Impl()
 {

 }

 RenderQueueManagerWidget::Impl::~Impl()
 {

 }


 RenderQueueManagerWidget::RenderQueueManagerWidget(QWidget* parent /*= nullptr*/):QWidget(parent)
 {
  auto manager=RendererQueueManager::instance();

 }

 RenderQueueManagerWidget::~RenderQueueManagerWidget()
 {

 }

 QSize RenderQueueManagerWidget::sizeHint() const
 {
  return QSize(500, QWidget::sizeHint().height());
 }

 void RenderQueueManagerWidget::setFloatingMode(bool isFloating)
 {

 }

};