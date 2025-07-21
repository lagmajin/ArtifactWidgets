module;
#include <QHash>
#include "QCoreApplication"
import std;

module WidgetCounter;


namespace ArtifactWidgets {

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
