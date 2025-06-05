module;



#include <wobjectdefs.h>
export module ParadeScopeWidget;
import AbstractWidget;
export namespace ArtifactWidgets {



 class ParadeScopeWidgetPrivate;

 class ParadeScopeWidget :public AbstractWidget{
 W_OBJECT(ParadeScopeWidget)
 private:

 public:
  ParadeScopeWidget(QWidget* parent = nullptr);
  ~ParadeScopeWidget();
 };







};