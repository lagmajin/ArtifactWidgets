module;

//import std.core;

//
#include <wobjectdefs.h>
#include <QtWidgets/QCheckBox>

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
export module Widgets.Knob.CheckBox;







export namespace ArtifactWidgets {

 class KnobCheckBoxPrivate;

 class KnobCheckBox :public QCheckBox{
  W_OBJECT(KnobCheckBox);
 private:
  class Impl;
  Impl* d;
 public:
  explicit KnobCheckBox(QWidget* parent = nullptr);;
  ~KnobCheckBox();
  bool checkBox() const;
  void setValue(bool b);

 };







};