module ;
#include <wobjectimpl.h>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QEvent>
#include <QResizeEvent>

#include <OpenImageIO/imagebuf.h>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMessageBox>
#include <QGraphicsItem>
#include <QMenu>
#include <QFile>
#include <QtConcurrent>
#include <QFutureWatcher>
module BasicImageViewWidget;

import Widgets.Effects.GlowFrame;

namespace ArtifactWidgets {

 W_OBJECT_IMPL(BasicImageViewWidget)

  class BasicImageViewWidget::Impl {
  private:

  public:
   Impl();
   QGraphicsPixmapItem* pixmap_ = nullptr;
   QGraphicsScene* scene_ = nullptr;
   QMenu* menu_ = nullptr;
   GlowFrame* glow = nullptr;
   bool soloMode = true;

 };

 BasicImageViewWidget::Impl::Impl()
 {

 }

 BasicImageViewWidget::BasicImageViewWidget(QWidget* parent) :QGraphicsView(parent), impl_(new Impl())
 {
  

  setWindowTitle("ImageView");

  impl_->menu_ = new QMenu(this);
  impl_->scene_ = new QGraphicsScene(this);

  setScene(impl_->scene_);

  setAcceptDrops(true);
  setContextMenuPolicy(Qt::CustomContextMenu);
  setRenderHint(QPainter::Antialiasing, true); // アンチエイリアシングを有効にする（オプション）
  setRenderHint(QPainter::SmoothPixmapTransform, true);

  setBackgroundBrush(Qt::darkGray);

  connect(this, &QWidget::customContextMenuRequested,
   this, &BasicImageViewWidget::showContextMenu);

  //impl_->glow = new GlowFrame(this);
  //impl_->glow->setGeometry(rect());
  //impl_->glow->raise();  // 最前面に
  //impl_->glow->show();
 }

 BasicImageViewWidget::~BasicImageViewWidget()
 {

 }

 void BasicImageViewWidget::Clear()
 {
  impl_->scene_->clear();
  impl_->pixmap_ = nullptr;
  impl_->scene_->setSceneRect(QRectF());
 }

 void BasicImageViewWidget::setImage(const QImage& image)
 {
  Clear();

  if (image.isNull()) {
   qDebug() << "Warning: Attempted to set a null image.";
   return;
  }

  QPixmap pixmap = QPixmap::fromImage(image);
  if (pixmap.isNull()) {
   qDebug() << "Error: Failed to convert QImage to QPixmap.";
   return;
  }

  auto pix = new QGraphicsPixmapItem(pixmap);
  impl_->pixmap_ = pix;
  impl_->scene_->addItem(pix);
  pix->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

  // 画像がビュー全体に収まるように調整
  fitInView(pix->boundingRect(), Qt::KeepAspectRatio);

 }

 void BasicImageViewWidget::wheelEvent(QWheelEvent* event)
 {
  constexpr double scaleFactor = 1.15;

  if (event->angleDelta().y() > 0) {
   // ホイール上方向 → 拡大
   scale(scaleFactor, scaleFactor);
  }
  else {
   // ホイール下方向 → 縮小
   scale(1.0 / scaleFactor, 1.0 / scaleFactor);
  }

  event->accept();
 }

 void BasicImageViewWidget::dragEnterEvent(QDragEnterEvent* event)
 {
  if (event->mimeData()->hasUrls()) {
   for (const QUrl& url : event->mimeData()->urls()) {
	QString path = url.toLocalFile();
	qDebug() << "Dropped file path:" << path;
   }

   event->acceptProposedAction();
  }
 }

 void BasicImageViewWidget::dropEvent(QDropEvent* event)
 {
  if (event->mimeData()->hasUrls()) {
   // Process first dropped file only
   const QUrl url = event->mimeData()->urls().first();
   const QString path = url.toLocalFile();

   // Show loading indicator
   setCursor(Qt::WaitCursor);

   // Load image asynchronously
   auto* watcher = new QFutureWatcher<QImage>(this);
   QObject::connect(watcher, &QFutureWatcher<QImage>::finished, this, [this, watcher, path]() {
    unsetCursor();
    QImage img = watcher->result();
    watcher->deleteLater();

    if (!img.isNull()) {
     setImage(img);
    } else {
     QMessageBox::warning(this, "Image Load Error",
       "Failed to load image from:\n" + path);
    }
   });

   watcher->setFuture(QtConcurrent::run([path]() -> QImage {
    return QImage(path);
   }));

   event->acceptProposedAction();
  } else {
   event->ignore();
  }
 }


 void BasicImageViewWidget::showContextMenu(const QPoint& pos)
 {

 }

 void BasicImageViewWidget::dragMoveEvent(QDragMoveEvent* event)
 {
  //throw std::logic_error("The method or operation is not implemented.");
 }

 void BasicImageViewWidget::dragLeaveEvent(QDragLeaveEvent* event)
 {
  //throw std::logic_error("The method or operation is not implemented.");
 }

 void BasicImageViewWidget::enableCheckerBoard()
 {

 }

 QSize BasicImageViewWidget::sizeHint() const
 {

  return QSize(600, 500);
 }

 void BasicImageViewWidget::focusInEvent(QFocusEvent*)
 {
  //impl_->glow->setFocused(true);
 }

 void BasicImageViewWidget::focusOutEvent(QFocusEvent*)
 {
  //impl_->glow->setFocused(false);
 }

 void BasicImageViewWidget::resizeEvent(QResizeEvent* event)
 {
  QGraphicsView::resizeEvent(event);
  // GlowFrame をリサイズに追従させる
  if (impl_->glow) {
   impl_->glow->setGeometry(rect());
  }
 }

 void BasicImageViewWidget::soloMode()
 {

 }

 void BasicImageViewWidget::setRotate(double angleDegrees)
 {

 }

};