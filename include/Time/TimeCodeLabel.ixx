module;
#include <QLabel>
export module Widgets.TimeCodeLabel;

import std;
import Frame.Position;


export namespace ArtifactWidgets {


 class TimeCodeLabel:public QLabel {
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit TimeCodeLabel(QWidget *parent = nullptr);
  ~TimeCodeLabel();

 };

};
