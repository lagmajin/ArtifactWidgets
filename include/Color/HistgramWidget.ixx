module;
#include <QtWidgets/QWidget>

export module HistgramWidget;



export namespace ArtifactWidgets {



 class HistgramWidget :public QWidget{
 private:
  class Impl;
  Impl* impl_;
 protected:
  

  void paintEvent(QPaintEvent* event) override;
  
 public:
  explicit HistgramWidget(QWidget* parent = nullptr);
  virtual ~HistgramWidget();
  void setHistgram();

 };






};