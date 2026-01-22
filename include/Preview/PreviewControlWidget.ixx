module;
#include <QWidget>
#include <wobjectdefs.h>
export module Preview.ControlWidget;

export namespace ArtifactWidgets {

 class PreviewControlWidget :public QWidget{
  W_OBJECT(PreviewControlWidget)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit PreviewControlWidget(QWidget* parent = nullptr);
  virtual ~PreviewControlWidget();
  void play();
  void stop();
  void seekStart();     // 最初のフレームへ
  void seekEnd();       // 最後のフレームへ（任意）
  void stepForward();   // 1フレーム進む
  void stepBackward();
  void setLoopEnabled(bool);
  void setPreviewRange(int start, int end);
 public/*signals*/:
 	void playButtonClicked() W_SIGNAL(playButtonClicked)
 	void stopButtonClicked() W_SIGNAL(stopButtonClicked)
 public/*slots*/:
 	
  W_SLOT(play)
 	W_SLOT(stop)


 };







};