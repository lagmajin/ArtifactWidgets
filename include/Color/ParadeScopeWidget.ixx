module;



#include <wobjectdefs.h>
export module ParadeScopeWidget;
import AbstractWidget;
export namespace ArtifactWidgets {

	

 class ParadeScopeWidget :public AbstractWidget{
 W_OBJECT(ParadeScopeWidget)
 private:
  class Impl;
  Impl* impl_;
 protected:
 	
 public:
  ParadeScopeWidget(QWidget* parent = nullptr);
  ~ParadeScopeWidget();
 };







};