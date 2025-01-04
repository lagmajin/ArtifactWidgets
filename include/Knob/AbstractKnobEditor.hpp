#pragma once


#include <QtWidgets/QtWidgets>

namespace ArtifactWidgets {

 class AbstractKnobEditorPrivate;

 class AbstractKnobEditor:public QWidget {
 private:
 protected:
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dragMoveEvent(QDragMoveEvent* event) override;
  void dropEvent(QDropEvent* event) override;

 public:
  explicit AbstractKnobEditor(QWidget* parent = nullptr);
  virtual ~AbstractKnobEditor();
 };






}