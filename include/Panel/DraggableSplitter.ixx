
module;
#include <QSplitter>
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QDropEvent>
#include <QPoint>
#include "../Define/DllExportMacro.hpp"
//#include "../Define/DllExportMacro.hpp"

export module Panel.DraggableSplitter;

export namespace ArtifactWidgets
{
 class WIDGET_LIBRARY_DLL_API DraggableSplitter : public QSplitter {
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
   // Keep splitter behavior deterministic: only handle size adjustment.
   // Widget-reordering drag is disabled because it interferes with handle dragging.
   QSplitter::mouseMoveEvent(event);
  }

  void dragEnterEvent(QDragEnterEvent* event) override {
   // レイヤー並べ替えやその他のカスタム MIME を無視し、子に委譲する
   const QMimeData* mime = event->mimeData();
   if (mime && mime->hasFormat("application/x-artifact-layer-reorder")) {
    event->ignore();
    return;
   }
   event->acceptProposedAction();
  }

  void dragMoveEvent(QDragMoveEvent* event) override {
   const QMimeData* mime = event->mimeData();
   if (mime && mime->hasFormat("application/x-artifact-layer-reorder")) {
    event->ignore();
    return;
   }
   event->acceptProposedAction();
  }

  void dropEvent(QDropEvent* event) override {
   // ドロップ位置に最も近いインデックスを探す
   int posX = event->position().toPoint().x();
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
