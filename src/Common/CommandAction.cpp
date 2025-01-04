#include "../../include/Common/CommandAction.hpp"






namespace ArtifactWidgets {

 class CommandActionPrivate {
 private:
  bool hasCommand_ = false;
  QString command_;

 public:
  CommandActionPrivate();
  ~CommandActionPrivate();
  QString command() const;
  void setCommand(const QString& command);
  bool hasCommand() const;
 };

 CommandActionPrivate::CommandActionPrivate()
 {


 }

 CommandActionPrivate::~CommandActionPrivate()
 {

 }

 QString CommandActionPrivate::command() const
 {
  return command_;
 }

 void CommandActionPrivate::setCommand(const QString& command)
 {
  command_ = command;
  hasCommand_ = true;
 }

 bool CommandActionPrivate::hasCommand() const
 {
  return true;
 }

 CommandAction::CommandAction(QObject* parent) :QAction(parent), pAction_(new CommandActionPrivate)
 {

  connect(this, SIGNAL(triggered()), this, SLOT(sendCommand()));
 }

 CommandAction::CommandAction(const QString& command, QObject* parent) :QAction(parent), pAction_(new CommandActionPrivate)
 {

 }

 CommandAction::~CommandAction()
 {
  delete pAction_;
 }

 QString CommandAction::command() const
 {

  return pAction_->command();
 }

 void CommandAction::setCommand(const QString& command)
 {

  pAction_->setCommand(command);

 }

 void CommandAction::sendCommand()
 {

  emit commandRequested(pAction_->command());
 }

 bool CommandAction::hasCommand() const
 {
  return pAction_->hasCommand();
 }













};