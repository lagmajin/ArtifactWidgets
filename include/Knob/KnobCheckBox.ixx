module;

import std.core;

export module KnobCheckBox;





export namespace ArtifactWidgets {

 class KnobCheckBoxPrivate;

 class KnobCheckBox {
 private:

 public:
  KnobCheckBox();
  ~KnobCheckBox();
  bool checkBox() const;
  void setCheckBox(bool b);
 };







};