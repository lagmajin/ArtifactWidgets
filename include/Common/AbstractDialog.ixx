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
  class Impl;
  Impl* impl_;

 protected:
  void keyPressEvent(QKeyEvent* event) override;

  // マウスでダイアログをドラッグ移動できるようにする
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
 public:
  explicit AbstractDialog(QWidget* parent = nullptr);
  virtual ~AbstractDialog();
  void setAcceptOnEnter(bool enabled = true);
  void setRejectOnEscape(bool enabled = true);
 signals:

 public slots:

 };





}