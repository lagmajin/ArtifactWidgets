module ;
#include <QWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <wobjectdefs.h>
#include "../Define/DllExportMacro.hpp"
//#include "..\Define\DllExportMacro.hpp"


export module Widgets.Render.Queue;

import Render;

export namespace ArtifactWidgets {

 class RenderQueueControlPanel:public QWidget
 {
 private:
  class Impl;
  Impl* impl_;
 public:
  RenderQueueControlPanel(QWidget* parent = nullptr);
  ~RenderQueueControlPanel();
 };


 class RenderQueueManagerJobPanel:public QTreeView {
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit RenderQueueManagerJobPanel(QWidget*parent=nullptr);
  ~RenderQueueManagerJobPanel();
 };

 class WIDGET_LIBRARY_DLL_API RenderQueueManagerWidget :public QWidget {
  W_OBJECT(RenderQueueManagerWidget)
 private:
  class Impl;
  Impl* impl_;
 public:

  explicit RenderQueueManagerWidget(QWidget* parent = nullptr);
  ~RenderQueueManagerWidget();
  QSize sizeHint() const override;
  void setFloatingMode(bool isFloating);
 };

 class RenderQueueManagerUnderInfoWidget:public QWidget
 {
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit RenderQueueManagerUnderInfoWidget(QWidget* parent = nullptr);
  ~RenderQueueManagerUnderInfoWidget();
 };






};