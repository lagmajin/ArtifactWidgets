module;
#include <QtWidgets/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QConicalGradient>
#include <QtGui/QFont>
#include <QtGui/QColor>
#include <QtCore/QRect>
#include <QtCore/QPoint>
#include <algorithm>
#include <cmath>
#include <vector>

module VectorScopeWidget;

import AbstractWidget;

namespace ArtifactWidgets {

 // ==================== Impl ====================
 class VectorScopeWidget::Impl {
 public:
  Impl();
  ~Impl();

  void rebuildScopeImage(int diameter);
  void drawGraticule(QPainter& painter, const QRect& rect, int radius);

  QImage currentFrame_;
  QImage scopeImage_;
  VectorScopeMode mode_ = VectorScopeMode::Standard;
  float intensity_ = 0.5f;
  bool dirty_ = true;
 };

 VectorScopeWidget::Impl::Impl() {}
 VectorScopeWidget::Impl::~Impl() {}

 void VectorScopeWidget::Impl::rebuildScopeImage(int diameter) {
  if (currentFrame_.isNull() || diameter <= 0) {
   scopeImage_ = QImage();
   dirty_ = false;
   return;
  }

  scopeImage_ = QImage(diameter, diameter, QImage::Format_ARGB32_Premultiplied);
  scopeImage_.fill(Qt::transparent);

  const QImage frame = currentFrame_.convertToFormat(QImage::Format_ARGB32);
  const int frameW = frame.width();
  const int frameH = frame.height();
  const int cx = diameter / 2;
  const int cy = diameter / 2;
  const float radius = (diameter / 2.0f) * 0.9f; // 90% of the radius
  const int alphaAdd = std::max(1, static_cast<int>(intensity_ * 8.0f));

  // Step through pixels (subsample for performance)
  const int step = std::max(1, static_cast<int>(std::sqrt(frameW * frameH / 80000.0)));

  for (int fy = 0; fy < frameH; fy += step) {
   for (int fx = 0; fx < frameW; fx += step) {
    QRgb pixel = frame.pixel(fx, fy);
    int r = qRed(pixel);
    int g = qGreen(pixel);
    int b = qBlue(pixel);

    // Convert to Cb, Cr (ITU-R BT.709)
    float cb = -0.1146f * r - 0.3854f * g + 0.5f * b;      // -127.5 .. +127.5
    float cr =  0.5f * r    - 0.4542f * g - 0.0458f * b;    // -127.5 .. +127.5

    // Map to scope coordinates
    int sx = cx + static_cast<int>(cb * radius / 127.5f);
    int sy = cy - static_cast<int>(cr * radius / 127.5f); // Y is inverted

    sx = std::clamp(sx, 0, diameter - 1);
    sy = std::clamp(sy, 0, diameter - 1);

    QRgb existing = scopeImage_.pixel(sx, sy);
    int ea = qAlpha(existing);
    int er = qRed(existing);
    int eg = qGreen(existing);
    int eb = qBlue(existing);

    // Colorize trace by the pixel's own color
    er = std::min(255, er + std::max(1, r * alphaAdd / 128));
    eg = std::min(255, eg + std::max(1, g * alphaAdd / 128));
    eb = std::min(255, eb + std::max(1, b * alphaAdd / 128));
    ea = std::min(255, ea + alphaAdd * 2);

    scopeImage_.setPixel(sx, sy, qRgba(er, eg, eb, ea));
   }
  }

  dirty_ = false;
 }

 void VectorScopeWidget::Impl::drawGraticule(QPainter& painter, const QRect& rect, int radius) {
  const int cx = rect.center().x();
  const int cy = rect.center().y();

  // Outer circle
  painter.setPen(QPen(QColor(60, 60, 60), 1));
  painter.drawEllipse(QPoint(cx, cy), radius, radius);

  // Inner circles (25%, 50%, 75%)
  painter.setPen(QPen(QColor(40, 40, 40), 1, Qt::DotLine));
  painter.drawEllipse(QPoint(cx, cy), radius / 4, radius / 4);
  painter.drawEllipse(QPoint(cx, cy), radius / 2, radius / 2);
  painter.drawEllipse(QPoint(cx, cy), radius * 3 / 4, radius * 3 / 4);

  // Crosshairs
  painter.drawLine(cx - radius, cy, cx + radius, cy);
  painter.drawLine(cx, cy - radius, cx, cy + radius);

  // Color target boxes (SMPTE color bar positions in Cb/Cr space)
  struct ColorTarget {
   const char* name;
   float cb;  // normalized -1..1
   float cr;  // normalized -1..1
   QColor color;
  };

  const ColorTarget targets[] = {
   {"R",   -0.169f,  0.500f, QColor(180, 60, 60)},
   {"G",   -0.331f, -0.419f, QColor(60, 180, 60)},
   {"B",    0.500f, -0.081f, QColor(60, 60, 180)},
   {"Cy",   0.331f, -0.500f, QColor(60, 180, 180)},
   {"Mg",   0.169f,  0.419f, QColor(180, 60, 180)},
   {"Yl",  -0.500f,  0.081f, QColor(180, 180, 60)},
  };

  const float r90 = radius * 0.9f;
  painter.setFont(QFont("Consolas", 7));
  for (const auto& t : targets) {
   int tx = cx + static_cast<int>(t.cb * r90);
   int ty = cy - static_cast<int>(t.cr * r90);
   painter.setPen(QPen(t.color, 1));
   painter.drawRect(tx - 5, ty - 5, 10, 10);
   painter.drawText(tx + 8, ty + 4, t.name);
  }

  // Skin tone line (approximately 123 degrees from Cb axis)
  if (mode_ == VectorScopeMode::Skin) {
   const float skinAngle = 123.0f * M_PI / 180.0f;
   painter.setPen(QPen(QColor(200, 150, 100), 1, Qt::DashLine));
   painter.drawLine(cx, cy,
    cx + static_cast<int>(std::cos(skinAngle) * radius),
    cy - static_cast<int>(std::sin(skinAngle) * radius));
  }
 }

 // ==================== VectorScopeWidget ====================

 VectorScopeWidget::VectorScopeWidget(QWidget* parent)
  : AbstractWidget(parent), impl_(new Impl)
 {
  setMinimumSize(200, 200);
 }

 VectorScopeWidget::~VectorScopeWidget() {
  delete impl_;
 }

 void VectorScopeWidget::updateFrame(const QImage& frame) {
  impl_->currentFrame_ = frame;
  impl_->dirty_ = true;
  update();
 }

 void VectorScopeWidget::setMode(VectorScopeMode mode) {
  if (impl_->mode_ != mode) {
   impl_->mode_ = mode;
   impl_->dirty_ = true;
   update();
   emit modeChanged(mode);
  }
 }

 VectorScopeMode VectorScopeWidget::mode() const {
  return impl_->mode_;
 }

 void VectorScopeWidget::setIntensity(float intensity) {
  impl_->intensity_ = std::clamp(intensity, 0.0f, 1.0f);
  impl_->dirty_ = true;
  update();
 }

 float VectorScopeWidget::intensity() const {
  return impl_->intensity_;
 }

 void VectorScopeWidget::resizeEvent(QResizeEvent* event) {
  AbstractWidget::resizeEvent(event);
  impl_->dirty_ = true;
 }

 void VectorScopeWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Background
  painter.fillRect(rect(), QColor(20, 20, 20));

  // Compute scope area (square, centered)
  const int margin = 15;
  int side = std::min(width(), height()) - margin * 2;
  if (side <= 0) return;
  const int radius = static_cast<int>(side / 2.0f * 0.9f);
  QRect scopeRect((width() - side) / 2, (height() - side) / 2 - 8, side, side);

  // Rebuild scope image if needed
  if (impl_->dirty_) {
   impl_->rebuildScopeImage(side);
  }

  // Draw graticule (behind the trace)
  impl_->drawGraticule(painter, scopeRect, radius);

  // Draw the scope trace image
  if (!impl_->scopeImage_.isNull()) {
   painter.drawImage(scopeRect.topLeft(), impl_->scopeImage_);
  }

  // Title
  painter.setPen(QColor(180, 180, 180));
  painter.setFont(QFont("Segoe UI", 8));
  QString title;
  switch (impl_->mode_) {
  case VectorScopeMode::Standard: title = "Vectorscope"; break;
  case VectorScopeMode::HLS:     title = "Vectorscope (HLS)"; break;
  case VectorScopeMode::Skin:    title = "Vectorscope (Skin)"; break;
  }
  painter.drawText(margin, height() - 5, title);
 }

} // namespace ArtifactWidgets
