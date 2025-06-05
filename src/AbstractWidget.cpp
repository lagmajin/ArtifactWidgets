//#include "../include/Common/AbstractWidget.hpp"

module;
#include <wobjectimpl.h>

module AbstractWidget;

import std;
namespace ArtifactWidgets {

 W_OBJECT_IMPL(AbstractWidget)

 class AbstractWidgetPrivate {
 private:

 
 public:
  AbstractWidgetPrivate();
  ~AbstractWidgetPrivate();
 };

 AbstractWidgetPrivate::AbstractWidgetPrivate()
 {

 }

 AbstractWidgetPrivate::~AbstractWidgetPrivate()
 {

 }

 AbstractWidget::AbstractWidget(QWidget* parent /*= nullptr*/) :QWidget(parent)
 {

 }

 AbstractWidget::~AbstractWidget()
 {

 }





}