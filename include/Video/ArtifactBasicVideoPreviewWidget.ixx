module;

#include <wobjectcpp.h>
#include <wobjectdefs.h>

#include <QtCore/QtCore>
#include <QtWidgets/QWidget>

//#include "../Image/BasicImageViewWidget.hpp"

export module ArtifactBasicViedeoPreviewWidget;

import AbstractWidget;

export namespace ArtifactWidgets {

 class ArtifactBasicVideoPreviewWidgetPrivate;

 class ArtifactBasicViedeoPreviewWidget:public QWidget{
  W_OBJECT(ArtifactBasicViedeoPreviewWidget)
 private:

 public:
  explicit ArtifactBasicViedeoPreviewWidget();
  virtual ~ArtifactBasicViedeoPreviewWidget();
 };




};


