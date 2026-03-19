module;

#include <QWidget>
#include <wobjectdefs.h>

export module Widgets.ColorWheel;

import Color.Float;

export namespace ArtifactWidgets {

class ColorWheelWidget : public QWidget {
  W_OBJECT(ColorWheelWidget)
private:
  class Impl;
  Impl *impl_;

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

public:
  explicit ColorWheelWidget(QWidget *parent = nullptr);
  ~ColorWheelWidget();

  ArtifactCore::FloatColor getColor() const;
  void setColor(const ArtifactCore::FloatColor &color);

signals:
  void colorChanged(const ArtifactCore::FloatColor &color)
      W_SIGNAL(colorChanged, color);
};

} // namespace ArtifactWidgets