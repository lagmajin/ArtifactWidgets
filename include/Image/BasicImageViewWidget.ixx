module;
#include <wobjectimpl.h>

#include <QImage>
#include <QWidget>
#include <QGraphicsView>
#include <QtCore/QSharedPointer>
//#include <opencv2/opencv.hpp>


export module BasicImageViewWidget;
import AbstractWidget;
import IViewer;


export namespace ArtifactWidgets {

 class BasicImageViewWidgetPrivate;

 class BasicImageViewWidget :public QGraphicsView{
  W_OBJECT(BasicImageViewWidget)
 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
 protected:
  void wheelEvent(QWheelEvent* event) override;
 public:
  explicit BasicImageViewWidget(QWidget*parent=nullptr);
  ~BasicImageViewWidget();
 //signals:
  void updateFrameSucceeded();

 //public slots:
  void Clear();
  W_SLOT(Clear)
  void setImage(const QImage& image);
  W_SLOT(setImage)
  //void setImage(const cv::Mat& image);
 	void setFromFile();


 };






}