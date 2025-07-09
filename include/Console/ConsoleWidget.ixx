
module;

#include <wobjectdefs.h>
//
// #include <QtWidgets/QWidget>
#include <QPlainTextEdit>

export module ConsoleWidget;



export namespace ArtifactWidgets {

 class ConsoleWidgetPrivate;

 class ConsoleWidget : public QPlainTextEdit {
 private:

 public:
  explicit ConsoleWidget(QWidget* parent = nullptr);
  ~ConsoleWidget();
  void user();
 };



}