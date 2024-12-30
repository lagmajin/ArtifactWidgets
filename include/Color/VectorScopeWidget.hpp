#pragma once

#include <QtCore/QSharedPointer>

#include <QtWidgets/QWidget>


#include "../Common/AbstractWidget.hpp"





namespace Artifact {

 class VectorScopeWidgetPrivate;

 class VectorScopeWidget :public QWidget{
 private:
  VectorScopeWidgetPrivate* pWidget_;
 public:
  explicit VectorScopeWidget(QWidget* parent = nullptr);
  virtual ~VectorScopeWidget();
 signals:
  void updateFrameSucceeded();
 public slots:

 };






}