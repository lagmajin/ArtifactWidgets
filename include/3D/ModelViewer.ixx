module;

#include <QWindow>

#include "..\Define\DllExportMacro.hpp"
export module Widgets.ModelViewer;

export namespace ArtifactWidgets {

 class WIDGET_LIBRARY_DLL_API ModelViewer:public QWindow {
 private:
  class Impl;
  Impl* impl_;
 public:
  ModelViewer();
  ~ModelViewer();
};






};