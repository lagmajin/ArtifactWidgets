module;
#include <wobjectimpl.h>

#include <QImage>
#include <QWidget>
#include <QGraphicsView>
#include <QtCore/QSharedPointer>
#include <OpenImageIO/imagebuf.h>
#include "../Define/DllExportMacro.hpp"
export module BasicImageViewWidget;
import AbstractWidget;
import IViewer;


export namespace ArtifactWidgets {

 

 class LIBRARY_DLL_API BasicImageViewWidget :public QGraphicsView{
  W_OBJECT(BasicImageViewWidget)
 private:
  class Impl;
  Impl* impl_;
 protected:
  void focusInEvent(QFocusEvent*) override;

  void focusOutEvent(QFocusEvent*) override;
  void wheelEvent(QWheelEvent* event) override;
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dropEvent(QDropEvent* event) override;

  void dragMoveEvent(QDragMoveEvent* event) override;


  void dragLeaveEvent(QDragLeaveEvent* event) override;

 public:
  explicit BasicImageViewWidget(QWidget*parent=nullptr);
  ~BasicImageViewWidget();
  void enableCheckerBoard();
  void soloMode();
  void setRotate(double angleDegrees);

  void resetRotate();
  void showContextMenu(const QPoint& pos);
  W_SLOT(showContextMenu, (const QPoint&));

  void updateFrameSucceeded();
  void Clear();
  W_SLOT(Clear)
  void setImage(const QImage& image);
  W_SLOT(setImage)
  //void setImage(const cv::Mat& image);
 	void setFromFile();



  QSize sizeHint() const override;

 };






}