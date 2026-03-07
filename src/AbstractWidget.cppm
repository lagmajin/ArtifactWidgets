//#include "../include/Common/AbstractWidget.hpp"

module;
#include <wobjectimpl.h>
#include <qevent.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <cmath>
#include <functional>
#include <optional>
#include <utility>
#include <array>
#include <mutex>
#include <thread>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <any>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <deque>
#include <list>
#include <tuple>
#include <numeric>
#include <regex>
#include <random>
module AbstractWidget;





import Widgets.Effects.GlowFrame;

namespace ArtifactWidgets {

 W_OBJECT_IMPL(AbstractWidget)

 class AbstractWidget::Impl {
 private:

 
 public:
  Impl();
  ~Impl();
  GlowFrame* glowFrame = nullptr;
 };

 AbstractWidget::Impl::Impl()
 {

 }

 AbstractWidget::Impl::~Impl()
 {

 }

 AbstractWidget::AbstractWidget(QWidget* parent /*= nullptr*/) :QWidget(parent),impl_(new Impl())
 {
  //impl_->glowFrame = new GlowFrame(this);
  //impl_->glowFrame->setGeometry(rect());
  //impl_->glowFrame->show();
 }

 AbstractWidget::~AbstractWidget()
 {
  delete impl_;
 }

 void AbstractWidget::focusInEvent(QFocusEvent* event)
 {
  //impl_->glowFrame->setFocused(true);
 }

 void AbstractWidget::focusOutEvent(QFocusEvent* event)
 {
  //impl_->glowFrame->setFocused(false);
 }

 void AbstractWidget::resizeEvent(QResizeEvent* event)
 {
  //impl_->glowFrame->resize(event->size());
 }

 void AbstractWidget::setEnableFocusFrameGlow(bool isEnabled/*=true*/)
 {

 }

}