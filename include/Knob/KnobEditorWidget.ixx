module;
#include <QtWidgets/QWidget>

export module KnobEditorWidget;

//import AbstractKnobWidgetl;


export namespace ArtifactWidgets {

 class KnobEditorWidget:public QWidget {
 private:

 public:
  explicit KnobEditorWidget();
  ~KnobEditorWidget();
  void addWidget();
  void clear();
 };

 KnobEditorWidget::KnobEditorWidget()
 {

 }

 KnobEditorWidget::~KnobEditorWidget()
 {

 }







}