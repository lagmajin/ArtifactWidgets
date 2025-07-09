module;

#include <wobjectcpp.h>
#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>

export module Widget.Knob;
//export import VolumeSlider;



export namespace ArtifactWidgets {

 class AbstractKnobEditorPrivate;

 class AbstractKnobEditor:public QWidget {
  W_OBJECT(AbstractKnobEditor)
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