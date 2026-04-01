module ;
#include <QWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QTimer>
#include <wobjectdefs.h>
#include "../Define/DllExportMacro.hpp"

export module Widgets.Render.Queue;

import Render;

export namespace ArtifactWidgets {

 class RenderQueueControlPanel:public QWidget
 {
  W_OBJECT(RenderQueueControlPanel)
 private:
  class Impl;
  Impl* impl_;
 public:
  RenderQueueControlPanel(QWidget* parent = nullptr);
  ~RenderQueueControlPanel();

  QProgressBar* progressBar() const;
  QPushButton* renderingStartButton() const;
  QPushButton* clearAllButton() const;

  void setRenderingState(bool rendering);
  void setTotalProgress(int percent);

 signals:
  void startRenderingClicked();
  void clearAllClicked();
 };


 class RenderQueueManagerJobPanel:public QTreeView {
  W_OBJECT(RenderQueueManagerJobPanel)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit RenderQueueManagerJobPanel(QWidget*parent=nullptr);
  ~RenderQueueManagerJobPanel();

  void setService(QObject* service);
  void refreshJobList();

 protected:
  void contextMenuEvent(QContextMenuEvent* event) override;
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dragMoveEvent(QDragMoveEvent* event) override;
  void dropEvent(QDropEvent* event) override;
  void startDrag(Qt::DropActions supportedActions) override;

 signals:
  void jobContextMenuRequested(int jobIndex, const QPoint& pos);
  void jobDropped(int fromIndex, int toIndex);
  void compositionDropped(const QString& compositionId);
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
  void setService(QObject* service);
 };

 class RenderQueueManagerUnderInfoWidget:public QWidget
 {
  W_OBJECT(RenderQueueManagerUnderInfoWidget)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit RenderQueueManagerUnderInfoWidget(QWidget* parent = nullptr);
  ~RenderQueueManagerUnderInfoWidget();

  void updateInfo(const QString& log, int ramMB, const QString& elapsedTime);
 };

};


 class RenderQueueManagerJobPanel:public QTreeView {
  W_OBJECT(RenderQueueManagerJobPanel)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit RenderQueueManagerJobPanel(QWidget*parent=nullptr);
  ~RenderQueueManagerJobPanel();

  void setService(QObject* service);
  void refreshJobList();

 protected:
  void contextMenuEvent(QContextMenuEvent* event) override;
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dragMoveEvent(QDragMoveEvent* event) override;
  void dropEvent(QDropEvent* event) override;
  void startDrag(Qt::DropActions supportedActions) override;

 signals:
  void jobContextMenuRequested(int jobIndex, const QPoint& pos);
  void jobDropped(int fromIndex, int toIndex);
  void compositionDropped(const QString& compositionId);
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
  void setService(QObject* service);
 };

 class RenderQueueManagerUnderInfoWidget:public QWidget
 {
  W_OBJECT(RenderQueueManagerUnderInfoWidget)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit RenderQueueManagerUnderInfoWidget(QWidget* parent = nullptr);
  ~RenderQueueManagerUnderInfoWidget();

  void updateInfo(const QString& log, int ramMB, const QString& elapsedTime);
 };

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