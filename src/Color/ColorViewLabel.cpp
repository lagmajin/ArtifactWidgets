module;
#include <QPainter>
#include <QWidget>
#include <wobjectimpl.h>

module Widget.ColorViewLabel;

import Color.Float;

namespace ArtifactWidgets {

W_OBJECT_IMPL(ColorViewLabel)

class ColorViewLabel::Impl {
public:
  Impl() = default;
  ~Impl() = default;

  ArtifactCore::FloatColor currentColor{1.0f, 1.0f, 1.0f, 1.0f};
};

ColorViewLabel::ColorViewLabel(QWidget *parent)
    : QWidget(parent), impl_(new Impl()) {
  setMinimumSize(40, 40);
}

ColorViewLabel::~ColorViewLabel() { delete impl_; }

ArtifactCore::FloatColor ColorViewLabel::getColor() const {
  return impl_->currentColor;
}

void ColorViewLabel::setColor(const ArtifactCore::FloatColor &color) {
  impl_->currentColor = color;
  update();
}

void ColorViewLabel::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // 背景を描画（チェッカーボードパターンで透明度を表現）
  int gridSize = 8;
  for (int y = 0; y < height(); y += gridSize) {
    for (int x = 0; x < width(); x += gridSize) {
      bool isLight = ((x / gridSize) + (y / gridSize)) % 2 == 0;
      painter.fillRect(x, y, gridSize, gridSize,
                       isLight ? QColor(255, 255, 255) : QColor(200, 200, 200));
    }
  }

  // 色を描画
  QColor color =
      QColor::fromRgbF(impl_->currentColor.r(), impl_->currentColor.g(),
                       impl_->currentColor.b(), impl_->currentColor.a());
  painter.fillRect(rect(), color);

  // 枠線を描画
  painter.setPen(QPen(Qt::black, 1));
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(rect().adjusted(0, 0, -1, -1));
}

} // namespace ArtifactWidgets