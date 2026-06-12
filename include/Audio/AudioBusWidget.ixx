module;
#include <memory>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPalette>
#include <QtGui/QColor>
#include <QtGui/QPaintEvent>
#include <QtCore/QTimer>
#include <wobjectdefs.h>
#include "../Define/DllExportMacro.hpp"
export module AudioBusWidget;

import Audio.Mixer;
import Audio.Bus;

export namespace ArtifactWidgets {

class AudioBusWidget : public QWidget {
    W_OBJECT(AudioBusWidget)

public:
    explicit AudioBusWidget(ArtifactCore::AudioMixer* mixer, std::shared_ptr<ArtifactCore::AudioBus> bus, QWidget* parent = nullptr);
    ~AudioBusWidget();

    void refreshRoutingTargets();
    void updateBusDisplay();

signals:
    void busRenamed(const QString& oldName, const QString& newName) W_SIGNAL(busRenamed, oldName, newName);
    void routingChanged() W_SIGNAL(routingChanged);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    ArtifactCore::AudioMixer* mixer_;
    std::shared_ptr<ArtifactCore::AudioBus> bus_;
    QLabel* nameLabel_;
    QSlider* volumeSlider_;
    QSlider* panSlider_;
    QPushButton* muteButton_;
    QPushButton* soloButton_;
    QComboBox* routeCombo_;
};

} // namespace ArtifactWidgets
