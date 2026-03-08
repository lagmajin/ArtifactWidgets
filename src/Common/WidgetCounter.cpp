#include <QHash>
#include "QCoreApplication"
#include "../../../ArtifactWidgets/include/Common/WidgetCounter.ixx"

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
#include <wobjectimpl.h>

namespace ArtifactWidgets {

	W_OBJECT_IMPL(WidgetCounter)

class WidgetCounter::Impl{
private:
 std::mutex mtx_;
 QHash<QString, int> m_widgetCounts;
 static WidgetCounter* s_instance;
public:
 Impl();
 ~Impl();
 void increment();
 void decrement();
 static WidgetCounter& getInstance();
};

WidgetCounter::Impl::~Impl()
{
 s_instance = nullptr;
}

WidgetCounter::Impl::Impl()
{

}

WidgetCounter& WidgetCounter::Impl::getInstance()
{
 if (!s_instance) {
  s_instance = new WidgetCounter();
  // QCoreApplicationのaboutToQuitシグナルに接続して、アプリケーション終了時にシングルトンインスタンスを解放する
  // QApplicationを使用している場合はQApplication::instance()::aboutToQuit()
  if (QCoreApplication::instance()) {
   QObject::connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, []() {
	delete s_instance;
	});
  }
 }
 return *s_instance;
}

WidgetCounter::WidgetCounter()
{

}


WidgetCounter::~WidgetCounter()
{

}

WidgetCounter& WidgetCounter::getInstance()
{
 return WidgetCounter::Impl::getInstance();
}

}
