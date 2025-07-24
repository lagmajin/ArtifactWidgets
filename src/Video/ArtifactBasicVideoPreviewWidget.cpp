module;

#include <wobjectcpp.h>
#include <wobjectdefs.h>
#include <wobjectimpl.h>

#include <QtCore/QtCore>
#include <QWidget>
#include <QGraphicsScene>
#include <QAudioSink>
#include <QIODevice>
#include <QBuffer>

//#include "../Image/BasicImageViewWidget.hpp"

module Widgets.VideoPreview;


 
namespace ArtifactWidgets {


 W_OBJECT_IMPL(ArtifactBasicVideoPreviewWidget)

  class ArtifactBasicVideoPreviewWidget::Impl {
  private:
   QGraphicsScene scene_;
  public:
   Impl();
   ~Impl();
   void fetchVideoThumbnail();
   void initializeAudio(int sampleRate, int channels, int sampleSize);
   void playAudio();
   void stopAudio();
   void writeAudioBuffer(const QByteArray& array);
   QAudioSink* audioSink_ = nullptr;
   QAudioFormat audioFormat_;        // 再生するオーディオのフォーマット
   QBuffer audioBuffer_;             // 再生用のオーディオデータバッファ (例: デコード済みPCMデータ)
   QIODevice* audioDeviceIo_ = nullptr;
 };



 ArtifactBasicVideoPreviewWidget::Impl::Impl()
 {

 }

 ArtifactBasicVideoPreviewWidget::Impl::~Impl()
 {
  if (audioSink_) {
   audioSink_->stop(); // 停止
   delete audioSink_;
  }
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

 }

 ArtifactBasicVideoPreviewWidget::ArtifactBasicVideoPreviewWidget(QWidget* parent/*=nullptr*/)
 {
  


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

 };