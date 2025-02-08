//#include "../../include/Common/AbstractDialog.hpp"

module;
//#include <wobjectdefs.h>
#include <wobjectimpl.h>
module AbstractDialog;




namespace ArtifactWidgets {
 W_OBJECT_IMPL(AbstractDialog)

 
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

