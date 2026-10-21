module;
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <wobjectimpl.h>
module DockWidget;


namespace ArtifactWidgets
{

 W_OBJECT_IMPL(Pane);

 class Pane::Impl
 {
 public:
  Impl() = default;
  ~Impl() = default;
 };

 Pane::Pane(const QString& title, QWidget* contentWidget, QWidget* parent /*= nullptr*/)
   : QWidget(parent), impl_(new Impl())
 {
  setWindowTitle(title);
  if (contentWidget) {
   contentWidget->setParent(this);
   auto *layout = new QVBoxLayout(this);
   layout->setContentsMargins(0, 0, 0, 0);
   layout->addWidget(contentWidget);
  }
 }

 Pane::~Pane()
 {
  delete impl_;
 }

};



