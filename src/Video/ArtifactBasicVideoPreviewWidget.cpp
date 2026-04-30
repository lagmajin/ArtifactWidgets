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
#include <QImage>
#include <QVBoxLayout>
#include <QTimer>
#include <cstring>

//#include "../Image/BasicImageViewWidget.hpp"

module Widgets.VideoPreview;

import std;

import Codec.FFmpegVideoDecoder;
import Codec.Thumbnail.FFmpeg;
import Video.VideoFrame;

namespace ArtifactCore{}//Dummy
 
namespace ArtifactWidgets {

 using namespace ArtifactCore;

 static QImage cpuVideoFrameToQImage(const ArtifactCore::CpuVideoFrame& frame) {
  if (!frame.isValid() || frame.meta.pixelFormat != ArtifactCore::VideoFramePixelFormat::RGB24) {
   return {};
  }

  QImage image(frame.meta.width, frame.meta.height, QImage::Format_RGB888);
  if (image.isNull()) {
   return {};
  }

  const int rowBytes = frame.meta.width * 3;
  for (int y = 0; y < frame.meta.height; ++y) {
   std::memcpy(image.scanLine(y), frame.bytes.data() + static_cast<size_t>(y) * static_cast<size_t>(frame.strideBytes), static_cast<size_t>(rowBytes));
  }

  return image;
 }

 static QImage decodedVideoFrameToQImage(const ArtifactCore::DecodedVideoFrame& frame) {
  return std::visit([](const auto& value) -> QImage {
   using ValueT = std::decay_t<decltype(value)>;
   if constexpr (std::is_same_v<ValueT, ArtifactCore::CpuVideoFrame>) {
    return cpuVideoFrameToQImage(value);
   }
   return {};
  }, frame);
 }

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
   ArtifactCore::FFmpegVideoDecoder* decoder_ = nullptr;
   ArtifactCore::FFmpegThumbnailExtractor* thumbnailExtractor_ = nullptr;
   QString videoFilePath_;
   QGraphicsView* view_ = nullptr;
   QTimer* timer_ = nullptr;
 };
 


 ArtifactBasicVideoPreviewWidget::Impl::Impl()
 : decoder_(new ArtifactCore::FFmpegVideoDecoder()),
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
      QImage frame = decodedVideoFrameToQImage(impl_->decoder_->decodeNextVideoFrameRaw());
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
