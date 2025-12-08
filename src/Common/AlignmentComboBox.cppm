module;
#include <QtWidgets/QWidget>
#include <wobjectimpl.h>

module Widgets.AlignmentCombobox;

namespace ArtifactWidgets {

 W_OBJECT_IMPL(AlignmentCombobox)
	
	
 class AlignmentCombobox::Impl
 {
 public:

  Qt::Alignment alignment = Qt::AlignLeft;
 };


 AlignmentCombobox::AlignmentCombobox(QWidget* parent /*= nullptr*/) :QComboBox(parent), impl_(new Impl)
 {

 }

 AlignmentCombobox::~AlignmentCombobox()
 {
  delete impl_;
 }

 Qt::Alignment AlignmentCombobox::allignment() const
 {
  return impl_->alignment;
 }

 void AlignmentCombobox::setAlignment(const Qt::Alignment align)
 {
  impl_->alignment = align;
 }

};

