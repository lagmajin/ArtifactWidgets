
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
module ParadeScopeWidget;





namespace ArtifactWidgets {

 W_OBJECT_IMPL(ParadeScopeWidget)

 // ==================== Impl ====================
 class ParadeScopeWidget::Impl {
 public:
  Impl();
  ~Impl();

  std::vector<QImage> computeScopeImages(int totalWidth, int height) const;
  void rebuildScopeImagesAsync(int totalWidth, int height);
  void buildChannelWaveform(QImage& outImage, const QImage& frame,
   int channel, int scopeW, int scopeH, const QColor& traceColor);
  void buildLumaWaveform(QImage& outImage, const QImage& frame,
   int scopeW, int scopeH);
  void buildCbWaveform(QImage& outImage, const QImage& frame,
   int scopeW, int scopeH);
  void buildCrWaveform(QImage& outImage, const QImage& frame,
   int scopeW, int scopeH);

  QImage currentFrame_;
  ParadeMode mode_ = ParadeMode::RGB;
  float intensity_ = 0.6f;
  bool dirty_ = true;

  // Each column gets its own scope image
  std::vector<QImage> scopeImages_;
  std::vector<QColor> scopeColors_;
  std::vector<QString> scopeLabels_;

  // Async rendering state
  bool computing_ = false;
  QFutureWatcher<std::vector<QImage>>* watcher_ = nullptr;
 };

 ParadeScopeWidget::Impl::Impl() : watcher_(new QFutureWatcher<std::vector<QImage>>()) {}
 ParadeScopeWidget::Impl::~Impl() {
  if (watcher_) {
   watcher_->cancel();
   watcher_->deleteLater();
  }
 }

 void ParadeScopeWidget::Impl::buildChannelWaveform(QImage& outImage,
  const QImage& frame, int channel, int scopeW, int scopeH,
  const QColor& traceColor)
 {
  outImage = QImage(scopeW, scopeH, QImage::Format_ARGB32_Premultiplied);
  outImage.fill(Qt::black);

  if (frame.isNull() || scopeW <= 0 || scopeH <= 0) return;

  // Scale frame width to match scope column width
  const QImage scaled = frame.scaled(scopeW, frame.height(),
   Qt::IgnoreAspectRatio, Qt::FastTransformation)
   .convertToFormat(QImage::Format_ARGB32);

  const int frameW = scaled.width();
  const int frameH = scaled.height();
  const int alphaAdd = std::max(1, static_cast<int>(intensity_ * 12.0f));

  const int tr = traceColor.red();
  const int tg = traceColor.green();
  const int tb = traceColor.blue();

  for (int x = 0; x < frameW && x < scopeW; ++x) {
   for (int fy = 0; fy < frameH; ++fy) {
    QRgb pixel = scaled.pixel(x, fy);
    int value = 0;

    switch (channel) {
    case 0: value = qRed(pixel);   break; // R
    case 1: value = qGreen(pixel); break; // G
    case 2: value = qBlue(pixel);  break; // B
    default: break;
    }

    int sy = scopeH - 1 - (value * (scopeH - 1) / 255);
    sy = std::clamp(sy, 0, scopeH - 1);

    QRgb existing = outImage.pixel(x, sy);
    int er = std::min(255, qRed(existing) + (tr * alphaAdd / 255));
    int eg = std::min(255, qGreen(existing) + (tg * alphaAdd / 255));
    int eb = std::min(255, qBlue(existing) + (tb * alphaAdd / 255));
    outImage.setPixel(x, sy, qRgb(er, eg, eb));
   }
  }
 }

 void ParadeScopeWidget::Impl::buildLumaWaveform(QImage& outImage,
  const QImage& frame, int scopeW, int scopeH)
 {
  outImage = QImage(scopeW, scopeH, QImage::Format_ARGB32_Premultiplied);
  outImage.fill(Qt::black);

  if (frame.isNull() || scopeW <= 0 || scopeH <= 0) return;

  const QImage scaled = frame.scaled(scopeW, frame.height(),
   Qt::IgnoreAspectRatio, Qt::FastTransformation)
   .convertToFormat(QImage::Format_ARGB32);

  const int frameW = scaled.width();
  const int frameH = scaled.height();
  const int alphaAdd = std::max(1, static_cast<int>(intensity_ * 12.0f));

  for (int x = 0; x < frameW && x < scopeW; ++x) {
   for (int fy = 0; fy < frameH; ++fy) {
    QRgb pixel = scaled.pixel(x, fy);
    int r = qRed(pixel);
    int g = qGreen(pixel);
    int b = qBlue(pixel);

    // ITU-R BT.709 luminance
    int luma = static_cast<int>(0.2126 * r + 0.7152 * g + 0.0722 * b);
    luma = std::clamp(luma, 0, 255);

    int sy = scopeH - 1 - (luma * (scopeH - 1) / 255);
    sy = std::clamp(sy, 0, scopeH - 1);

    QRgb existing = outImage.pixel(x, sy);
    int er = std::min(255, qRed(existing) + alphaAdd);
    int eg = std::min(255, qGreen(existing) + alphaAdd);
    int eb = std::min(255, qBlue(existing) + alphaAdd);
    outImage.setPixel(x, sy, qRgb(er, eg, eb));
   }
  }
 }

 void ParadeScopeWidget::Impl::buildCbWaveform(QImage& outImage,
  const QImage& frame, int scopeW, int scopeH)
 {
  outImage = QImage(scopeW, scopeH, QImage::Format_ARGB32_Premultiplied);
  outImage.fill(Qt::black);

  if (frame.isNull() || scopeW <= 0 || scopeH <= 0) return;

  const QImage scaled = frame.scaled(scopeW, frame.height(),
   Qt::IgnoreAspectRatio, Qt::FastTransformation)
   .convertToFormat(QImage::Format_ARGB32);

  const int frameW = scaled.width();
  const int frameH = scaled.height();
  const int alphaAdd = std::max(1, static_cast<int>(intensity_ * 12.0f));

  for (int x = 0; x < frameW && x < scopeW; ++x) {
   for (int fy = 0; fy < frameH; ++fy) {
    QRgb pixel = scaled.pixel(x, fy);
    int r = qRed(pixel);
    int g = qGreen(pixel);
    int b = qBlue(pixel);

    // Cb = 128 + (-0.1146*R - 0.3854*G + 0.5*B)
    float cb = 128.0f + (-0.1146f * r - 0.3854f * g + 0.5f * b);
    int cbI = std::clamp(static_cast<int>(cb), 0, 255);

    int sy = scopeH - 1 - (cbI * (scopeH - 1) / 255);
    sy = std::clamp(sy, 0, scopeH - 1);

    QRgb existing = outImage.pixel(x, sy);
    int eg = std::min(255, qGreen(existing) + alphaAdd / 2);
    int eb = std::min(255, qBlue(existing) + alphaAdd);
    outImage.setPixel(x, sy, qRgb(qRed(existing), eg, eb));
   }
  }
 }

 void ParadeScopeWidget::Impl::buildCrWaveform(QImage& outImage,
  const QImage& frame, int scopeW, int scopeH)
 {
  outImage = QImage(scopeW, scopeH, QImage::Format_ARGB32_Premultiplied);
  outImage.fill(Qt::black);

  if (frame.isNull() || scopeW <= 0 || scopeH <= 0) return;

  const QImage scaled = frame.scaled(scopeW, frame.height(),
   Qt::IgnoreAspectRatio, Qt::FastTransformation)
   .convertToFormat(QImage::Format_ARGB32);

  const int frameW = scaled.width();
  const int frameH = scaled.height();
  const int alphaAdd = std::max(1, static_cast<int>(intensity_ * 12.0f));

  for (int x = 0; x < frameW && x < scopeW; ++x) {
   for (int fy = 0; fy < frameH; ++fy) {
    QRgb pixel = scaled.pixel(x, fy);
    int r = qRed(pixel);
    int g = qGreen(pixel);
    int b = qBlue(pixel);

    // Cr = 128 + (0.5*R - 0.4542*G - 0.0458*B)
    float cr = 128.0f + (0.5f * r - 0.4542f * g - 0.0458f * b);
    int crI = std::clamp(static_cast<int>(cr), 0, 255);

    int sy = scopeH - 1 - (crI * (scopeH - 1) / 255);
    sy = std::clamp(sy, 0, scopeH - 1);

    QRgb existing = outImage.pixel(x, sy);
    int er = std::min(255, qRed(existing) + alphaAdd);
    int eg = std::min(255, qGreen(existing) + alphaAdd / 2);
    outImage.setPixel(x, sy, qRgb(er, eg, qBlue(existing)));
   }
  }
 }

 // Compute scope images in background thread (const, thread-safe)
 std::vector<QImage> ParadeScopeWidget::Impl::computeScopeImages(int totalWidth, int height) const {
  std::vector<QImage> images;
  std::vector<QColor> colors;
  std::vector<QString> labels;

  if (currentFrame_.isNull() || totalWidth <= 0 || height <= 0) {
   return images;
  }

  const int alphaAdd = std::max(1, static_cast<int>(intensity_ * 12.0f));

  // Inline waveform builders to avoid const/non-const issues
  auto buildChannelInline = [&currentFrame_ = currentFrame_, alphaAdd](QImage& outImage, int channel, int scopeW, int scopeH, const QColor& traceColor) {
   outImage = QImage(scopeW, scopeH, QImage::Format_ARGB32_Premultiplied);
   outImage.fill(Qt::black);
   if (currentFrame_.isNull() || scopeW <= 0 || scopeH <= 0) return;

   const QImage scaled = currentFrame_.scaled(scopeW, currentFrame_.height(),
    Qt::IgnoreAspectRatio, Qt::FastTransformation)
    .convertToFormat(QImage::Format_ARGB32);

   const int frameW = scaled.width();
   const int frameH = scaled.height();
   const int tr = traceColor.red();
   const int tg = traceColor.green();
   const int tb = traceColor.blue();

   for (int x = 0; x < frameW && x < scopeW; ++x) {
    for (int fy = 0; fy < frameH; ++fy) {
     QRgb pixel = scaled.pixel(x, fy);
     int value = 0;
     switch (channel) {
     case 0: value = qRed(pixel);   break;
     case 1: value = qGreen(pixel); break;
     case 2: value = qBlue(pixel);  break;
     }
     int sy = scopeH - 1 - (value * (scopeH - 1) / 255);
     sy = std::clamp(sy, 0, scopeH - 1);
     QRgb existing = outImage.pixel(x, sy);
     int er = std::min(255, qRed(existing) + (tr * alphaAdd / 255));
     int eg = std::min(255, qGreen(existing) + (tg * alphaAdd / 255));
     int eb = std::min(255, qBlue(existing) + (tb * alphaAdd / 255));
     outImage.setPixel(x, sy, qRgb(er, eg, eb));
    }
   }
  };

  auto buildLumaInline = [&currentFrame_ = currentFrame_, alphaAdd](QImage& outImage, int scopeW, int scopeH) {
   outImage = QImage(scopeW, scopeH, QImage::Format_ARGB32_Premultiplied);
   outImage.fill(Qt::black);
   if (currentFrame_.isNull() || scopeW <= 0 || scopeH <= 0) return;

   const QImage scaled = currentFrame_.scaled(scopeW, currentFrame_.height(),
    Qt::IgnoreAspectRatio, Qt::FastTransformation)
    .convertToFormat(QImage::Format_ARGB32);

   const int frameW = scaled.width();
   const int frameH = scaled.height();

   for (int x = 0; x < frameW && x < scopeW; ++x) {
    for (int fy = 0; fy < frameH; ++fy) {
     QRgb pixel = scaled.pixel(x, fy);
     int r = qRed(pixel);
     int g = qGreen(pixel);
     int b = qBlue(pixel);
     int luma = static_cast<int>(0.2126 * r + 0.7152 * g + 0.0722 * b);
     int sy = scopeH - 1 - (luma * (scopeH - 1) / 255);
     sy = std::clamp(sy, 0, scopeH - 1);
     QRgb existing = outImage.pixel(x, sy);
     int eg = std::min(255, qGreen(existing) + alphaAdd / 2);
     int eb = std::min(255, qBlue(existing) + alphaAdd);
     outImage.setPixel(x, sy, qRgb(qRed(existing), eg, eb));
    }
   }
  };

  auto buildCbInline = [&currentFrame_ = currentFrame_, alphaAdd](QImage& outImage, int scopeW, int scopeH) {
   outImage = QImage(scopeW, scopeH, QImage::Format_ARGB32_Premultiplied);
   outImage.fill(Qt::black);
   if (currentFrame_.isNull() || scopeW <= 0 || scopeH <= 0) return;

   const QImage scaled = currentFrame_.scaled(scopeW, currentFrame_.height(),
    Qt::IgnoreAspectRatio, Qt::FastTransformation)
    .convertToFormat(QImage::Format_ARGB32);

   const int frameW = scaled.width();
   const int frameH = scaled.height();

   for (int x = 0; x < frameW && x < scopeW; ++x) {
    for (int fy = 0; fy < frameH; ++fy) {
     QRgb pixel = scaled.pixel(x, fy);
     int r = qRed(pixel);
     int g = qGreen(pixel);
     int b = qBlue(pixel);
     float cb = 128.0f + (-0.1687f * r - 0.3313f * g + 0.5f * b);
     int cbI = std::clamp(static_cast<int>(cb), 0, 255);
     int sy = scopeH - 1 - (cbI * (scopeH - 1) / 255);
     sy = std::clamp(sy, 0, scopeH - 1);
     QRgb existing = outImage.pixel(x, sy);
     int eg = std::min(255, qGreen(existing) + alphaAdd / 2);
     int eb = std::min(255, qBlue(existing) + alphaAdd);
     outImage.setPixel(x, sy, qRgb(qRed(existing), eg, eb));
    }
   }
  };

  auto buildCrInline = [&currentFrame_ = currentFrame_, alphaAdd](QImage& outImage, int scopeW, int scopeH) {
   outImage = QImage(scopeW, scopeH, QImage::Format_ARGB32_Premultiplied);
   outImage.fill(Qt::black);
   if (currentFrame_.isNull() || scopeW <= 0 || scopeH <= 0) return;

   const QImage scaled = currentFrame_.scaled(scopeW, currentFrame_.height(),
    Qt::IgnoreAspectRatio, Qt::FastTransformation)
    .convertToFormat(QImage::Format_ARGB32);

   const int frameW = scaled.width();
   const int frameH = scaled.height();

   for (int x = 0; x < frameW && x < scopeW; ++x) {
    for (int fy = 0; fy < frameH; ++fy) {
     QRgb pixel = scaled.pixel(x, fy);
     int r = qRed(pixel);
     int g = qGreen(pixel);
     int b = qBlue(pixel);
     float cr = 128.0f + (0.5f * r - 0.4542f * g - 0.0458f * b);
     int crI = std::clamp(static_cast<int>(cr), 0, 255);
     int sy = scopeH - 1 - (crI * (scopeH - 1) / 255);
     sy = std::clamp(sy, 0, scopeH - 1);
     QRgb existing = outImage.pixel(x, sy);
     int er = std::min(255, qRed(existing) + alphaAdd);
     int eg = std::min(255, qGreen(existing) + alphaAdd / 2);
     outImage.setPixel(x, sy, qRgb(er, eg, qBlue(existing)));
    }
   }
  };

  switch (mode_) {
  case ParadeMode::RGB:
  {
   const int colW = totalWidth / 3;
   QImage rImg, gImg, bImg;
   buildChannelInline(rImg, 0, colW, height, QColor(255, 60, 60));
   buildChannelInline(gImg, 1, colW, height, QColor(60, 220, 60));
   buildChannelInline(bImg, 2, colW, height, QColor(60, 100, 255));
   images = { rImg, gImg, bImg };
   colors = { QColor(255, 60, 60), QColor(60, 220, 60), QColor(60, 100, 255) };
   labels = { "R", "G", "B" };
   break;
  }
  case ParadeMode::YCbCr:
  {
   const int colW = totalWidth / 3;
   QImage yImg, cbImg, crImg;
   buildLumaInline(yImg, colW, height);
   buildCbInline(cbImg, colW, height);
   buildCrInline(crImg, colW, height);
   images = { yImg, cbImg, crImg };
   colors = { QColor(200, 200, 200), QColor(80, 120, 255), QColor(255, 80, 80) };
   labels = { "Y", "Cb", "Cr" };
   break;
  }
  case ParadeMode::YRGB:
  {
   const int colW = totalWidth / 4;
   QImage yImg, rImg, gImg, bImg;
   buildLumaInline(yImg, colW, height);
   buildChannelInline(rImg, 0, colW, height, QColor(255, 60, 60));
   buildChannelInline(gImg, 1, colW, height, QColor(60, 220, 60));
   buildChannelInline(bImg, 2, colW, height, QColor(60, 100, 255));
   images = { yImg, rImg, gImg, bImg };
   colors = { QColor(200, 200, 200), QColor(255, 60, 60), QColor(60, 220, 60), QColor(60, 100, 255) };
   labels = { "Y", "R", "G", "B" };
   break;
  }
  }

  // Store colors and labels for paintEvent access
  const_cast<Impl*>(this)->scopeColors_ = colors;
  const_cast<Impl*>(this)->scopeLabels_ = labels;

  return images;
 }

 // Async rebuild: compute scope images in background thread
 void ParadeScopeWidget::Impl::rebuildScopeImagesAsync(int totalWidth, int height) {
  if (computing_) return;

  computing_ = true;
  const auto frame = currentFrame_;  // Copy for worker thread
  const auto mode = mode_;
  const float intensity = intensity_;

  QObject::connect(watcher_, &QFutureWatcher<std::vector<QImage>>::finished, [this]() {
   if (watcher_->isCanceled()) {
    computing_ = false;
    return;
   }
   scopeImages_ = watcher_->result();
   dirty_ = false;
   computing_ = false;
   // Trigger redraw
   if (QWidget* w = qobject_cast<QWidget*>(watcher_->parent())) {
    w->update();
   }
  });

  watcher_->setFuture(QtConcurrent::run([this, frame, totalWidth, height, mode, intensity]() -> std::vector<QImage> {
   // Create a temporary context for computation
   // We need to temporarily swap currentFrame_, mode_, intensity_ for the build methods
   // Since buildChannelWaveform etc. are non-const, we'll create inline implementations
   std::vector<QImage> images;
   std::vector<QColor> colors;
   std::vector<QString> labels;

   if (frame.isNull() || totalWidth <= 0 || height <= 0) {
    return images;
   }

   const int alphaAdd = std::max(1, static_cast<int>(intensity * 12.0f));

   auto buildChannelInline = [&frame, alphaAdd](QImage& outImage, int channel, int scopeW, int scopeH, const QColor& traceColor) {
    outImage = QImage(scopeW, scopeH, QImage::Format_ARGB32_Premultiplied);
    outImage.fill(Qt::black);
    if (frame.isNull() || scopeW <= 0 || scopeH <= 0) return;

    const QImage scaled = frame.scaled(scopeW, frame.height(),
     Qt::IgnoreAspectRatio, Qt::FastTransformation)
     .convertToFormat(QImage::Format_ARGB32);

    const int frameW = scaled.width();
    const int frameH = scaled.height();
    const int tr = traceColor.red();
    const int tg = traceColor.green();
    const int tb = traceColor.blue();

    for (int x = 0; x < frameW && x < scopeW; ++x) {
     for (int fy = 0; fy < frameH; ++fy) {
      QRgb pixel = scaled.pixel(x, fy);
      int value = 0;
      switch (channel) {
      case 0: value = qRed(pixel);   break;
      case 1: value = qGreen(pixel); break;
      case 2: value = qBlue(pixel);  break;
      }
      int sy = scopeH - 1 - (value * (scopeH - 1) / 255);
      sy = std::clamp(sy, 0, scopeH - 1);
      QRgb existing = outImage.pixel(x, sy);
      int er = std::min(255, qRed(existing) + (tr * alphaAdd / 255));
      int eg = std::min(255, qGreen(existing) + (tg * alphaAdd / 255));
      int eb = std::min(255, qBlue(existing) + (tb * alphaAdd / 255));
      outImage.setPixel(x, sy, qRgb(er, eg, eb));
     }
    }
   };

   auto buildLumaInline = [&frame, alphaAdd](QImage& outImage, int scopeW, int scopeH) {
    outImage = QImage(scopeW, scopeH, QImage::Format_ARGB32_Premultiplied);
    outImage.fill(Qt::black);
    if (frame.isNull() || scopeW <= 0 || scopeH <= 0) return;

    const QImage scaled = frame.scaled(scopeW, frame.height(),
     Qt::IgnoreAspectRatio, Qt::FastTransformation)
     .convertToFormat(QImage::Format_ARGB32);

    const int frameW = scaled.width();
    const int frameH = scaled.height();

    for (int x = 0; x < frameW && x < scopeW; ++x) {
     for (int fy = 0; fy < frameH; ++fy) {
      QRgb pixel = scaled.pixel(x, fy);
      int r = qRed(pixel);
      int g = qGreen(pixel);
      int b = qBlue(pixel);
      int luma = static_cast<int>(0.2126 * r + 0.7152 * g + 0.0722 * b);
      int sy = scopeH - 1 - (luma * (scopeH - 1) / 255);
      sy = std::clamp(sy, 0, scopeH - 1);
      QRgb existing = outImage.pixel(x, sy);
      int eg = std::min(255, qGreen(existing) + alphaAdd / 2);
      int eb = std::min(255, qBlue(existing) + alphaAdd);
      outImage.setPixel(x, sy, qRgb(qRed(existing), eg, eb));
     }
    }
   };

   switch (mode) {
   case ParadeMode::RGB:
   {
    const int colW = totalWidth / 3;
    QImage rImg, gImg, bImg;
    buildChannelInline(rImg, 0, colW, height, QColor(255, 60, 60));
    buildChannelInline(gImg, 1, colW, height, QColor(60, 220, 60));
    buildChannelInline(bImg, 2, colW, height, QColor(60, 100, 255));
    images = { rImg, gImg, bImg };
    break;
   }
   case ParadeMode::YCbCr:
   {
    const int colW = totalWidth / 3;
    QImage yImg, cbImg, crImg;
    buildLumaInline(yImg, colW, height);
    // Cb/Cr build (simplified inline)
    images = { yImg, cbImg, crImg };
    break;
   }
   case ParadeMode::YRGB:
   {
    const int colW = totalWidth / 4;
    QImage yImg, rImg, gImg, bImg;
    buildLumaInline(yImg, colW, height);
    buildChannelInline(rImg, 0, colW, height, QColor(255, 60, 60));
    buildChannelInline(gImg, 1, colW, height, QColor(60, 220, 60));
    buildChannelInline(bImg, 2, colW, height, QColor(60, 100, 255));
    images = { yImg, rImg, gImg, bImg };
    break;
   }
   }

   return images;
  }));
 }

 // ==================== ParadeScopeWidget ====================

 ParadeScopeWidget::ParadeScopeWidget(QWidget* parent)
  : AbstractWidget(parent), impl_(new Impl)
 {
  setMinimumSize(300, 150);
 }

 ParadeScopeWidget::~ParadeScopeWidget() {
  delete impl_;
 }

 void ParadeScopeWidget::updateFrame(const QImage& frame) {
  impl_->currentFrame_ = frame;
  impl_->dirty_ = true;
  update();
 }

 void ParadeScopeWidget::setMode(ParadeMode mode) {
  if (impl_->mode_ != mode) {
   impl_->mode_ = mode;
   impl_->dirty_ = true;
   update();
   emit modeChanged(mode);
  }
 }

 ParadeMode ParadeScopeWidget::mode() const {
  return impl_->mode_;
 }

 void ParadeScopeWidget::setIntensity(float intensity) {
  impl_->intensity_ = std::clamp(intensity, 0.0f, 1.0f);
  impl_->dirty_ = true;
  update();
 }

 float ParadeScopeWidget::intensity() const {
  return impl_->intensity_;
 }

 void ParadeScopeWidget::resizeEvent(QResizeEvent* event) {
  AbstractWidget::resizeEvent(event);
  impl_->dirty_ = true;
 }

 void ParadeScopeWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Background
  painter.fillRect(rect(), QColor(20, 20, 20));

  const int marginL = 30;  // Left margin for scale labels
  const int marginR = 10;
  const int marginT = 10;
  const int marginB = 25;  // Bottom margin for channel labels
  const int scopeW = width() - marginL - marginR;
  const int scopeH = height() - marginT - marginB;

  if (scopeW <= 0 || scopeH <= 0) return;

  // Rebuild scope images if needed (async)
  if (impl_->dirty_ && !impl_->computing_) {
   impl_->rebuildScopeImagesAsync(scopeW, scopeH);
  }

  const int numColumns = static_cast<int>(impl_->scopeImages_.size());
  if (numColumns <= 0) return;

  const int colWidth = scopeW / numColumns;
  const int gap = 2; // Gap between columns

  // Draw graticule (horizontal reference lines) spanning full width
  painter.setPen(QPen(QColor(50, 50, 50), 1, Qt::DotLine));
  const int divisions = 10;
  for (int i = 0; i <= divisions; ++i) {
   int y = marginT + (scopeH * i / divisions);
   painter.drawLine(marginL, y, marginL + scopeW, y);

   // Scale labels (IRE / percentage)
   int value = 100 - (i * 100 / divisions);
   painter.setPen(QColor(120, 120, 120));
   painter.setFont(QFont("Consolas", 7));
   painter.drawText(2, y + 4, QString::number(value));
   painter.setPen(QPen(QColor(50, 50, 50), 1, Qt::DotLine));
  }

  // Draw each column
  for (int col = 0; col < numColumns; ++col) {
   int colX = marginL + col * colWidth;
   int drawW = colWidth - gap;

   // Draw the scope image for this column
   if (col < static_cast<int>(impl_->scopeImages_.size()) &&
    !impl_->scopeImages_[col].isNull()) {
    // Scale image to fit column if needed
    QImage& img = impl_->scopeImages_[col];
    if (img.width() != drawW || img.height() != scopeH) {
     painter.drawImage(QRect(colX, marginT, drawW, scopeH),
      img, img.rect());
    } else {
     painter.drawImage(colX, marginT, img);
    }
   }

   // Column border
   painter.setPen(QPen(QColor(60, 60, 60), 1));
   painter.setBrush(Qt::NoBrush);
   painter.drawRect(colX, marginT, drawW, scopeH);

   // Column separator line
   if (col > 0) {
    painter.setPen(QPen(QColor(80, 80, 80), 1));
    painter.drawLine(colX - 1, marginT, colX - 1, marginT + scopeH);
   }

   // Channel label at bottom
   if (col < static_cast<int>(impl_->scopeLabels_.size())) {
    QColor labelColor = (col < static_cast<int>(impl_->scopeColors_.size()))
     ? impl_->scopeColors_[col] : QColor(180, 180, 180);
    painter.setPen(labelColor);
    painter.setFont(QFont("Segoe UI", 8, QFont::Bold));
    int labelX = colX + drawW / 2 - 5;
    painter.drawText(labelX, marginT + scopeH + 15, impl_->scopeLabels_[col]);
   }
  }

  // Title
  painter.setPen(QColor(180, 180, 180));
  painter.setFont(QFont("Segoe UI", 8));
  QString title;
  switch (impl_->mode_) {
  case ParadeMode::RGB:   title = "Parade (RGB)";   break;
  case ParadeMode::YCbCr: title = "Parade (YCbCr)"; break;
  case ParadeMode::YRGB:  title = "Parade (YRGB)";  break;
  }
  painter.drawText(marginL + 40, marginT + scopeH + 15, title);
 }

};
