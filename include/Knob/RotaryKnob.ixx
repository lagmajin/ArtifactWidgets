module;
#include <QWidget>
#include <wobjectdefs.h>

export module Widgets.Knob.Rotary;

import Widgets.Knob;



export namespace ArtifactCore {

 class [[deprecated("Use Artifact PropertyEditor rotation controls instead")]] RotaryKnob:public QWidget {
  W_OBJECT(RotaryKnob)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit RotaryKnob(QWidget* parent = nullptr);
  ~RotaryKnob();
 };






};
