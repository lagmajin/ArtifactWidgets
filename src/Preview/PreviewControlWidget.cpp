module;
#include <QWidget>
#include <QPushButton>
module Preview.ControlWidget;


namespace ArtifactCore {


 class PreviewControlWidget::Impl {
 private:
 
 public:
  Impl();
  ~Impl();
  QPushButton* playButton=nullptr;
  QPushButton* stopButton=nullptr;

 };



 PreviewControlWidget::PreviewControlWidget(QWidget* parent /*= nullptr*/):QWidget(parent)
 {

 }

 PreviewControlWidget::~PreviewControlWidget()
 {

 }

 void PreviewControlWidget::play()
 {

 }

 void PreviewControlWidget::stop()
 {

 }

};