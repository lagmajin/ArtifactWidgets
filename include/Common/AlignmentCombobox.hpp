#pragma once

#include <QtWidgets/QtWidgets>

namespace ArtifactWidgets {


 struct AlignmentComboboxPrivate;

 class  AlignmentCombobox :public QComboBox {
  Q_OBJECT
 private:

 protected:
  //bool eventFilter(QObject* object, QEvent* event);
 public:
  explicit AlignmentCombobox(QWidget* parent = nullptr);
  virtual ~AlignmentCombobox();
  Qt::Alignment allignment() const;
  void setAlignment(const Qt::Alignment align);
 signals:
 private slots:

 public slots:
 };





}