module;
#include <QPushButton>
module Preview.ControlWidget;


namespace ArtifactCore {
 class PreviewControlWidget::Impl {
 private:
  QPushButton playButton;
  QPushButton stopButton;
 public:
  Impl();
  ~Impl();
 };



 PreviewControlWidget::PreviewControlWidget(QWidget* parent /*= nullptr*/)
 {

 }

 PreviewControlWidget::~PreviewControlWidget()
 {

 }
};