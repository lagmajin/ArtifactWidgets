#pragma once


#include <QtGui/QAction>




namespace ArtifactWidgets {

 class CommandActionPrivate;

 class __declspec(dllexport) CommandAction :public QAction {
  Q_OBJECT
 private:
  CommandActionPrivate* const	pAction_;
  QString	command_;
 protected:

 public:
  explicit CommandAction(QObject* parent);
  CommandAction(const QString& command, QObject* parent);
  virtual ~CommandAction();
  QString command() const;
  void setCommand(const QString& command);
  bool hasCommand() const;

 signals:
  void commandRequested(const QString& str);
 private slots:
  void sendCommand();
 };



}