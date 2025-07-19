module;
#include <QWidget>

export module Widgets.Knob.Rotary;

import Widgets.Knob;



export namespace ArtifactCore {

 class RotaryKnob:public QWidget {
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit RotaryKnob(QWidget* parent = nullptr);
  ~RotaryKnob();
 };






};