module;
#include <wobjectimpl.h>
#include <QLabel>
#include <wobjectdefs.h>
#include "../Define/DllExportMacro.hpp"
export module Common.ClikableLabel;

export namespace ArtifactWidgets
{
 class WIDGET_LIBRARY_DLL_API ClickableLabel : public QLabel {
 	W_OBJECT(ClickableLabel)
 protected:
  void mousePressEvent(QMouseEvent*) override;
 public:
  using QLabel::QLabel;

  void clicked() W_SIGNAL(clicked)

 };

};
