module;
#include <QWidget>
#include <QPainter>
#include "qevent.h"
module Widgets.ColorWheel;



namespace ArtifactWidgets
{
 class ColorWheelWidget::Impl
 {
 private:

 public:
  void drawColorWheel(QPainter& painter, const QRect& rect);
  QPointF hsvToPoint(double hue, double sat, int radius);
  void updateHSFromMouse(const QPoint& pos);
  float currentHue = 0.0f;
  float currentSat = 0.0f;
 };

 void ColorWheelWidget::Impl::drawColorWheel(QPainter& painter, const QRect& rect)
 {
  QImage img(rect.size(), QImage::Format_ARGB32);
  QPoint center = rect.center();
  int radius = rect.width() / 2;

  for (int y = 0; y < rect.height(); ++y) {
   for (int x = 0; x < rect.width(); ++x) {
    int dx = x - rect.width() / 2;
    int dy = y - rect.height() / 2;
    double r = std::sqrt(dx * dx + dy * dy);

    if (r <= radius) {
     double angle = std::atan2(dy, dx) * 180.0 / M_PI;
     if (angle < 0) angle += 360;

     double sat = r / radius;  // 0=中心, 1=外周
     QColor c = QColor::fromHsv(angle, sat * 255, 200); // V=200ぐらいに固定
     img.setPixel(x, y, c.rgb());
    }
    else {
     img.setPixel(x, y, qRgba(0, 0, 0, 0));
    }
   }
  }
  painter.drawImage(rect.topLeft(), img);
 }

 QPointF ColorWheelWidget::Impl::hsvToPoint(double hue, double sat, int radius)
 {
  double rad = hue * M_PI / 180.0;
  double r = sat * radius;
  return QPointF(std::cos(rad) * r, std::sin(rad) * r);
 }

 void ColorWheelWidget::Impl::updateHSFromMouse(const QPoint& pos)
 {
    /*
  QPointF delta = pos - QPointF(width() / 2, height() / 2);
  double angle = std::atan2(delta.y(), delta.x()) * 180.0 / M_PI;
  if (angle < 0) angle += 360;

  double r = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());
  double sat = std::min(1.0, r / (width() / 2.0));

  currentHue = angle;
  currentSat = sat;
  update(); // 再描画
  emit colorChanged(QColor::fromHsv(currentHue, currentSat * 255, 255));
  */
 }

 ColorWheelWidget::ColorWheelWidget(QWidget* parent /*= nullptr*/) : QWidget(parent),impl_(new Impl)
 {
  setMinimumSize(150, 150);
 }

 ColorWheelWidget::~ColorWheelWidget()
 {
  delete impl_;
 }

 void ColorWheelWidget::mousePressEvent(QMouseEvent* event)
 {
  //updateHSFromMouse(event->pos());
 }

 void ColorWheelWidget::mouseMoveEvent(QMouseEvent* event)
 {
  if (event->buttons() & Qt::LeftButton)
  {
   //updateHSFromMouse(event->pos());
  }
   
 }


 void ColorWheelWidget::paintEvent(QPaintEvent* event)
 {


 }

}

