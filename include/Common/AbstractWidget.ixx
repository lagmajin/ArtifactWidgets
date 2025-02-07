
module;

#include <wobjectcpp.h>

#include <QtCore/QScopedPointer>
#include <QtCore/QFileInfo>
#include <QtWidgets/QWidget>

export module AbstractWidget;

export namespace ArtifactWidgets {



 class AbstractWidgetPrivate;

 class AbstractWidget :public QWidget{
  W_OBJECT(AbstractWidget)
 private:
  QScopedPointer<AbstractWidgetPrivate>const pImpl_;
 public:
  explicit AbstractWidget(QWidget* parent = nullptr);
  virtual~AbstractWidget();
 public slots:

 };






};