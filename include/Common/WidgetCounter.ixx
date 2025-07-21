module;

#include <QtCore/QObject>
#include <wobjectcpp.h>
//import <memory>;


export module WidgetCounter;


export namespace ArtifactWidgets {

 class WidgetCounter : public QObject
 {
  W_OBJECT(WidgetCounter)
 private:
  class Impl;
  Impl* impl_;

  
 public:
  explicit WidgetCounter();
  ~WidgetCounter();
  static WidgetCounter& getInstance();
 };





}

//export void MyFunc();