module;

#include <QtCore/QScopedPointer>
#include <QtGui/QAction>
#include <wobjectdefs.h>

export module CommandAction;



namespace ArtifactWidgets {

 class CommandActionPrivate;

 class __declspec(dllexport) CommandAction :public QAction {
  W_OBJECT(CommandAction)
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
  //void commandRequested(const QString& str);
 private slots:
  void sendCommand();
 };



}