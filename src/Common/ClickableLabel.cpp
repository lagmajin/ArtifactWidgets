module;
#include <QLabel>
#include <QMouseEvent>
#include <wobjectimpl.h>
module Common.ClikableLabel;


namespace ArtifactWidgets
{
 W_OBJECT_IMPL(ClickableLabel)

 void ClickableLabel::mousePressEvent(QMouseEvent*)
 {
  clicked();
 }

};
