module;

#include <QtCore/QScopedPointer>
#include <QtWidgets/QDialog>
#include <wobjectdefs.h>

export module Widgets.Dialog.Abstract;


export namespace ArtifactWidgets {

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
  void focusInEvent(QFocusEvent* event) override;
  void focusOutEvent(QFocusEvent* event) override;
 public:
  explicit AbstractDialog(QWidget* parent = nullptr);
  virtual ~AbstractDialog();
  void setAcceptOnEnter(bool enabled = true);
  void setRejectOnEscape(bool enabled = true);
  void setAlwaysOnTop(bool enabled);
  void setFrameless(bool enabled = true);
 signals:

 public slots:

 };





}