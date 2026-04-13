module;
#include <QtWidgets/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <wobjectimpl.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <QtConcurrent>
#include <QFutureWatcher>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <cmath>
#include <functional>
#include <optional>
#include <utility>
#include <array>
#include <mutex>
#include <thread>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <any>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <deque>
#include <list>
#include <tuple>
#include <numeric>
#include <regex>
#include <random>
module WaveformScopeWidget;





namespace ArtifactWidgets {

 W_OBJECT_IMPL(WaveformScopeWidget)

 // ==================== Impl ====================
 class WaveformScopeWidget::Impl {
 public:
  Impl();
  ~Impl();

  QImage computeScopeImage(int width, int height) const;
  void rebuildScopeImageAsync(int width, int height);

  QImage currentFrame_;
  QImage scopeImage_;          // Pre-rendered scope image
  QImage pendingScopeImage_;   // Image being computed
  WaveformMode mode_ = WaveformMode::Luma;
  float intensity_ = 0.6f;
  bool dirty_ = true;
  bool computing_ = false;     // True while background computation is running
  QFutureWatcher<QImage>* watcher_ = nullptr;
 };

 WaveformScopeWidget::Impl::Impl() : watcher_(new QFutureWatcher<QImage>()) {}
 WaveformScopeWidget::Impl::~Impl() {
  if (watcher_) {
   watcher_->cancel();
   watcher_->deleteLater();
  }
 }

 // Build the waveform scope image from the current video frame (heavy computation)
 QImage WaveformScopeWidget::Impl::computeScopeImage(int scopeW, int scopeH) const {
  if (currentFrame_.isNull() || scopeW <= 0 || scopeH <= 0) {
   return QImage();
  }

  QImage scopeImage(scopeW, scopeH, QImage::Format_ARGB32_Premultiplied);
  scopeImage.fill(Qt::black);

  const QImage frame = currentFrame_.scaled(scopeW, currentFrame_.height(),
   Qt::IgnoreAspectRatio, Qt::FastTransformation)
   .convertToFormat(QImage::Format_ARGB32);

  const int frameW = frame.width();
  const int frameH = frame.height();
  const int alphaAdd = std::max(1, static_cast<int>(intensity_ * 12.0f));

  // Use scanLine for faster pixel access
  for (int x = 0; x < frameW && x < scopeW; ++x) {
   for (int fy = 0; fy < frameH; ++fy) {
    const QRgb* scanLine = reinterpret_cast<const QRgb*>(frame.scanLine(fy));
    QRgb pixel = scanLine[x];
    int r = qRed(pixel);
    int g = qGreen(pixel);
    int b = qBlue(pixel);

    if (mode_ == WaveformMode::Luma || mode_ == WaveformMode::YCbCr) {
     // ITU-R BT.709 luminance
     int luma = static_cast<int>(0.2126 * r + 0.7152 * g + 0.0722 * b);
     int sy = scopeH - 1 - (luma * (scopeH - 1) / 255);
     sy = std::clamp(sy, 0, scopeH - 1);

     QRgb existing = scopeImage.pixel(x, sy);
     int er = std::min(255, qRed(existing) + alphaAdd);
     int eg = std::min(255, qGreen(existing) + alphaAdd);
     int eb = std::min(255, qBlue(existing) + alphaAdd);
     scopeImage.setPixel(x, sy, qRgb(er, eg, eb));
    }

    if (mode_ == WaveformMode::RGB) {
     // Red channel
     {
      int sy = scopeH - 1 - (r * (scopeH - 1) / 255);
      sy = std::clamp(sy, 0, scopeH - 1);
      QRgb existing = scopeImage.pixel(x, sy);
      int nr = std::min(255, qRed(existing) + alphaAdd);
      scopeImage.setPixel(x, sy, qRgb(nr, qGreen(existing), qBlue(existing)));
     }
     // Green channel
     {
      int sy = scopeH - 1 - (g * (scopeH - 1) / 255);
      sy = std::clamp(sy, 0, scopeH - 1);
      QRgb existing = scopeImage.pixel(x, sy);
      int ng = std::min(255, qGreen(existing) + alphaAdd);
      scopeImage.setPixel(x, sy, qRgb(qRed(existing), ng, qBlue(existing)));
     }
     // Blue channel
     {
      int sy = scopeH - 1 - (b * (scopeH - 1) / 255);
      sy = std::clamp(sy, 0, scopeH - 1);
      QRgb existing = scopeImage.pixel(x, sy);
      int nb = std::min(255, qBlue(existing) + alphaAdd);
      scopeImage.setPixel(x, sy, qRgb(qRed(existing), qGreen(existing), nb));
     }
    }

    if (mode_ == WaveformMode::YCbCr) {
     // Cb
     float cb = 128.0f + (-0.1146f * r - 0.3854f * g + 0.5f * b);
     int cbI = std::clamp(static_cast<int>(cb), 0, 255);
     int sy = scopeH - 1 - (cbI * (scopeH - 1) / 255);
     sy = std::clamp(sy, 0, scopeH - 1);
     QRgb existing = scopeImage.pixel(x, sy);
     int nb = std::min(255, qBlue(existing) + alphaAdd);
     scopeImage.setPixel(x, sy, qRgb(qRed(existing), qGreen(existing), nb));

     // Cr
     float cr = 128.0f + (0.5f * r - 0.4542f * g - 0.0458f * b);
     int crI = std::clamp(static_cast<int>(cr), 0, 255);
     sy = scopeH - 1 - (crI * (scopeH - 1) / 255);
     sy = std::clamp(sy, 0, scopeH - 1);
     existing = scopeImage.pixel(x, sy);
     int nr = std::min(255, qRed(existing) + alphaAdd);
     scopeImage.setPixel(x, sy, qRgb(nr, qGreen(existing), qBlue(existing)));
    }
   }
  }
}

 // ==================== WaveformScopeWidget ====================

 WaveformScopeWidget::WaveformScopeWidget(QWidget* parent)
  : AbstractWidget(parent), impl_(new Impl)
 {
  setMinimumSize(200, 150);
 }

 WaveformScopeWidget::~WaveformScopeWidget() {
  delete impl_;
 }

 void WaveformScopeWidget::updateFrame(const QImage& frame) {
  impl_->currentFrame_ = frame;
  impl_->dirty_ = true;
  update();
 }

 void WaveformScopeWidget::setMode(WaveformMode mode) {
  if (impl_->mode_ != mode) {
   impl_->mode_ = mode;
   impl_->dirty_ = true;
   update();
   emit modeChanged(mode);
  }
 }

 WaveformMode WaveformScopeWidget::mode() const {
  return impl_->mode_;
 }

 void WaveformScopeWidget::setIntensity(float intensity) {
  impl_->intensity_ = std::clamp(intensity, 0.0f, 1.0f);
  impl_->dirty_ = true;
  update();
 }

 float WaveformScopeWidget::intensity() const {
  return impl_->intensity_;
 }

 void WaveformScopeWidget::resizeEvent(QResizeEvent* event) {
  AbstractWidget::resizeEvent(event);
  impl_->dirty_ = true;
 }

 void WaveformScopeWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Background
  painter.fillRect(rect(), QColor(20, 20, 20));

  const int margin = 30;
  const QRect scopeRect(margin, 10, width() - margin - 10, height() - 30);

  if (scopeRect.width() <= 0 || scopeRect.height() <= 0) return;

  // Rebuild scope image if needed (async)
  if (impl_->dirty_ && !impl_->computing_) {
   impl_->rebuildScopeImageAsync(scopeRect.width(), scopeRect.height());
  }

  // Draw graticule (horizontal reference lines)
  painter.setPen(QPen(QColor(60, 60, 60), 1, Qt::DotLine));
  const int divisions = 10;
  for (int i = 0; i <= divisions; ++i) {
   int y = scopeRect.top() + (scopeRect.height() * i / divisions);
   painter.drawLine(scopeRect.left(), y, scopeRect.right(), y);

   // IRE / percentage labels
   int value = 100 - (i * 100 / divisions);
   painter.setPen(QColor(120, 120, 120));
   painter.setFont(QFont("Consolas", 7));
   painter.drawText(2, y + 4, QString::number(value));
   painter.setPen(QPen(QColor(60, 60, 60), 1, Qt::DotLine));
  }

  // Draw scope image
  if (!impl_->scopeImage_.isNull()) {
   painter.drawImage(scopeRect.topLeft(), impl_->scopeImage_);
  }

  // Border
  painter.setPen(QPen(QColor(80, 80, 80), 1));
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(scopeRect);

  // Title
  painter.setPen(QColor(180, 180, 180));
  painter.setFont(QFont("Segoe UI", 8));
  QString title;
  switch (impl_->mode_) {
  case WaveformMode::Luma: title = "Waveform (Luma)"; break;
  case WaveformMode::RGB:  title = "Waveform (RGB)"; break;
  case WaveformMode::YCbCr: title = "Waveform (YCbCr)"; break;
  }
  painter.drawText(scopeRect.left(), height() - 5, title);
 }

 // Async rebuild: compute scope image in background thread
 void WaveformScopeWidget::Impl::rebuildScopeImageAsync(int width, int height) {
  if (computing_) return;

  computing_ = true;
  const auto frame = currentFrame_;  // Copy for worker thread
  const auto mode = mode_;
  const float intensity = intensity_;

  QObject::connect(watcher_, &QFutureWatcher<QImage>::finished, [this]() {
   if (watcher_->isCanceled()) {
    computing_ = false;
    return;
   }
   scopeImage_ = watcher_->result();
   dirty_ = false;
   computing_ = false;
   // Trigger redraw
   if (QWidget* w = qobject_cast<QWidget*>(watcher_->parent())) {
    w->update();
   }
  });

  watcher_->setFuture(QtConcurrent::run([this, frame, width, height, mode, intensity]() -> QImage {
   // Create a temporary Impl-like context for computation
   QImage scopeImage(width, height, QImage::Format_ARGB32_Premultiplied);
   scopeImage.fill(Qt::black);

   const QImage scaledFrame = frame.scaled(width, frame.height(),
    Qt::IgnoreAspectRatio, Qt::FastTransformation)
    .convertToFormat(QImage::Format_ARGB32);

   const int frameW = scaledFrame.width();
   const int frameH = scaledFrame.height();
   const int alphaAdd = std::max(1, static_cast<int>(intensity * 12.0f));

   for (int x = 0; x < frameW && x < width; ++x) {
    for (int fy = 0; fy < frameH; ++fy) {
     const QRgb* scanLine = reinterpret_cast<const QRgb*>(scaledFrame.scanLine(fy));
     QRgb pixel = scanLine[x];
     int r = qRed(pixel);
     int g = qGreen(pixel);
     int b = qBlue(pixel);

     if (mode == WaveformMode::Luma || mode == WaveformMode::YCbCr) {
      int luma = static_cast<int>(0.2126 * r + 0.7152 * g + 0.0722 * b);
      int sy = height - 1 - (luma * (height - 1) / 255);
      QRgb existing = scopeImage.pixel(x, sy);
      int a = std::min(255, qAlpha(existing) + alphaAdd);
      scopeImage.setPixel(x, sy, qRgba(r, g, b, a));
     } else if (mode == WaveformMode::RGB) {
      int sr = height - 1 - (r * (height - 1) / 255);
      int sg = height - 1 - (g * (height - 1) / 255);
      int sb = height - 1 - (b * (height - 1) / 255);

      QRgb existingR = scopeImage.pixel(x, sr);
      scopeImage.setPixel(x, sr, qRgba(255, 0, 0, std::min(255, qAlpha(existingR) + alphaAdd)));

      QRgb existingG = scopeImage.pixel(x, sg);
      scopeImage.setPixel(x, sg, qRgba(0, 255, 0, std::min(255, qAlpha(existingG) + alphaAdd)));

      QRgb existingB = scopeImage.pixel(x, sb);
      scopeImage.setPixel(x, sb, qRgba(0, 0, 255, std::min(255, qAlpha(existingB) + alphaAdd)));
     }
    }
   }

   return scopeImage;
  }));
 }

};
