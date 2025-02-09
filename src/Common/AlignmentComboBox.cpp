module;
#include <QtWidgets/QWidget>
#include <wobjectimpl.h>

module AlignmentCombobox;

namespace ArtifactWidgets {

 W_OBJECT_IMPL(AlignmentCombobox)

 struct AlignmentComboboxPrivate
 {
  Qt::Alignment allignment() const;
  void setAlignment(const Qt::Alignment align);
 };

 void AlignmentComboboxPrivate::setAlignment(const Qt::Alignment align)
 {

 }

AlignmentCombobox::AlignmentCombobox(QWidget* parent /*= nullptr*/):QComboBox(parent)
 {

 }

AlignmentCombobox::~AlignmentCombobox()
{

}

void AlignmentCombobox::setAlignment(const Qt::Alignment align)
{

}

}

