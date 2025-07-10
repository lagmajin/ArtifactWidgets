module;
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
  setRenderHint(QPainter::Antialiasing, true); // �A���`�G�C���A�V���O��L���ɂ���i�I�v�V�����j
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

  // �V�[���̃T�C�Y���摜�ɍ��킹�Đݒ�
  impl_->scene_->setSceneRect(pix->boundingRect());

  // �摜���r���[�S�̂Ɏ��܂�悤�ɒ���
  fitInView(pix->boundingRect(), Qt::KeepAspectRatio);

 }

 void BasicImageViewWidget::wheelEvent(QWheelEvent* event)
 {
  constexpr double scaleFactor = 1.15;

  if (event->angleDelta().y() > 0) {
   // �z�C�[������� �� �g��
   scale(scaleFactor, scaleFactor);
  }
  else {
   // �z�C�[�������� �� �k��
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
	// �ŏ��Ƀh���b�v���ꂽ�摜��ǂݍ���
	// �����h���b�v���ꂽ�ꍇ�ł��ŏ���1�������������
	QImage droppedImage(path);
	if (!droppedImage.isNull()) {
	 setImage(droppedImage);
	 break; // �ŏ��̉摜�����������烋�[�v�𔲂���
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

};