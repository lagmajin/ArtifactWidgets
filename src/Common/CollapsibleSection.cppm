module;
#include <QToolButton>
#include <QBoxLayout>
#include <wobjectimpl.h>

module CollapsibleSection;



namespace ArtifactWidgets
{
 W_OBJECT_IMPL(CollapsibleSection)

  class CollapsibleSection::Impl
 {
 private:


 public:
  Impl();
  ~Impl();
  QToolButton* toggle_ = nullptr;
  QWidget* content_ = nullptr;
  QVBoxLayout* mainLayout_ = nullptr;
  bool expanded_ = true;
 };

 CollapsibleSection::Impl::Impl()
 {

 }

 CollapsibleSection::Impl::~Impl()
 {

 }

 CollapsibleSection::CollapsibleSection(const QString& title, QWidget* contentWidget /*= nullptr*/, QWidget* parent /*= nullptr*/) :QWidget(parent), impl_(new Impl())
 {
  impl_->mainLayout_ = new QVBoxLayout(this);
  impl_->mainLayout_->setContentsMargins(0, 0, 0, 0);
  impl_->mainLayout_->setSpacing(0);

  impl_->toggle_ = new QToolButton(this);
  impl_->toggle_->setText(title);
  impl_->toggle_->setCheckable(true);
  impl_->toggle_->setChecked(true);
  impl_->toggle_->setArrowType(Qt::DownArrow);
  impl_->toggle_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
 	
  connect(impl_->toggle_, &QToolButton::toggled, this, [this](bool checked) {
   setExpanded(checked);
   });

  impl_->mainLayout_->addWidget(impl_->toggle_);

  if (contentWidget)
   setContent(contentWidget);
 }

 CollapsibleSection::~CollapsibleSection()
 {
  delete impl_;
 }

 void CollapsibleSection::setContent(QWidget* widget)
 {
  if (impl_->content_) {
   impl_->mainLayout_->removeWidget(impl_->content_);
   impl_->content_->setParent(nullptr);
  }

  impl_->content_ = widget;

  if (widget) {
   impl_->mainLayout_->addWidget(widget);
   widget->setVisible(impl_->expanded_);
  }
 }

 void CollapsibleSection::setExpanded(bool expand)
 {
  impl_->expanded_ = expand;
  if (impl_->content_)
   impl_->content_->setVisible(expand);

  impl_->toggle_->setArrowType(expand ? Qt::DownArrow : Qt::RightArrow);
 }

 bool CollapsibleSection::isExpanded() const
 {
  return impl_->expanded_;
 }

};