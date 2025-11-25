module;

#include <wobjectcpp.h>
#include <QtCore/QtCore>
#include <Qtwidgets/QWidget>

#include "../Define/DllExportMacro.hpp"

export module Widgets.Knob;
//export import VolumeSlider;



export namespace ArtifactWidgets {



 class WIDGET_LIBRARY_DLL_API AbstractKnobEditor:public QWidget {
  W_OBJECT(AbstractKnobEditor)
 private:
  class Impl;
  Impl* impl_;
 protected:
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dragMoveEvent(QDragMoveEvent* event) override;
  void dropEvent(QDropEvent* event) override;

 public:
  explicit AbstractKnobEditor(QWidget* parent = nullptr);
  virtual ~AbstractKnobEditor();
 };






}