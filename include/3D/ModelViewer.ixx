module;

#include <QWindow>
#include "..\Define\DllExportMacro.hpp"
export module ModelViewer;

export namespace ArtifactWidgets {

 class LIBRARY_DLL_API ModelViewer:public QWindow {
 private:
  class Impl;
  Impl* impl_;
 public:
  ModelViewer();
  ~ModelViewer();
};






};