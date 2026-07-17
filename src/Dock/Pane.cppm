module;
#include <DockWidget.h>
#include <QBoxLayout>
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
  : ads::CDockWidget(title, parent), impl_(new Impl())
 {
  if (contentWidget) {
   // CDockWidget の標準動作に完全に任せる（AutoScrollArea）
   setWidget(contentWidget);
  }
 }

 Pane::~Pane()
 {
  delete impl_;
 }

};



