module;

#include <QWidget>
#include <wobjectimpl.h>
export module Widget.ColorViewLabel;

import Color.Float;

export namespace ArtifactWidgets {

class ColorViewLabel : public QWidget {
	W_OBJECT(ColorViewLabel)
private:
  class Impl;
  Impl *impl_;

protected:
  void paintEvent(QPaintEvent *event) override;

public:
  ColorViewLabel(QWidget *parent = nullptr);
  ~ColorViewLabel();

  ArtifactCore::FloatColor getColor() const;
  void setColor(const ArtifactCore::FloatColor &color);
};

} // namespace ArtifactWidgets