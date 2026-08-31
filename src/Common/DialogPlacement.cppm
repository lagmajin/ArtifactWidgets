module;

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>

module Widgets.Common.DialogPlacement;

namespace ArtifactWidgets {
namespace {
class DialogCenteringFilter final : public QObject {
 public:
  explicit DialogCenteringFilter(QWidget* mainWindow)
      : QObject(mainWindow), mainWindow_(mainWindow) {}

 protected:
  bool eventFilter(QObject* watched, QEvent* event) override {
    if (event->type() == QEvent::Show && mainWindow_ && mainWindow_->isVisible()) {
      if (auto* dialog = qobject_cast<QDialog*>(watched)) {
        const QRect frame = mainWindow_->frameGeometry();
        const QSize size = dialog->size();
        dialog->move(frame.center() - QPoint(size.width() / 2, size.height() / 2));
      }
    }
    return QObject::eventFilter(watched, event);
  }

 private:
  QPointer<QWidget> mainWindow_;
};
}

void installDialogCentering(QWidget* mainWindow) {
  if (!mainWindow || !qApp || mainWindow->property("artifact.dialogCenteringInstalled").toBool())
    return;
  mainWindow->setProperty("artifact.dialogCenteringInstalled", true);
  qApp->installEventFilter(new DialogCenteringFilter(mainWindow));
}
}
