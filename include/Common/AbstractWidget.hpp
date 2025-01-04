#pragma once

#include <QtCore/QScopedPointer>
#include <QtWidgets/QWidget>


namespace ArtifactWidgets {

 class AbstractWidgetPrivate;

 class AbstractWidget :public QWidget{
  Q_OBJECT
 private:

 public:
  explicit AbstractWidget(QWidget* parent = nullptr);
  virtual~AbstractWidget();
 public slots:

 };






};