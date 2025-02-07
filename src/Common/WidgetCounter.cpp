
module;

import std;

module WidgetCounter;


namespace ArtifactWidgets {

class WidgetCounterPrivate {
private:
 std::mutex mtx_;
public:
 WidgetCounterPrivate();
 ~WidgetCounterPrivate();
 void increment();
 void decrement();
};

WidgetCounterPrivate::WidgetCounterPrivate()
{

}

WidgetCounterPrivate::~WidgetCounterPrivate()
{

}

void WidgetCounterPrivate::increment()
{

}

void WidgetCounterPrivate::decrement()
{

}

WidgetCounter::WidgetCounter()
{

}


WidgetCounter::~WidgetCounter()
{

}

}
