module;

#include <QtCore/QScopedPointer>
#include <QtWidgets/QDialog>
#include <wobjectdefs.h>

export module AbstractDialog;


export namespace ArtifactWidgets {

 class AbstractDialogPrivate;

 class AbstractDialog :public QDialog {
	W_OBJECT(AbstractDialog)
 private:
  QScopedPointer<AbstractDialogPrivate> const pImpl_;
 public:
  explicit AbstractDialog(QWidget* parent = nullptr);
  virtual ~AbstractDialog();
 signals:

 public slots:

 };





}