module;

//import std.core;

//import std;
#include <wobjectdefs.h>
#include <QtWidgets/QCheckBox>

export module Widgets.Knob.CheckBox;





export namespace ArtifactWidgets {

 class KnobCheckBoxPrivate;

 class KnobCheckBox :public QCheckBox{
  W_OBJECT(KnobCheckBox);
 private:
  class Impl;
  Impl* d;
 public:
  explicit KnobCheckBox(QWidget* parent = nullptr);;
  ~KnobCheckBox();
  bool checkBox() const;
  void setValue(bool b);

 };







};