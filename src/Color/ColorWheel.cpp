module;
#include <QConicalGradient>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QRadialGradient>
#include <QWidget>
#include <algorithm>
#include <cmath>
#include <wobjectimpl.h>

module Widgets.ColorWheel;

import Color.Float;

namespace ArtifactWidgets {

W_OBJECT_IMPL(ColorWheelWidget)

class ColorWheelWidget::Impl {
public:
  Impl() = default;
  ~Impl() = default;

  ArtifactCore::FloatColor currentColor{1.0f, 1.0f, 1.0f, 1.0f};
  bool isDragging = false;
  QImage wheelImage;
  QSize wheelImageSize;

  void updateColorFromPosition(const QPoint &pos, const QRect &wheelRect) {
    QPointF center = wheelRect.center();
    QPointF delta = QPointF(pos) - center;
    double radius = std::min(wheelRect.width(), wheelRect.height()) / 2.0;

    double distance = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());
    double sat = std::min(1.0, distance / radius);

    double angle = std::atan2(delta.y(), delta.x()) * 180.0 / M_PI;
    if (angle < 0)
      angle += 360.0;

    // HSVからRGBに変換
    double h = angle;
    double s = sat;
    double v = 1.0;

    int hi = static_cast<int>(h / 60.0) % 6;
    double f = h / 60.0 - hi;
    double p = v * (1.0 - s);
    double q = v * (1.0 - f * s);
    double t = v * (1.0 - (1.0 - f) * s);

    double r, g, b;
    switch (hi) {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    default:
      r = v;
      g = p;
      b = q;
      break;
    }

    currentColor.setColor(static_cast<float>(r), static_cast<float>(g),
                          static_cast<float>(b), currentColor.a());
  }

  QPointF getPositionFromColor(const QRect &wheelRect) const {
    // HSVから位置を計算
    double h, s, v;
    double r = currentColor.r();
    double g = currentColor.g();
    double b = currentColor.b();

    double max = std::max({r, g, b});
    double min = std::min({r, g, b});
    v = max;
    double d = max - min;
    s = max == 0 ? 0 : d / max;

    if (d == 0) {
      h = 0;
    } else {
      if (max == r)
        h = 60 * (fmod((g - b) / d, 6));
      else if (max == g)
        h = 60 * ((b - r) / d + 2);
      else
        h = 60 * ((r - g) / d + 4);
      if (h < 0)
        h += 360;
    }

    double radius = std::min(wheelRect.width(), wheelRect.height()) / 2.0;
    double rad = h * M_PI / 180.0;
    double dist = s * radius;

    QPointF center = wheelRect.center();
    return center + QPointF(std::cos(rad) * dist, std::sin(rad) * dist);
  }
};

ColorWheelWidget::ColorWheelWidget(QWidget *parent)
    : QWidget(parent), impl_(new Impl()) {
  setMinimumSize(150, 150);
}

ColorWheelWidget::~ColorWheelWidget() { delete impl_; }

ArtifactCore::FloatColor ColorWheelWidget::getColor() const {
  return impl_->currentColor;
}

void ColorWheelWidget::setColor(const ArtifactCore::FloatColor &color) {
  impl_->currentColor = color;
  update();
}

void ColorWheelWidget::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

  QRect wheelRect = rect().adjusted(10, 10, -10, -10);
  const int side = std::min(wheelRect.width(), wheelRect.height());
  wheelRect.setSize(QSize(side, side));
  wheelRect.moveCenter(rect().center());
  if (side <= 0) {
    return;
  }

  const qreal dpr = devicePixelRatioF();
  const QSize pixelSize(std::max(1, static_cast<int>(std::round(side * dpr))),
                        std::max(1, static_cast<int>(std::round(side * dpr))));
  if (impl_->wheelImage.isNull() || impl_->wheelImageSize != pixelSize) {
    impl_->wheelImage = QImage(pixelSize, QImage::Format_ARGB32_Premultiplied);
    impl_->wheelImage.setDevicePixelRatio(dpr);
    impl_->wheelImageSize = pixelSize;

    const int w = impl_->wheelImage.width();
    const int h = impl_->wheelImage.height();
    const float cx = (w - 1) * 0.5f;
    const float cy = (h - 1) * 0.5f;
    const float radius = std::min(w, h) * 0.5f - 1.0f;

    for (int y = 0; y < h; ++y) {
      QRgb* line = reinterpret_cast<QRgb*>(impl_->wheelImage.scanLine(y));
      for (int x = 0; x < w; ++x) {
        const float dx = (static_cast<float>(x) - cx);
        const float dy = (static_cast<float>(y) - cy);
        const float dist = std::sqrt(dx * dx + dy * dy);
        if (dist > radius) {
          line[x] = qRgba(0, 0, 0, 0);
          continue;
        }

        float hue = std::atan2(dy, dx) * 180.0f / static_cast<float>(M_PI);
        if (hue < 0.0f) hue += 360.0f;
        const float sat = std::clamp(dist / radius, 0.0f, 1.0f);
        const QColor c = QColor::fromHsvF(hue / 360.0f, sat, 1.0f, 1.0f);
        line[x] = c.rgba();
      }
    }
  }

  painter.drawImage(wheelRect, impl_->wheelImage);
  painter.setPen(QPen(QColor(20, 20, 20, 220), 1.0));
  painter.setBrush(Qt::NoBrush);
  painter.drawEllipse(wheelRect);

  // 現在の色の位置にマーカーを描画
  QPointF markerPos = impl_->getPositionFromColor(wheelRect);
  painter.setPen(QPen(Qt::black, 2));
  painter.setBrush(Qt::NoBrush);
  painter.drawEllipse(markerPos, 8, 8);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor::fromRgbF(impl_->currentColor.r(),
                                    impl_->currentColor.g(),
                                    impl_->currentColor.b()));
  painter.drawEllipse(markerPos, 6, 6);
}

void ColorWheelWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    QRect wheelRect = rect().adjusted(10, 10, -10, -10);
    impl_->updateColorFromPosition(event->pos(), wheelRect);
    impl_->isDragging = true;
    update();
    emit colorChanged(impl_->currentColor);
  }
}

void ColorWheelWidget::mouseMoveEvent(QMouseEvent *event) {
  if (impl_->isDragging && (event->buttons() & Qt::LeftButton)) {
    QRect wheelRect = rect().adjusted(10, 10, -10, -10);
    impl_->updateColorFromPosition(event->pos(), wheelRect);
    update();
    emit colorChanged(impl_->currentColor);
  }
}

void ColorWheelWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    impl_->isDragging = false;
  }
}

} // namespace ArtifactWidgets
