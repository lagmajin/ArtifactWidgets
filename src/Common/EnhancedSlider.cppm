module;

#include <algorithm>

#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QStyle>
#include <QStyleOptionSlider>
#include <QVariant>
#include <wobjectimpl.h>

module EnhancedSlider;

namespace ArtifactWidgets {

W_OBJECT_IMPL(EnhancedSlider)

namespace {

QStyleOptionSlider makeSliderOption(const QSlider* slider)
{
  QStyleOptionSlider option;
  option.initFrom(slider);
  option.orientation = slider->orientation();
  option.minimum = slider->minimum();
  option.maximum = slider->maximum();
  option.sliderPosition = slider->sliderPosition();
  option.sliderValue = slider->value();
  option.singleStep = slider->singleStep();
  option.pageStep = slider->pageStep();
  option.upsideDown = slider->invertedAppearance();
  option.tickPosition = slider->tickPosition();
  option.tickInterval = slider->tickInterval();
  option.rect = slider->rect();
  option.subControls = QStyle::SC_SliderGroove | QStyle::SC_SliderHandle;
  option.activeSubControls = QStyle::SC_None;
  return option;
}

int coordinateAlongTrack(const QSlider* slider, const QPoint& point)
{
  return slider->orientation() == Qt::Horizontal ? point.x() : point.y();
}

QColor propertyColor(const QObject* object, const char* name, const QColor& fallback)
{
  if (!object) {
    return fallback;
  }
  const QVariant value = object->property(name);
  if (!value.isValid()) {
    return fallback;
  }
  const QColor color = value.value<QColor>();
  return color.isValid() ? color : fallback;
}

}

class EnhancedSlider::Impl {
public:
  bool dragging = false;
  int dragOffset = 0;
};

EnhancedSlider::EnhancedSlider(Qt::Orientation orientation, QWidget* parent)
    : QSlider(orientation, parent), impl_(new Impl)
{
}

EnhancedSlider::~EnhancedSlider()
{
  delete impl_;
}

int EnhancedSlider::valueFromPoint(const QPoint& point) const
{
  const QStyleOptionSlider option = makeSliderOption(this);
  QStyle* style = this->style();
  const QRect grooveRect =
      style->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, this);
  if (!grooveRect.isValid()) {
    return value();
  }

  const int minValue = minimum();
  const int maxValue = maximum();
  if (maxValue <= minValue) {
    return minValue;
  }

  if (orientation() == Qt::Horizontal) {
    const int span = std::max(1, grooveRect.width() - 1);
    const int position = std::clamp(point.x() - grooveRect.left(), 0, span);
    return QStyle::sliderValueFromPosition(
        minValue, maxValue, position, span, option.upsideDown);
  }

  const int span = std::max(1, grooveRect.height() - 1);
  const int positionFromTop = std::clamp(point.y() - grooveRect.top(), 0, span);
  const int position = option.upsideDown ? positionFromTop : (span - positionFromTop);
  return QStyle::sliderValueFromPosition(minValue, maxValue, position, span, false);
}

void EnhancedSlider::mousePressEvent(QMouseEvent* event)
{
  if (!event || event->button() != Qt::LeftButton) {
    QSlider::mousePressEvent(event);
    return;
  }

  setFocus(Qt::MouseFocusReason);

  const QStyleOptionSlider option = makeSliderOption(this);
  QStyle* style = this->style();
  const QRect handleRect =
      style->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this);
  const QRect expandedHandle = handleRect.adjusted(-6, -6, 6, 6);
  const QPoint point = event->position().toPoint();

  impl_->dragging = true;
  setSliderDown(true);
  if (expandedHandle.contains(point)) {
    const int center = orientation() == Qt::Horizontal ? handleRect.center().x()
                                                       : handleRect.center().y();
    impl_->dragOffset = coordinateAlongTrack(this, point) - center;
  } else {
    impl_->dragOffset = 0;
    const int nextValue = valueFromPoint(point);
    if (nextValue != value()) {
      setValue(nextValue);
    }
  }

  event->accept();
}

void EnhancedSlider::mouseMoveEvent(QMouseEvent* event)
{
  if (impl_->dragging && event && (event->buttons() & Qt::LeftButton)) {
    QPoint point = event->position().toPoint();
    if (orientation() == Qt::Horizontal) {
      point.setX(point.x() - impl_->dragOffset);
    } else {
      point.setY(point.y() - impl_->dragOffset);
    }

    const int nextValue = valueFromPoint(point);
    if (nextValue != value()) {
      setValue(nextValue);
    }
    event->accept();
    return;
  }

  QSlider::mouseMoveEvent(event);
}

void EnhancedSlider::mouseReleaseEvent(QMouseEvent* event)
{
  if (impl_->dragging && event && event->button() == Qt::LeftButton) {
    impl_->dragging = false;
    impl_->dragOffset = 0;
    setSliderDown(false);
    event->accept();
    return;
  }

  QSlider::mouseReleaseEvent(event);
}

void EnhancedSlider::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event);

  QStyleOptionSlider option = makeSliderOption(this);
  QStyle* style = this->style();
  const QRect grooveRect =
      style->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, this);
  const QRect handleRect =
      style->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this);
  if (!grooveRect.isValid() || !handleRect.isValid()) {
    QSlider::paintEvent(event);
    return;
  }

  const QColor fillColor =
      propertyColor(this, "artifactFillColor", palette().color(QPalette::Highlight));
  const QColor trackColor =
      propertyColor(this, "artifactTrackColor", palette().color(QPalette::Mid));
  const QColor handleColor =
      propertyColor(this, "artifactHandleColor", palette().color(QPalette::Base));
  const QColor borderColor = fillColor.lighter(118);

  QPainter painter(this);
  painter.save();
  painter.setRenderHint(QPainter::Antialiasing, true);

  QRectF trackRect = grooveRect;
  if (orientation() == Qt::Horizontal) {
    trackRect.setTop(trackRect.center().y() - 3.0);
    trackRect.setBottom(trackRect.center().y() + 3.0);
  } else {
    trackRect.setLeft(trackRect.center().x() - 3.0);
    trackRect.setRight(trackRect.center().x() + 3.0);
  }

  painter.setPen(Qt::NoPen);
  painter.setBrush(trackColor);
  painter.drawRoundedRect(trackRect, 3.0, 3.0);

  const double denom = std::max(1, maximum() - minimum());
  const double t = std::clamp((value() - minimum()) / denom, 0.0, 1.0);
  QRectF fillRect = trackRect;
  if (orientation() == Qt::Horizontal) {
    fillRect.setRight(trackRect.left() + trackRect.width() * t);
  } else {
    fillRect.setTop(trackRect.bottom() - trackRect.height() * t);
  }
  if (fillRect.isValid() && fillRect.width() > 0.0 && fillRect.height() > 0.0) {
    painter.setBrush(fillColor);
    painter.drawRoundedRect(fillRect, 3.0, 3.0);
  }

  const QPointF handleCenter = handleRect.center();
  const qreal radius =
      orientation() == Qt::Horizontal ? std::max<qreal>(6.0, handleRect.height() * 0.42)
                                      : std::max<qreal>(6.0, handleRect.width() * 0.42);
  painter.setPen(QPen(borderColor, 1.0));
  painter.setBrush(handleColor);
  painter.drawEllipse(handleCenter, radius, radius);
  painter.restore();
}

}
