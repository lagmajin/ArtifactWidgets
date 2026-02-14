module;
#include "../Define/DllExportMacro.hpp"
#include <DockWidget.h>
#include <qboxlayout.h>
#include <QLabel>
#include <QPushButton>
#include <wobjectdefs.h>
#include <wobjectimpl.h>


export module DockWidget;





//#pragma comment(lib,"qtadvanceddockingd.lib")

export namespace ArtifactWidgets {

 class WIDGET_LIBRARY_DLL_API Pane : public ads::CDockWidget {
  W_OBJECT(Pane)
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit Pane(const QString& title, QWidget* contentWidget, QWidget* parent = nullptr);
  ~Pane();
 };







};