module;
#include <QWidget>
#include <wobjectdefs.h>

export module Widgets.Knob.Rotary;

import Widgets.Knob;



export namespace ArtifactCore {

 [[deprecated("Use Artifact PropertyEditor rotation controls instead")]]
 class  RotaryKnob:public QWidget {
  W_OBJECT(RotaryKnob)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit RotaryKnob(QWidget* parent = nullptr);
  ~RotaryKnob();
 };






};
