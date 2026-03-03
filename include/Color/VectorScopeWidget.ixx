module;
#include <QtCore/QSharedPointer>

#include <QtWidgets/QWidget>
#include <QtGui/QImage>
#include <wobjectdefs.h>
export module VectorScopeWidget;

import AbstractWidget;

export namespace ArtifactWidgets {

 /// Vectorscope display mode
 enum class VectorScopeMode {
  Standard,     // Standard vectorscope (Cb vs Cr)
  HLS,          // Hue-Lightness-Saturation
  Skin           // With skin tone indicator line
 };

 /// Video vectorscope widget
 /// Displays chrominance information on a circular plot.
 /// X-axis = Cb (blue difference), Y-axis = Cr (red difference)
 class VectorScopeWidget :public AbstractWidget{
 W_OBJECT(VectorScopeWidget)
 private:
  class Impl;
  Impl* impl_;
 protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
 public:
  explicit VectorScopeWidget(QWidget* parent = nullptr);
  ~VectorScopeWidget();

  /// Update with a new video frame
  void updateFrame(const QImage& frame);

  /// Set display mode
  void setMode(VectorScopeMode mode);
  VectorScopeMode mode() const;

  /// Set trace intensity
  void setIntensity(float intensity);
  float intensity() const;

 signals:
  void modeChanged(VectorScopeMode mode) W_SIGNAL(modeChanged, mode);
 };

}