module;

#include <QtWidgets/QtWidgets>
#include <wobjectdefs.h>

export module Widgets.AlignmentCombobox;



export namespace ArtifactWidgets {


 struct AlignmentComboboxPrivate;

 class  AlignmentCombobox :public QComboBox {
  W_OBJECT(AlignmentCombobox)
 private:
  class Impl;
  Impl* impl_;
 protected:
  //bool eventFilter(QObject* object, QEvent* event);
 public:
  explicit AlignmentCombobox(QWidget* parent = nullptr);
  virtual ~AlignmentCombobox();
  Qt::Alignment allignment() const;
  void setAlignment(const Qt::Alignment align);

  

 };





}