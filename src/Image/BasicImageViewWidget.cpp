module ;
#include <wobjectimpl.h>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QEvent>

#include <OpenImageIO/imagebuf.h>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMessageBox>
#include <QGraphicsItem>
#include <QMenu>
#include <QFile>
module BasicImageViewWidget;


namespace ArtifactWidgets {

 W_OBJECT_IMPL(BasicImageViewWidget)

  class BasicImageViewWidget::Impl {
  private:

  public:
   Impl();
   QGraphicsPixmapItem* pixmap_ = nullptr;
   QGraphicsScene* scene_ = nullptr;
   QMenu* menu_ = nullptr;
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
 }

 BasicImageViewWidget::~BasicImageViewWidget()
 {

 }

 void BasicImageViewWidget::Clear()
 {

  //fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
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
  impl_->scene_->addItem(pix);
  pix->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

  // シーンのサイズを画像に合わせて設定
  impl_->scene_->setSceneRect(pix->boundingRect());

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
   for (const QUrl& url : event->mimeData()->urls()) {
	QString path = url.toLocalFile();
	qDebug() << "Dropped file path:" << path;
	qDebug() << "Exists:" << QFile::exists(path);
	// 最初にドロップされた画像を読み込む
	// 複数ドロップされた場合でも最初の1つだけ処理する例
	QImage droppedImage(path);
	if (!droppedImage.isNull()) {
	 setImage(droppedImage);
	 break; // 最初の画像を処理したらループを抜ける
	}
	else {
	 qDebug() << "Failed to load image from:" << path;
	 QMessageBox::warning(this, "Image Load Error", "Failed to load image from:\n" + path);
	}
   }
   event->acceptProposedAction();
  }
  else {
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

};