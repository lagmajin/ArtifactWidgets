
module;
#include <QtCore/QSharedPointer>

#include <QtWidgets/QWidget>
#include <wobjectdefs.h>
export module VectorScopeWidget;


namespace ArtifactWidgets {

 class VectorScopeWidgetPrivate;

 class VectorScopeWidget :public QWidget{
 private:
  VectorScopeWidgetPrivate* pWidget_;
 public:
  explicit VectorScopeWidget(QWidget* parent = nullptr);
  virtual ~VectorScopeWidget();
 signals:
  void updateFrameSucceeded();
 public slots:

 };






}