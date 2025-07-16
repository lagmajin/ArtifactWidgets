module;

#include <wobjectcpp.h>
#include <wobjectdefs.h>

#include <QtCore/QtCore>
#include <QWidget>
#include <QGraphicsView>
//#include "../Image/BasicImageViewWidget.hpp"

export module Widgets.VideoPreview;

import AbstractWidget;

export namespace ArtifactWidgets {

 

 class ArtifactBasicVideoPreviewWidget:public QWidget{
  W_OBJECT(ArtifactBasicVideoPreviewWidget)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit ArtifactBasicVideoPreviewWidget(QWidget*parent=nullptr);
  virtual ~ArtifactBasicVideoPreviewWidget();
  void play();
  void stop();
 };




};


