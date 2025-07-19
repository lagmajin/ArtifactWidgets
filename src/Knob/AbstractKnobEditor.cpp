//#include "../../include/Knob/AbstractKnobEditor.hpp"

module;
#include <wobjectimpl.h>

module Widgets.Knob;

namespace ArtifactWidgets {

 W_OBJECT_IMPL(AbstractKnobEditor)

  class AbstractKnobEditor::Impl {
  private:

  public:
   Impl();
   ~Impl();
 };

 AbstractKnobEditor::Impl::Impl()
 {

 }

 AbstractKnobEditor::Impl::~Impl()
 {

 }

 AbstractKnobEditor::AbstractKnobEditor(QWidget* parent /*= nullptr*/) :QWidget(parent),impl_(new Impl())
 {
  setDisabled(false);
  setAcceptDrops(true);
 
 }

 AbstractKnobEditor::~AbstractKnobEditor()
 {
  delete impl_;
 }
 void AbstractKnobEditor::dragEnterEvent(QDragEnterEvent* event)
 {

 }

 void AbstractKnobEditor::dragMoveEvent(QDragMoveEvent* event)
 {
  //throw std::logic_error("The method or operation is not implemented.");
 }

 void AbstractKnobEditor::dropEvent(QDropEvent* event)
 {
  //throw std::logic_error("The method or operation is not implemented.");
 }



};