module;
#include <QWidget>
#include <wobjectdefs.h>
export module Preview.ControlWidget;

export namespace ArtifactCore {

 class PreviewControlWidget {
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit PreviewControlWidget(QWidget* parent = nullptr);
  ~PreviewControlWidget();
 };







};