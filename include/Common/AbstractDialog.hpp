#pragma once

#include <QtCore/QScopedPointer>
#include <QtWidgets/QDialog>




namespace ArtifactWidgets {

 class AbstractDialogPrivate;

 class AbstractDialog :public QDialog {
 private:
  QScopedPointer<AbstractDialogPrivate> const pImpl_;
 public:
  explicit AbstractDialog(QWidget* parent = nullptr);
  virtual ~AbstractDialog();
 signals:

 public slots:

 };





}