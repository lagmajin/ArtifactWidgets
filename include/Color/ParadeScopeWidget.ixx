module;



#include <QtWidgets/QWidget>
#include <QtGui/QImage>
#include <wobjectdefs.h>
#include <Define/DllExportMacro.hpp>
//#include <Wobject>

export module ParadeScopeWidget;
import AbstractWidget;
export namespace ArtifactWidgets {

 /// Parade display mode
 enum class ParadeMode {
  RGB,        // Red, Green, Blue side by side
  YCbCr,      // Y, Cb, Cr side by side
  YRGB        // Luma + R, G, B (4 columns)
 };

  class WIDGET_LIBRARY_DLL_API ParadeScopeWidget :public AbstractWidget{
 W_OBJECT(ParadeScopeWidget)
 private:
  class Impl;
  Impl* impl_;
 protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
 public:
  ParadeScopeWidget(QWidget* parent = nullptr);
  ~ParadeScopeWidget();

  /// Update with a new video frame
  void updateFrame(const QImage& frame);

  /// Set parade display mode
  void setMode(ParadeMode mode);
  ParadeMode mode() const;

  /// Set trace intensity
  void setIntensity(float intensity);
  float intensity() const;

 signals:
  void modeChanged(ParadeMode mode) W_SIGNAL(modeChanged, mode);
 };

};

W_REGISTER_ARGTYPE(ArtifactWidgets::ParadeMode)
