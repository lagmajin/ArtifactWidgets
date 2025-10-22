module;

#include <QWidget>
#include <wobjectimpl.h>

module Widgets.Knob.Rotary;


namespace ArtifactCore {

	W_OBJECT_IMPL(RotaryKnob)

 class RotaryKnob::Impl {
 private:

 public:
  Impl();
  ~Impl();
};

 RotaryKnob::Impl::Impl()
 {

 }

 RotaryKnob::Impl::~Impl()
 {

 }

 RotaryKnob::RotaryKnob(QWidget* parent /*= nullptr*/):QWidget(parent),impl_(new Impl())
 {

 }
	
 RotaryKnob::~RotaryKnob()
 {
  delete impl_;
 }



};