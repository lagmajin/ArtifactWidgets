module;
#include <wobjectimpl.h>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QEvent>

#include <OpenImageIO/imagebuf.h>
#include <QDragEnterEvent>
#include <QDropEvent>
#include "qevent.h"
#include <QMimeData>
//#include "../../../include/Image/BasicImageViewWidget.hpp"
module BasicImageViewWidget;





namespace ArtifactWidgets {

 W_OBJECT_IMPL(BasicImageViewWidget)

 class BasicImageViewWidgetPrivate {

 };

	struct BasicImageViewWidget::Impl
	{
		
	};

 BasicImageViewWidget::BasicImageViewWidget(QWidget*parent):QGraphicsView(parent)
 {
  setAcceptDrops(true);
 
 }

 BasicImageViewWidget::~BasicImageViewWidget()
 {

 }

 void BasicImageViewWidget::Clear()
 {

 }

 void BasicImageViewWidget::setImage(const QImage& image)
 {

 }

 void BasicImageViewWidget::wheelEvent(QWheelEvent* event)
 {

 }

 void BasicImageViewWidget::dragEnterEvent(QDragEnterEvent* event)
 {
  if (event->mimeData()->hasUrls()) {
   event->acceptProposedAction();
  }
 }

 void BasicImageViewWidget::dropEvent(QDropEvent* event)
 {

 }

};