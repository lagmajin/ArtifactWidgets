module;
#include <QWidget>
export module IClonableWidget;

export namespace ArtifactWidgets
{

 class ICloneableWidget {
 public:
  virtual ~ICloneableWidget() = default;
  virtual QWidget* clone() const = 0;
 };








};