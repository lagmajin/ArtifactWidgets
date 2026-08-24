module;
#include "../Define/DllExportMacro.hpp"
#include <QWidget>
#include <wobjectdefs.h>
#include <wobjectimpl.h>


export module DockWidget;





export namespace ArtifactWidgets {

 class WIDGET_LIBRARY_DLL_API Pane : public QWidget {
  W_OBJECT(Pane)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit Pane(const QString& title, QWidget* contentWidget, QWidget* parent = nullptr);
  ~Pane();
 };







};
