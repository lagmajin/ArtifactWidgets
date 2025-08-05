module;

#include <QWidget>
#include <QTreeView>
#include <QPushButton>
#include <QProgressBar>
#include <wobjectimpl.h>

module Widgets.Render.Queue;

import Render;
import Widgets.Utils.CSS;

namespace ArtifactCore{}

namespace ArtifactWidgets {

 using namespace ArtifactCore;

W_OBJECT_IMPL(RenderQueueManagerWidget)


RenderQueueManagerJobPanel::RenderQueueManagerJobPanel(QWidget* parent/*=nullptr*/):QTreeView(parent)
{
 auto style = getDCCStyleSheetPreset(DccStylePreset::ModoStyle);

 setStyleSheet(style);


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
  setEnabled(false);
  auto style = getDCCStyleSheetPreset(DccStylePreset::ModoStyle);

  setStyleSheet(style);
  auto& manager=RendererQueueManager::instance();

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