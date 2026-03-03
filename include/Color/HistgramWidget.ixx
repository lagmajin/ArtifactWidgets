module;
#include <QtWidgets/QWidget>
#include <QtGui/QImage>

export module HistgramWidget;



export namespace ArtifactWidgets {

 /// Histogram display mode
 enum class HistogramMode {
  Luma,       // Luminance histogram only
  RGB,        // Overlaid R/G/B histograms
  Parade,     // R, G, B side by side (mini parade in histogram)
  Combined    // Luma + RGB overlaid
 };

 class HistgramWidget :public QWidget{
 private:
  class Impl;
  Impl* impl_;
 protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
 public:
  explicit HistgramWidget(QWidget* parent = nullptr);
  virtual ~HistgramWidget();

  /// Update with a new video frame
  void updateFrame(const QImage& frame);

  /// Set histogram display mode
  void setMode(HistogramMode mode);
  HistogramMode mode() const;

  /// Toggle logarithmic scale
  void setLogScale(bool enabled);
  bool logScale() const;

  /// Legacy API
  void setHistgram();
 };

};