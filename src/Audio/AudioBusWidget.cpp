module;
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QComboBox>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QFont>
#include <QPalette>
#include <QColor>
#include <wobjectimpl.h>

module AudioBusWidget;

import Audio.Mixer;
import Audio.Bus;

namespace ArtifactWidgets {

W_OBJECT_IMPL(AudioBusWidget)

AudioBusWidget::AudioBusWidget(ArtifactCore::AudioMixer* mixer, std::shared_ptr<ArtifactCore::AudioBus> bus, QWidget* parent)
    : QWidget(parent), mixer_(mixer), bus_(bus) {

    setFixedWidth(140);
    setMinimumHeight(200);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(4);

    QString busName = QString::fromStdString(bus->getName());
    nameLabel_ = new QLabel(busName, this);
    nameLabel_->setAlignment(Qt::AlignCenter);
    QFont nameFont = nameLabel_->font();
    nameFont.setBold(true);
    nameFont.setPointSize(9);
    nameLabel_->setFont(nameFont);
    layout->addWidget(nameLabel_);

    volumeSlider_ = new QSlider(Qt::Vertical, this);
    volumeSlider_->setRange(-600, 120);
    volumeSlider_->setValue(static_cast<int>(bus->getVolume() * 10));
    connect(volumeSlider_, &QSlider::valueChanged, this, [this](int val) {
        bus_->setVolume(val / 10.0f);
    });
    layout->addWidget(volumeSlider_, 0, Qt::AlignHCenter);

    panSlider_ = new QSlider(Qt::Horizontal, this);
    panSlider_->setRange(-100, 100);
    panSlider_->setValue(static_cast<int>(bus->getPan() * 100));
    connect(panSlider_, &QSlider::valueChanged, this, [this](int val) {
        bus_->setPan(val / 100.0f);
    });
    layout->addWidget(panSlider_);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    muteButton_ = new QPushButton("M", this);
    muteButton_->setCheckable(true);
    muteButton_->setFixedSize(24, 24);
    connect(muteButton_, &QPushButton::toggled, this, [this](bool checked) {
        bus_->setMute(checked);
    });
    soloButton_ = new QPushButton("S", this);
    soloButton_->setCheckable(true);
    soloButton_->setFixedSize(24, 24);
    connect(soloButton_, &QPushButton::toggled, this, [this](bool checked) {
        bus_->setSolo(checked);
    });
    btnLayout->addWidget(muteButton_);
    btnLayout->addWidget(soloButton_);
    layout->addLayout(btnLayout);

    routeCombo_ = new QComboBox(this);
    refreshRoutingTargets();
    connect(routeCombo_, &QComboBox::currentIndexChanged, this, [this](int idx) {
        if (!mixer_ || !bus_) return;
        auto master = mixer_->getMasterBus();
        if (idx == 0) {
            mixer_->connect(bus_, master);
        } else {
            auto buses = mixer_->getAllBuses();
            int busIdx = idx - 1;
            if (busIdx >= 0 && busIdx < static_cast<int>(buses.size())) {
                mixer_->connect(bus_, buses[busIdx]);
            }
        }
        emit routingChanged();
    });
    layout->addWidget(routeCombo_);

    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(32, 34, 38));
    setPalette(palette);
}

AudioBusWidget::~AudioBusWidget() = default;

void AudioBusWidget::refreshRoutingTargets() {
    if (!routeCombo_) return;
    routeCombo_->blockSignals(true);
    routeCombo_->clear();
    routeCombo_->addItem(QStringLiteral("Master"));
    if (mixer_) {
        auto buses = mixer_->getAllBuses();
        for (const auto& b : buses) {
            if (b && b != bus_ && b != mixer_->getMasterBus()) {
                routeCombo_->addItem(QString::fromStdString(b->getName()));
            }
        }
        auto target = mixer_->getRoutingTarget(bus_);
        int idx = 0;
        for (int i = 0; i < routeCombo_->count(); ++i) {
            if (target && routeCombo_->itemText(i) == QString::fromStdString(target->getName())) {
                idx = i;
                break;
            }
        }
        routeCombo_->setCurrentIndex(idx);
    }
    routeCombo_->blockSignals(false);
}

void AudioBusWidget::updateBusDisplay() {
    if (nameLabel_ && bus_) {
        nameLabel_->setText(QString::fromStdString(bus_->getName()));
    }
}

void AudioBusWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && bus_) {
        QMenu menu(this);
        menu.addAction("Rename...", [this]() {
            // TODO: Implement rename dialog
        });
        if (bus_ != mixer_->getMasterBus()) {
            menu.addAction("Remove Bus", [this]() {
                if (mixer_ && bus_) {
                    mixer_->removeBus(bus_);
                    emit routingChanged();
                }
            });
        }
        menu.exec(mapToGlobal(event->pos()));
    }
    QWidget::mousePressEvent(event);
}

void AudioBusWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), palette().color(QPalette::Window));
}

} // namespace ArtifactWidgets
