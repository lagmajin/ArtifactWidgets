#pragma once

#include <QtWidgets/QWidget>





namespace ArtifactWidgets {

 class HistgramWidgetPrivate;

 class HistgramWidget :public QWidget{
 private:

 protected:
  

  void paintEvent(QPaintEvent* event) override;
  
 public:
  explicit HistgramWidget(QWidget* parent = nullptr);
  virtual ~HistgramWidget();
  void setHistgram();

 };






};