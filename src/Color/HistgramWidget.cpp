module;
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QPainterPath>
#include <algorithm>
#include <cmath>
#include <array>

module HistgramWidget;

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




namespace ArtifactWidgets {

 // ==================== Impl ====================
 class HistgramWidget::Impl {
 public:
  Impl();
  ~Impl();

  void computeHistogram(const QImage& frame);
  void drawChannel(QPainter& painter, const QRect& rect,
   const std::array<int, 256>& data, int maxVal,
   const QColor& color, bool filled);

  QImage currentFrame_;
  std::array<int, 256> histR_{};
  std::array<int, 256> histG_{};
  std::array<int, 256> histB_{};
  std::array<int, 256> histLuma_{};
  int maxCount_ = 1;
  HistogramMode mode_ = HistogramMode::RGB;
  bool logScale_ = false;
  bool dirty_ = true;
 };

 HistgramWidget::Impl::Impl() {
  histR_.fill(0);
  histG_.fill(0);
  histB_.fill(0);
  histLuma_.fill(0);
 }

 HistgramWidget::Impl::~Impl() {}

 void HistgramWidget::Impl::computeHistogram(const QImage& frame) {
  histR_.fill(0);
  histG_.fill(0);
  histB_.fill(0);
  histLuma_.fill(0);
  maxCount_ = 1;

  if (frame.isNull()) return;

  const QImage img = frame.convertToFormat(QImage::Format_ARGB32);
  const int w = img.width();
  const int h = img.height();

  // Subsample for performance on large frames
  const int step = std::max(1, static_cast<int>(std::sqrt(w * h / 500000.0)));

  for (int y = 0; y < h; y += step) {
   const QRgb* line = reinterpret_cast<const QRgb*>(img.constScanLine(y));
   for (int x = 0; x < w; x += step) {
    QRgb pixel = line[x];
    int r = qRed(pixel);
    int g = qGreen(pixel);
    int b = qBlue(pixel);

    histR_[r]++;
    histG_[g]++;
    histB_[b]++;

    // BT.709 luminance
    int luma = static_cast<int>(0.2126 * r + 0.7152 * g + 0.0722 * b);
    luma = std::clamp(luma, 0, 255);
    histLuma_[luma]++;
   }
  }

  // Find max count for normalization
  for (int i = 0; i < 256; ++i) {
   maxCount_ = std::max(maxCount_, histR_[i]);
   maxCount_ = std::max(maxCount_, histG_[i]);
   maxCount_ = std::max(maxCount_, histB_[i]);
   maxCount_ = std::max(maxCount_, histLuma_[i]);
  }

  dirty_ = false;
 }

 void HistgramWidget::Impl::drawChannel(QPainter& painter, const QRect& rect,
  const std::array<int, 256>& data, int maxVal,
  const QColor& color, bool filled)
 {
  if (maxVal <= 0 || rect.width() <= 0 || rect.height() <= 0) return;

  const float binWidth = rect.width() / 256.0f;
  const int h = rect.height();

  QPainterPath path;
  path.moveTo(rect.left(), rect.bottom());

  for (int i = 0; i < 256; ++i) {
   float val = static_cast<float>(data[i]);

   float normalized;
   if (logScale_) {
    normalized = (val > 0) ? std::log(val + 1.0f) / std::log(static_cast<float>(maxVal) + 1.0f) : 0.0f;
   } else {
    normalized = val / static_cast<float>(maxVal);
   }

   float x = rect.left() + i * binWidth;
   float y = rect.bottom() - normalized * h;
   path.lineTo(x, y);
  }

  path.lineTo(rect.right(), rect.bottom());
  path.closeSubpath();

  if (filled) {
   QColor fillColor = color;
   fillColor.setAlpha(80);
   painter.setBrush(fillColor);
   painter.setPen(Qt::NoPen);
   painter.drawPath(path);
  }

  // Draw the outline
  painter.setBrush(Qt::NoBrush);
  painter.setPen(QPen(color, 1));

  // Re-draw just the top edge
  QPainterPath linePath;
  linePath.moveTo(rect.left(), rect.bottom());
  for (int i = 0; i < 256; ++i) {
   float val = static_cast<float>(data[i]);
   float normalized;
   if (logScale_) {
    normalized = (val > 0) ? std::log(val + 1.0f) / std::log(static_cast<float>(maxVal) + 1.0f) : 0.0f;
   } else {
    normalized = val / static_cast<float>(maxVal);
   }
   float x = rect.left() + i * binWidth;
   float y = rect.bottom() - normalized * h;
   linePath.lineTo(x, y);
  }
  painter.drawPath(linePath);
 }

 // ==================== HistgramWidget ====================

 HistgramWidget::HistgramWidget(QWidget* parent)
  : QWidget(parent), impl_(new Impl)
 {
  setMinimumSize(200, 120);
 }

 HistgramWidget::~HistgramWidget() {
  delete impl_;
 }

 void HistgramWidget::setHistgram() {
  // Legacy: trigger repaint
  update();
 }

 void HistgramWidget::updateFrame(const QImage& frame) {
  impl_->currentFrame_ = frame;
  impl_->dirty_ = true;
  update();
 }

 void HistgramWidget::setMode(HistogramMode mode) {
  impl_->mode_ = mode;
  impl_->dirty_ = true;
  update();
 }

 HistogramMode HistgramWidget::mode() const {
  return impl_->mode_;
 }

 void HistgramWidget::setLogScale(bool enabled) {
  impl_->logScale_ = enabled;
  update(); // No recompute needed, just re-render
 }

 bool HistgramWidget::logScale() const {
  return impl_->logScale_;
 }

 void HistgramWidget::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
 }

 void HistgramWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Background
  painter.fillRect(rect(), QColor(20, 20, 20));

  const int marginL = 10;
  const int marginR = 10;
  const int marginT = 10;
  const int marginB = 25;
  const QRect plotRect(marginL, marginT, width() - marginL - marginR, height() - marginT - marginB);

  if (plotRect.width() <= 0 || plotRect.height() <= 0) return;

  // Recompute if dirty
  if (impl_->dirty_) {
   impl_->computeHistogram(impl_->currentFrame_);
  }

  // Draw grid lines
  painter.setPen(QPen(QColor(45, 45, 45), 1, Qt::DotLine));
  for (int i = 1; i < 4; ++i) {
   int x = plotRect.left() + plotRect.width() * i / 4;
   painter.drawLine(x, plotRect.top(), x, plotRect.bottom());
  }
  for (int i = 1; i < 4; ++i) {
   int y = plotRect.top() + plotRect.height() * i / 4;
   painter.drawLine(plotRect.left(), y, plotRect.right(), y);
  }

  // Draw histograms based on mode
  switch (impl_->mode_) {
  case HistogramMode::Luma:
   impl_->drawChannel(painter, plotRect, impl_->histLuma_, impl_->maxCount_,
    QColor(200, 200, 200), true);
   break;

  case HistogramMode::RGB:
  case HistogramMode::Combined:
   // Draw in order: B, G, R for proper layering
   impl_->drawChannel(painter, plotRect, impl_->histB_, impl_->maxCount_,
    QColor(60, 100, 255), true);
   impl_->drawChannel(painter, plotRect, impl_->histG_, impl_->maxCount_,
    QColor(60, 220, 60), true);
   impl_->drawChannel(painter, plotRect, impl_->histR_, impl_->maxCount_,
    QColor(255, 60, 60), true);

   if (impl_->mode_ == HistogramMode::Combined) {
    impl_->drawChannel(painter, plotRect, impl_->histLuma_, impl_->maxCount_,
     QColor(200, 200, 200, 150), false);
   }
   break;

  case HistogramMode::Parade:
  {
   // Split into 3 regions for R, G, B
   int third = plotRect.width() / 3;
   QRect rRect(plotRect.left(), plotRect.top(), third - 2, plotRect.height());
   QRect gRect(plotRect.left() + third, plotRect.top(), third - 2, plotRect.height());
   QRect bRect(plotRect.left() + third * 2, plotRect.top(), third - 2, plotRect.height());

   impl_->drawChannel(painter, rRect, impl_->histR_, impl_->maxCount_,
    QColor(255, 60, 60), true);
   impl_->drawChannel(painter, gRect, impl_->histG_, impl_->maxCount_,
    QColor(60, 220, 60), true);
   impl_->drawChannel(painter, bRect, impl_->histB_, impl_->maxCount_,
    QColor(60, 100, 255), true);

   // Separator lines
   painter.setPen(QPen(QColor(80, 80, 80), 1));
   painter.drawLine(plotRect.left() + third - 1, plotRect.top(),
    plotRect.left() + third - 1, plotRect.bottom());
   painter.drawLine(plotRect.left() + third * 2 - 1, plotRect.top(),
    plotRect.left() + third * 2 - 1, plotRect.bottom());
  }
  break;
  }

  // Border
  painter.setPen(QPen(QColor(80, 80, 80), 1));
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(plotRect);

  // Scale labels
  painter.setPen(QColor(100, 100, 100));
  painter.setFont(QFont("Consolas", 7));
  painter.drawText(plotRect.left(), plotRect.bottom() + 12, "0");
  painter.drawText(plotRect.left() + plotRect.width() / 2 - 8, plotRect.bottom() + 12, "128");
  painter.drawText(plotRect.right() - 18, plotRect.bottom() + 12, "255");

  // Title
  painter.setPen(QColor(180, 180, 180));
  painter.setFont(QFont("Segoe UI", 8));
  QString title;
  switch (impl_->mode_) {
  case HistogramMode::Luma:    title = "Histogram (Luma)"; break;
  case HistogramMode::RGB:     title = "Histogram (RGB)"; break;
  case HistogramMode::Parade:  title = "Histogram (Parade)"; break;
  case HistogramMode::Combined:title = "Histogram (Combined)"; break;
  }
  if (impl_->logScale_) title += " [Log]";
  painter.drawText(plotRect.left() + 60, plotRect.bottom() + 12, title);
 }

};