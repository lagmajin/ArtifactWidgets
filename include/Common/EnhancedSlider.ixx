module;
#include <QMouseEvent>
#include <QWheelEvent>
#include <QSlider>
#include <wobjectdefs.h>
#include "../Define/DllExportMacro.hpp"

export module EnhancedSlider;

export namespace ArtifactWidgets {

class WIDGET_LIBRARY_DLL_API EnhancedSlider : public QSlider {
  W_OBJECT(EnhancedSlider)

private:
  class Impl;
  Impl* impl_;

  int valueFromPoint(const QPoint& point) const;

protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

public:
  explicit EnhancedSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
  ~EnhancedSlider();
};

}
