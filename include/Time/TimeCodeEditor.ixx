module;
#include <QWidget>
export module Widgets.TimeCodeEditor;

import Frame.Position;

export namespace ArtifactWidgets {


 class TimeCodeEditor {
 private:
  class Impl;
  Impl* impl_;
 public:

  explicit TimeCodeEditor(QWidget*parent=nullptr);
  ~TimeCodeEditor();
  void setFrame(int64_t frame, double fps);

  enum DisplayMode { TimeCode, Frames };
  void setDisplayMode(DisplayMode mode);
 };
};