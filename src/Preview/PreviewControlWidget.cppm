module;
#include <QWidget>
#include <QPushButton>
#include <wobjectimpl.h>
module Preview.ControlWidget;


namespace ArtifactWidgets {

 W_OBJECT_IMPL(PreviewControlWidget)

 class PreviewControlWidget::Impl {
 private:
 
 public:
  Impl();
  ~Impl();
  QPushButton* playButton_=nullptr;
  QPushButton* stopButton_=nullptr;
  QPushButton* backForward_ = nullptr;
 };

 PreviewControlWidget::Impl::Impl()
 {

 }

 PreviewControlWidget::Impl::~Impl()
 {

 }

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