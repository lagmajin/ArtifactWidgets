#pragma once


//#include <memory>
#include <QtWidgets/QGraphicsItem>



namespace ArtifactCore {

 class GraphicsFloatImageItem;

 class GraphicsFloatImageItem:public QGraphicsItem {
 private:

 public:
  explicit GraphicsFloatImageItem(QGraphicsItem* parent = nullptr);
  ~GraphicsFloatImageItem();

 };






}