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
  QPushButton* playButton_=nullptr;
  QPushButton* stopButton_=nullptr;
  QPushButton* backForward_ = nullptr;
 };



 PreviewControlWidget::PreviewControlWidget(QWidget* parent /*= nullptr*/):QWidget(parent),impl_(new Impl)
 {

 }

 PreviewControlWidget::~PreviewControlWidget()
 {
  delete impl_;
 }

 void PreviewControlWidget::play()
 {

 }

 void PreviewControlWidget::stop()
 {

 }

 void PreviewControlWidget::seekStart()
 {

 }

 void PreviewControlWidget::seekEnd()
 {

 }

 void PreviewControlWidget::stepForward()
 {

 }

 void PreviewControlWidget::stepBackward()
 {

 }

 void PreviewControlWidget::setLoopEnabled(bool)
 {

 }

 void PreviewControlWidget::setPreviewRange(int start, int end)
 {

 }

};