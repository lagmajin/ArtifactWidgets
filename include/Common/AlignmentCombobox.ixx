module;

#include <QtWidgets/QtWidgets>
#include <wobjectdefs.h>

export module AlignmentCombobox;



export namespace ArtifactWidgets {


 struct AlignmentComboboxPrivate;

 class  AlignmentCombobox :public QComboBox {
  W_OBJECT(AlignmentCombobox)
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