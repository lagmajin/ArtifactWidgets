//#include "../../include/Audio/VolumeSlider.hpp"


module VolumeSlider;

namespace ArtifactWidgets {
 
 
 class VolumeSliderPrivate {
 private:

 public:
  VolumeSliderPrivate();
  ~VolumeSliderPrivate();
 };

 VolumeSliderPrivate::VolumeSliderPrivate()
 {

 }

 VolumeSliderPrivate::~VolumeSliderPrivate()
 {

 }

 VolumeSlider::VolumeSlider(QWidget* parent /*= nullptr*/) :QSlider(parent)
 {

 }

 VolumeSlider::~VolumeSlider()
 {

 }


 void VolumeSlider::setVolume(float volume/*=1.0f*/)
 {

 }

};