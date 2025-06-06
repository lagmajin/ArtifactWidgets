module;

#include <QtGui/QImage>
#include <QtWidgets/QWidget>

#include <QtCore/QSharedPointer>


export module BasicImageViewWidget;
import AbstractWidget;
import IViewer;
export namespace ArtifactWidgets {

 class BasicImageViewWidgetPrivate;

 class BasicImageViewWidget :public QWidget{
  //Q_OBJECT
 private:

 public:
  explicit BasicImageViewWidget();
  ~BasicImageViewWidget();
 //signals:
  void updateFrameSucceeded();

 //public slots:
  void Clear();
  void setImage(const QImage& image);
 };






}