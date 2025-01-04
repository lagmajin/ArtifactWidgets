#pragma once

#include <memory>
#include <QtWidgets/QSlider>





namespace ArtifactWidgets {

 class VolumeSliderPrivate;

 class __declspec(dllexport) VolumeSlider :public QSlider{
  Q_OBJECT
 private:

 public:
  explicit VolumeSlider(QWidget* parent = nullptr);
  virtual ~VolumeSlider();
 signals:

 public slots:
  void setVolume(float volume=1.0f);
 };







}