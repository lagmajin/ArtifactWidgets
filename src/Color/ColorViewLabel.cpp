module;
#include <QWidget>
#include <QPainter>
#include <wobjectimpl.h>
module Widget.ColorViewLabel;

import Color.Float;

namespace ArtifactWidgets {

 ColorViewLabel::ColorViewLabel(QWidget* parent)
  : QWidget(parent)
 {
  setMinimumSize(40, 40);
 }

 ColorViewLabel::~ColorViewLabel()
 {
 }

}
