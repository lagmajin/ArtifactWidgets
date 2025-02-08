module;
#include <memory>
#include <QtWidgets/QSlider>
#include <wobjectdefs.h>

export module VolumeSlider;




namespace ArtifactWidgets {

 class VolumeSliderPrivate;

 class __declspec(dllexport) VolumeSlider :public QSlider{
  //Q_OBJECT
  W_OBJECT(VolumeSlider)
 private:

 public:
  explicit VolumeSlider(QWidget* parent = nullptr);
  virtual ~VolumeSlider();
 signals:

 public slots:
  void setVolume(float volume=1.0f);
 };







}