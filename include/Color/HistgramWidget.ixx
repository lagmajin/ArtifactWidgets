module;
#include <QtWidgets/QWidget>
#include <QtGui/QImage>
#include <Define/DllExportMacro.hpp>

export module HistgramWidget;



export namespace ArtifactWidgets {

 /// Histogram display mode
 enum class HistogramMode {
  Luma,       // Luminance histogram only
  RGB,        // Overlaid R/G/B histograms
  Parade,     // R, G, B side by side (mini parade in histogram)
  Combined    // Luma + RGB overlaid
 };

 class WIDGET_LIBRARY_DLL_API HistogramWidget : public QWidget {
 private:
  class Impl;
  Impl* impl_;
 protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
 public:
  explicit HistogramWidget(QWidget* parent = nullptr);
  virtual ~HistogramWidget();

  /// Update with a new video frame
  void updateFrame(const QImage& frame);

  /// Set histogram display mode
  void setMode(HistogramMode mode);
  HistogramMode mode() const;

  /// Toggle logarithmic scale
  void setLogScale(bool enabled);
  bool logScale() const;

  /// Compatibility API for the former misspelled entry point.
  void setHistgram();
  void setHistogram();
 };

 // Compatibility alias. Keep the old public name while downstream modules migrate.
 using HistgramWidget = HistogramWidget;

};
