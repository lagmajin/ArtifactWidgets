
module;

#include <wobjectcpp.h>

#include <QtCore/QScopedPointer>
#include <QtCore/QFileInfo>
#include <QtWidgets/QWidget>
#include "../Define/DllExportMacro.hpp"

//#include "../Define/DllExportMacro.hpp"

export module AbstractWidget;

export namespace ArtifactWidgets {

  class WIDGET_LIBRARY_DLL_API AbstractWidget :public QWidget{
  W_OBJECT(AbstractWidget)
 private:
  class Impl;
  Impl* impl_;
 protected:
  void focusInEvent(QFocusEvent* event) override;
  void focusOutEvent(QFocusEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
 public:
  explicit AbstractWidget(QWidget* parent = nullptr);
  virtual~AbstractWidget();
  void setEnableFocusFrameGlow(bool isEnabled=true);

 };






};