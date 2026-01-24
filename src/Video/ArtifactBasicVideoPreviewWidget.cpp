module;

#include <wobjectcpp.h>
#include <wobjectdefs.h>
#include <wobjectimpl.h>

#include <QtCore/QtCore>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAudioSink>
#include <QIODevice>
#include <QBuffer>
#include <QVBoxLayout>
#include <QTimer>

//#include "../Image/BasicImageViewWidget.hpp"

module Widgets.VideoPreview;

import Codec.FFmpegVideoDecoder;
import Codec.Thumbnail.FFmpeg;

namespace ArtifactCore{}//Dummy
 
namespace ArtifactWidgets {

 using namespace ArtifactCore;

 W_OBJECT_IMPL(ArtifactBasicVideoPreviewWidget)

  class ArtifactBasicVideoPreviewWidget::Impl {
  private:
   void initializeAudio(int sampleRate, int channels, int sampleSize);
   void playAudio();
   void stopAudio();
   void writeAudioBuffer(const QByteArray& array);
  public:
   Impl();
   ~Impl();
   void fetchVideoThumbnail();
   QGraphicsScene scene_;
   QImage thumbnail_;
   QAudioSink* audioSink_ = nullptr;
   QAudioFormat audioFormat_;        // 再生するオーディオのフォーマット
   QBuffer audioBuffer_;             // 再生用のオーディオデータバッファ (例: デコード済みPCMデータ)
   QIODevice* audioDeviceIo_ = nullptr;
   ArtifactCore::FFmpegDecoder* decoder_ = nullptr;
   ArtifactCore::FFmpegThumbnailExtractor* thumbnailExtractor_ = nullptr;
   QString videoFilePath_;
   QGraphicsView* view_ = nullptr;
   QTimer* timer_ = nullptr;
 };
 


 ArtifactBasicVideoPreviewWidget::Impl::Impl()
 : decoder_(new ArtifactCore::FFmpegDecoder()),
   thumbnailExtractor_(new ArtifactCore::FFmpegThumbnailExtractor()),
   view_(new QGraphicsView(&scene_)),
   timer_(new QTimer())
 {

 }

 ArtifactBasicVideoPreviewWidget::Impl::~Impl()
 {
  if (audioSink_) {
   audioSink_->stop(); // 停止
   delete audioSink_;
  }
  delete decoder_;
  delete thumbnailExtractor_;
  delete timer_;
 }

 void ArtifactBasicVideoPreviewWidget::Impl::initializeAudio(int sampleRate, int channels, int sampleSize)
 {

 }

 void ArtifactBasicVideoPreviewWidget::Impl::playAudio()
 {

 }

 void ArtifactBasicVideoPreviewWidget::Impl::stopAudio()
 {

 }

 void ArtifactBasicVideoPreviewWidget::Impl::writeAudioBuffer(const QByteArray& array)
 {

 }

 void ArtifactBasicVideoPreviewWidget::Impl::fetchVideoThumbnail()
 {
  if (!videoFilePath_.isEmpty()) {
    thumbnail_ = thumbnailExtractor_->extractThumbnailOld(videoFilePath_);
  }
 }

 ArtifactBasicVideoPreviewWidget::ArtifactBasicVideoPreviewWidget(QWidget* parent/*=nullptr*/):QWidget(parent)
 {
  auto layout = new QVBoxLayout(this);
  layout->addWidget(impl_->view_);
  setLayout(layout);

 }

 ArtifactBasicVideoPreviewWidget::~ArtifactBasicVideoPreviewWidget()
 {

 }

 void ArtifactBasicVideoPreviewWidget::dragEnterEvent(QDragEnterEvent* event)
 {
  
 }

 QSize ArtifactBasicVideoPreviewWidget::sizeHint() const
 {
  //throw std::logic_error("The method or operation is not implemented.");
 
  return QSize(600, 400);
 }

 void ArtifactBasicVideoPreviewWidget::setVideoFilePath(const QString& path)
 {
  impl_->videoFilePath_ = path;
  impl_->fetchVideoThumbnail();
 }

 void ArtifactBasicVideoPreviewWidget::play()
 {
  if (!impl_->videoFilePath_.isEmpty()) {
    impl_->decoder_->openFile(impl_->videoFilePath_);
    connect(impl_->timer_, &QTimer::timeout, this, [this]() {
      QImage frame = impl_->decoder_->decodeNextVideoFrame();
      if (!frame.isNull()) {
        impl_->scene_.clear();
        impl_->scene_.addPixmap(QPixmap::fromImage(frame));
        impl_->view_->fitInView(impl_->scene_.sceneRect(), Qt::KeepAspectRatio);
      } else {
        // End of video or error
        stop();
      }
    });
    impl_->timer_->start(33); // ~30 fps
  }
 }

 void ArtifactBasicVideoPreviewWidget::stop()
 {
  impl_->timer_->stop();
  impl_->decoder_->closeFile();
 }

 };