//#include "../../include/Common/AbstractDialog.hpp"

module;

module AbstractDialog;




namespace ArtifactWidgets {
 
 class AbstractDialogPrivate {
 private:

 public:
  AbstractDialogPrivate();
  ~AbstractDialogPrivate();
 };

 AbstractDialogPrivate::AbstractDialogPrivate()
 {

 }

 AbstractDialogPrivate::~AbstractDialogPrivate()
 {

 }

 AbstractDialog::AbstractDialog(QWidget* parent/*=nullptr*/) :QDialog(parent), pImpl_(new AbstractDialogPrivate)
 {

 }

 AbstractDialog::~AbstractDialog()
 {
  
 }

};

