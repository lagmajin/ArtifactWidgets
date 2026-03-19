module;
#include <QConicalGradient>
#include <QMouseEvent>
#include <QPainter>
#include <QRadialGradient>
#include <QWidget>
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

  void updateColorFromPosition(const QPoint &pos, const QRect &wheelRect) {
    QPointF center = wheelRect.center();
    QPointF delta = QPointF(pos) - center;
    double radius = wheelRect.width() / 2.0;

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

    double radius = wheelRect.width() / 2.0;
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
  painter.setRenderHint(QPainter::Antialiasing);

  QRect wheelRect = rect().adjusted(10, 10, -10, -10);
  int radius = wheelRect.width() / 2;
  QPointF center = wheelRect.center();

  // カラーホイールを描画
  for (int angle = 0; angle < 360; ++angle) {
    double rad = angle * M_PI / 180.0;
    QConicalGradient gradient(center, angle);
    gradient.setColorAt(0, QColor::fromHsv(angle, 0, 255));
    gradient.setColorAt(1, QColor::fromHsv(angle, 255, 255));

    painter.save();
    painter.setClipRect(wheelRect);
    painter.setPen(Qt::NoPen);
    painter.setBrush(gradient);
    painter.drawEllipse(center, radius, radius);
    painter.restore();
  }

  // 中心から外周への彩度グラデーション
  QRadialGradient satGradient(center, radius);
  satGradient.setColorAt(0, Qt::white);
  satGradient.setColorAt(1, Qt::transparent);

  painter.save();
  painter.setClipRect(wheelRect);
  painter.setPen(Qt::NoPen);
  painter.setBrush(satGradient);
  painter.drawEllipse(center, radius, radius);
  painter.restore();

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