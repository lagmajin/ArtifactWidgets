module;
#include <wobjectimpl.h>
#include <QGraphicsView>
#include <QGraphicsScene>
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

};