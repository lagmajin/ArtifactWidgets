//#include <QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
//#include <QVBoxLayout>

import ConsoleWidget;




namespace ArtifactWidgets {

 class ConsoleWidgetPrivate {
  QPlainTextEdit* outputArea;
  QLineEdit* inputLine;
  //QProcess* process;
  ConsoleWidgetPrivate();
  ~ConsoleWidgetPrivate();
 };

 ConsoleWidgetPrivate::ConsoleWidgetPrivate()
 {

 }

 ConsoleWidgetPrivate::~ConsoleWidgetPrivate()
 {

 }

 ConsoleWidget::ConsoleWidget(QWidget* parent /*= nullptr*/)
 {
  //QVBoxLayout
 }

 ConsoleWidget::~ConsoleWidget()
 {

 }


}