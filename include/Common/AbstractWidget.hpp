#pragma once

#include <QtWidgets/QWidget>


namespace ArtifactWidgets {

 class AbstractWidgetPrivate;

 class AbstractWidget :public QWidget{
 private:

 public:
  explicit AbstractWidget(QWidget* parent = nullptr);
  virtual~AbstractWidget();
 };

 AbstractWidget::AbstractWidget(QWidget* parent /*= nullptr*/)
 {

 }

 AbstractWidget::~AbstractWidget()
 {

 }




};