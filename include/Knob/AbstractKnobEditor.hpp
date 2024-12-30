#pragma once


#include <QtWidgets/QtWidgets>

namespace ArtifactWidgets {

 class AbstractKnobEditorPrivate;

 class AbstractKnobEditor:public QWidget {
 private:

 public:
  explicit AbstractKnobEditor(QWidget* parent = nullptr);
  virtual ~AbstractKnobEditor();
 };






}