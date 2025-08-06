module;

#include <QWidget>
#include <QTreeView>
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>
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
  
 public:
  Impl();
  ~Impl();
  RenderQueueManagerJobPanel* jobPanel_=nullptr;
 };

 RenderQueueManagerWidget::Impl::Impl()
 {

 }

 RenderQueueManagerWidget::Impl::~Impl()
 {

 }


 RenderQueueManagerWidget::RenderQueueManagerWidget(QWidget* parent /*= nullptr*/):QWidget(parent),impl_(new Impl())
 {
  setEnabled(false);
  auto style = getDCCStyleSheetPreset(DccStylePreset::ModoStyle);

  setStyleSheet(style);
  auto& manager=RendererQueueManager::instance();

  //impl_->jobPanel_->

  impl_->jobPanel_ = new RenderQueueManagerJobPanel();
	
  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(impl_->jobPanel_);

  setLayout(layout);
 }

 RenderQueueManagerWidget::~RenderQueueManagerWidget()
 {
  delete impl_;
 }

 QSize RenderQueueManagerWidget::sizeHint() const
 {
  return QSize(500, QWidget::sizeHint().height());
 }

 void RenderQueueManagerWidget::setFloatingMode(bool isFloating)
 {

 }

};