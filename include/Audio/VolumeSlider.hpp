#pragma once
#include <QtWidgets/QSlider>





namespace ArtifactWidgets {

 class __declspec(dllexport) VolumeSlider :public QSlider{
 private:

 public:
  explicit VolumeSlider(QWidget* parent = nullptr);
  virtual ~VolumeSlider();
 };







}