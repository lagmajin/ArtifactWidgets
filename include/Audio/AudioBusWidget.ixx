module;
#include <wobjectcpp.h>
#include <QtCore/qobject.h>
export module AudioBusWidget;


namespace ArtifactWidgets {

class AudioBusWidget : public QObject {
    W_OBJECT(AudioBusWidget)

 private:

 public:
    AudioBusWidget(QObject* parent = nullptr);
    ~AudioBusWidget();
};

};