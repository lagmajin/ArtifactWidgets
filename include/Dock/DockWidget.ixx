module;
#include <DockWidget.h>
#include <qboxlayout.h>
#include <QLabel>
#include <QPushButton>
#include <wobjectdefs.h>
#include <wobjectimpl.h>
export module DockWidget;




#pragma comment(lib,"qtadvanceddockingd.lib")

export namespace ArtifactWidgets {

 class Pane : public ads::CDockWidget {
  W_OBJECT(Pane)
 public:
  explicit Pane(const QString& title, QWidget* contentWidget, QWidget* parent = nullptr)
   : ads::CDockWidget(title, parent)
  {
   auto wrapper = new QWidget(this);
   auto layout = new QVBoxLayout(wrapper);
   layout->setContentsMargins(0, 0, 0, 0);

   auto header = new QWidget(wrapper);
   auto hlayout = new QHBoxLayout(header);
   hlayout->setContentsMargins(4, 2, 4, 2);

   auto label = new QLabel(title);
   hlayout->addWidget(label);

   auto btnCollapse = new QPushButton("-");
   btnCollapse->setFixedSize(16, 16);
   hlayout->addWidget(btnCollapse);

   layout->addWidget(header);
   layout->addWidget(contentWidget);

   this->setWidget(wrapper);

   connect(btnCollapse, &QPushButton::clicked, [=]() {
	contentWidget->setVisible(!contentWidget->isVisible());
	});
  }
 };

 W_OBJECT_IMPL(Pane);




};