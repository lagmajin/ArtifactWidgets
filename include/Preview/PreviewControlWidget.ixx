module;
#include <QWidget>
#include <wobjectdefs.h>
export module Preview.ControlWidget;

export namespace ArtifactCore {

 class PreviewControlWidget :public QWidget{
  W_OBJECT(PreviewControlWidget)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit PreviewControlWidget(QWidget* parent = nullptr);
  ~PreviewControlWidget();
  void play();
  void stop();
  
  W_SLOT(play)

  void playButtonClicked();
  //W_SIGNAL(playButtonClicked, )
 };







};