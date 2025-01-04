#include "../../include/Knob/AbstractKnobEditor.hpp"





namespace ArtifactWidgets {


 AbstractKnobEditor::AbstractKnobEditor(QWidget* parent /*= nullptr*/) :QWidget(parent)
 {
  setAcceptDrops(true);
 }

 AbstractKnobEditor::~AbstractKnobEditor()
 {

 }
 void AbstractKnobEditor::dragEnterEvent(QDragEnterEvent* event)
 {

 }

 void AbstractKnobEditor::dragMoveEvent(QDragMoveEvent* event)
 {
  throw std::logic_error("The method or operation is not implemented.");
 }

 void AbstractKnobEditor::dropEvent(QDropEvent* event)
 {
  //throw std::logic_error("The method or operation is not implemented.");
 }
};