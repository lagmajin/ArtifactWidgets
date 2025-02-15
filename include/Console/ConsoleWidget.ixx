
module;

#include <wobjectdefs.h>
#include <QtWidgets/QWidget>

export module ConsoleWidget;



export namespace ArtifactWidgets {

 class ConsoleWidgetPrivate;

 class ConsoleWidget : public QWidget {
 private:

 public:
  explicit ConsoleWidget(QWidget* parent = nullptr);
  ~ConsoleWidget();
  void user();
 };



}