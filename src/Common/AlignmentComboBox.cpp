module;
#include <QtWidgets/QWidget>

module AlignmentCombobox;

namespace ArtifactWidgets {

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

