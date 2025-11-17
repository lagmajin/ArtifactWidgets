module;
#include <QDialog>
export module KeyboardOverlayDialog;

export namespace ArtifactWidgets
{
 class KeyboardOverlayDialog:public QDialog
 {
 private:
  class Impl;
  Impl* impl_;
 public:
  explicit KeyboardOverlayDialog(QWidget* parent = nullptr);
  ~KeyboardOverlayDialog();
  //void test();
 };


}
