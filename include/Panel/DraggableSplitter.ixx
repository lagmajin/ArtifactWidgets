
module;
#include <QSplitter>
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QDropEvent>
#include <QPoint>
#include "../Define/DllExportMacro.hpp"

export module Panel.DraggableSplitter;

export namespace ArtifactWidgets
{
 class LIBRARY_DLL_API DraggableSplitter : public QSplitter {
  //Q_OBJECT
 public:
  using QSplitter::QSplitter;

  DraggableSplitter(QWidget* parent = nullptr)
   : QSplitter(parent)
  {
   setAcceptDrops(true);  // ドロップを受け付ける
  }

 protected:
  void mousePressEvent(QMouseEvent* event) override {
   if (event->button() == Qt::LeftButton) {
    dragStartPos = event->pos();
   }
   QSplitter::mousePressEvent(event);
  }

  void mouseMoveEvent(QMouseEvent* event) override {
   if (!(event->buttons() & Qt::LeftButton))
    return;

   if ((event->pos() - dragStartPos).manhattanLength() < QApplication::startDragDistance())
    return;

   // ドラッグ開始
   auto drag = new QDrag(this);
   auto mime = new QMimeData;
   mime->setText("DraggableSplitterWidget");
   drag->setMimeData(mime);
   drag->exec(Qt::MoveAction);
  }

  void dragEnterEvent(QDragEnterEvent* event) override {
   event->acceptProposedAction();
  }

  void dragMoveEvent(QDragMoveEvent* event) override {
   event->acceptProposedAction();
  }

  void dropEvent(QDropEvent* event) override {
   // ドロップ位置に最も近いインデックスを探す
   int posX = event->pos().x();
   int closestIndex = 0;
   int minDistance = INT_MAX;

   for (int i = 0; i < count(); ++i) {
    int widgetCenter = widget(i)->x() + widget(i)->width() / 2;
    int dist = abs(widgetCenter - posX);
    if (dist < minDistance) {
     minDistance = dist;
     closestIndex = i;
    }
   }

   // ドラッグ中のウィジェットを取得
   QWidget* draggedWidget = nullptr;
   for (int i = 0; i < count(); ++i) {
    if (widget(i)->underMouse()) {
     draggedWidget = widget(i);
     break;
    }
   }
   if (!draggedWidget) return;

   // いったん削除して挿入
   draggedWidget->setParent(nullptr);
   insertWidget(closestIndex, draggedWidget);

   event->acceptProposedAction();
  }

 private:
  QPoint dragStartPos;
 };

};