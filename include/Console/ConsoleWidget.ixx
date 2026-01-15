module;

#include <QString>
#include <QPlainTextEdit>

export module ConsoleWidget;



export namespace ArtifactWidgets {

 class ConsoleWidgetPrivate;

 class ConsoleWidget : public QPlainTextEdit {
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit ConsoleWidget(QWidget* parent = nullptr);
  ~ConsoleWidget();
  void user();
  void addpendLog();
   
 };



}