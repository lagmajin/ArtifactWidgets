#pragma once

#include <QtWidgets/QDialog>




namespace Artifact {

 class AbstractDialogPrivate;

 class AbstractDialog :public QDialog {
 private:

 public:

 signals:

 public slots:
  explicit AbstractDialog();
  ~AbstractDialog();
 };






}