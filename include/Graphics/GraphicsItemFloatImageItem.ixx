module;

#include <QtWidgets/QGraphicsItem>

export module GraphicsFloatImageItem;

export namespace ArtifactCore {

 class GraphicsFloatImageItem;

 class GraphicsFloatImageItem:public QGraphicsItem {
 private:

 public:
  explicit GraphicsFloatImageItem(QGraphicsItem* parent = nullptr);
  ~GraphicsFloatImageItem();

 };






}