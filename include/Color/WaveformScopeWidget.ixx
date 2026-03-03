module;

#include <QtWidgets/QWidget>
#include <QtGui/QImage>
#include <wobjectdefs.h>

export module WaveformScopeWidget;

import AbstractWidget;

export namespace ArtifactWidgets {

 /// Waveform display mode
 enum class WaveformMode {
  Luma,       // Luminance only (classic waveform)
  RGB,        // Overlaid R/G/B waveforms
  YCbCr       // Y, Cb, Cr components
 };

 /// Video waveform scope widget (luminance / RGB waveform)
 /// Displays brightness distribution across the horizontal axis of the frame.
 class WaveformScopeWidget : public AbstractWidget {
  W_OBJECT(WaveformScopeWidget)
 private:
  class Impl;
  Impl* impl_;
 protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
 public:
  explicit WaveformScopeWidget(QWidget* parent = nullptr);
  ~WaveformScopeWidget();

  /// Update the scope with a new video frame
  void updateFrame(const QImage& frame);

  /// Set the waveform display mode
  void setMode(WaveformMode mode);
  WaveformMode mode() const;

  /// Set intensity / brightness of the scope trace
  void setIntensity(float intensity);
  float intensity() const;

 signals:
  void modeChanged(WaveformMode mode) W_SIGNAL(modeChanged, mode);
 };

};
