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

 public:
  explicit WidgetCounter();
  ~WidgetCounter();
 signals:

 public slots:

 };





}

//export void MyFunc();