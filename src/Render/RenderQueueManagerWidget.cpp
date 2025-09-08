module;

#include <QLabel>
#include <QWidget>
#include <QTreeView>
#include <QPushButton>
#include <QProgressBar>
#include <QVBoxLayout>
#include <wobjectimpl.h>

#include "../../../../../../../Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/ucrt/corecrt_io.h"

module Widgets.Render.Queue;

import Render;
import Widgets.Utils.CSS;


namespace ArtifactCore {}

namespace ArtifactWidgets {

 using namespace ArtifactCore;

 W_OBJECT_IMPL(RenderQueueManagerWidget)

  class RenderQueueControlPanel::Impl
 {

 public:
  Impl();
  //RenderQueueControlPanel* parent = nullptr;

  QProgressBar* progressBar = nullptr;
  QPushButton* renderingStartButton = nullptr;
  QPushButton* clearAllRenderQueueButton = nullptr;
 };

 RenderQueueControlPanel::Impl::Impl()
 {

 }

 RenderQueueControlPanel::RenderQueueControlPanel(QWidget* parent /*= nullptr*/) :QWidget(parent), impl_(new Impl())
 {
  auto pHVoxLayout = new QHBoxLayout();

  auto renderingStartButton = impl_->renderingStartButton = new QPushButton();
  renderingStartButton->setText("Rendering");
  auto clearAllRenderQueueButton = impl_->clearAllRenderQueueButton = new QPushButton();
  clearAllRenderQueueButton->setText("clear all");
  //renderingStartButton->setFixedSize(50, 50); // 正方形に
  renderingStartButton->setStyleSheet(
   "QPushButton {"
   " border-radius: 10px;"      // 角丸の半径
   " background-color: #3498db;"
   " color: white;"
   " font-weight: bold;"
   " }"
   "QPushButton:hover {"
   " background-color: #2980b9;"
   " }"
  );

  auto progressBar = impl_->progressBar = new QProgressBar();
  progressBar->setValue(10);


  
  pHVoxLayout->addWidget(progressBar);
  pHVoxLayout->addWidget(renderingStartButton);
  pHVoxLayout->addWidget(clearAllRenderQueueButton);
  setLayout(pHVoxLayout);
 }

 RenderQueueControlPanel::~RenderQueueControlPanel()
 {
  delete impl_;
 }

 class RenderQueueManagerJobPanel::Impl
 {
 private:

 public:
  Impl();
  ~Impl();
 };

 RenderQueueManagerJobPanel::Impl::~Impl()
 {

 }

 RenderQueueManagerJobPanel::RenderQueueManagerJobPanel(QWidget* parent/*=nullptr*/) :QTreeView(parent)
 {
  auto style = getDCCStyleSheetPreset(DccStylePreset::ModoStyle);

  setStyleSheet(style);

  auto model = new RenderJobModel();

  setModel(model);

 }

 RenderQueueManagerJobPanel::~RenderQueueManagerJobPanel()
 {

 }



 class RenderQueueManagerWidget::Impl {
 private:

 public:
  Impl();
  ~Impl();
  RenderQueueControlPanel* coontrolPanel_ = nullptr;
  RenderQueueManagerJobPanel* jobPanel_ = nullptr;
  RenderQueueManagerUnderInfoWidget* infoWidget = nullptr;
 };

 RenderQueueManagerWidget::Impl::Impl()
 {

 }

 RenderQueueManagerWidget::Impl::~Impl()
 {

 }


 RenderQueueManagerWidget::RenderQueueManagerWidget(QWidget* parent /*= nullptr*/) :QWidget(parent), impl_(new Impl())
 {
  setEnabled(false);
  auto style = getDCCStyleSheetPreset(DccStylePreset::ModoStyle);

  setStyleSheet(style);
  auto& manager = RendererQueueManager::instance();

  //impl_->jobPanel_->

  impl_->coontrolPanel_ = new RenderQueueControlPanel();

  auto info = impl_->infoWidget = new RenderQueueManagerUnderInfoWidget();

  impl_->jobPanel_ = new RenderQueueManagerJobPanel();
  impl_->jobPanel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  QVBoxLayout* layout = new QVBoxLayout();

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(impl_->coontrolPanel_);
  layout->addWidget(impl_->jobPanel_, 0, Qt::AlignTop);
  layout->addWidget(info);
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

 class RenderQueueManagerUnderInfoWidget::Impl
 {
 private:

 public:
  Impl();
  ~Impl();
  QLabel* logLabel = nullptr;
  QLabel* ramUsageLabel = nullptr;
  QLabel* elpsedTimeLabel = nullptr;
 };

 RenderQueueManagerUnderInfoWidget::Impl::Impl()
 {

 }

 RenderQueueManagerUnderInfoWidget::Impl::~Impl()
 {

 }

 RenderQueueManagerUnderInfoWidget::RenderQueueManagerUnderInfoWidget(QWidget* parent /*= nullptr*/):QWidget(parent),impl_(new Impl())
 {
  QHBoxLayout* layout = new QHBoxLayout();

  auto logLabel = impl_->logLabel = new QLabel();
  logLabel->setText("Log:");

  auto ramUsageLabel=impl_->ramUsageLabel = new QLabel();
  ramUsageLabel->setText("Ram:");

  auto elpsedTimeLabel = impl_->elpsedTimeLabel = new QLabel();
  elpsedTimeLabel->setText("Elapsed Time");

  layout->addWidget(logLabel);
  layout->addWidget(ramUsageLabel);
  layout->addWidget(elpsedTimeLabel);
  setLayout(layout);
 }

 RenderQueueManagerUnderInfoWidget::~RenderQueueManagerUnderInfoWidget()
 {
  delete impl_;
 }

};