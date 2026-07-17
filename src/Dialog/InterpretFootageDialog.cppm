module;
#include <wobjectimpl.h>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

module Widgets.Dialog.InterpretFootage;

import Media.SourceInterpret;

namespace ArtifactWidgets {
W_OBJECT_IMPL(InterpretFootageDialog)

struct InterpretFootageDialog::Impl {
    QDoubleSpinBox* frameRateSpin = nullptr;
    QRadioButton* keepKeyframesBtn = nullptr;
    QRadioButton* keepTimeBtn = nullptr;
    QRadioButton* reSampleBtn = nullptr;
    QLabel* impactLabel = nullptr;
    QComboBox* inputColorSpace = nullptr;
    QComboBox* inputTransferFunction = nullptr;

    int selectedMode = 0;
};

InterpretFootageDialog::InterpretFootageDialog(const QString& footageName,
                                               double currentFrameRate,
                                               double sourceFrameRate,
                                               int affectedLayerCount,
                                               int affectedKeyframeCount,
                                               bool hasTimeRemap,
                                               QWidget* parent)
    : QDialog(parent), impl_(std::make_unique<Impl>()) {
    setWindowTitle(tr("Interpret Footage: %1").arg(footageName));
    setMinimumWidth(400);

    auto* mainLayout = new QVBoxLayout(this);

    auto* infoLabel = new QLabel(
        tr("Source frame rate: %1 fps").arg(sourceFrameRate, 0, 'f', 3));
    mainLayout->addWidget(infoLabel);

    auto* formLayout = new QFormLayout();
    impl_->frameRateSpin = new QDoubleSpinBox();
    impl_->frameRateSpin->setRange(1.0, 240.0);
    impl_->frameRateSpin->setDecimals(3);
    impl_->frameRateSpin->setValue(currentFrameRate > 0.0 ? currentFrameRate : sourceFrameRate);
    formLayout->addRow(tr("Frame rate (fps):"), impl_->frameRateSpin);
    impl_->inputColorSpace = new QComboBox();
    impl_->inputColorSpace->addItems({tr("Auto (source metadata)"), tr("Linear"), tr("sRGB"),
                                      tr("Rec.709"), tr("Rec.2020"), tr("ACEScc"), tr("ACEScct")});
    formLayout->addRow(tr("Input color space:"), impl_->inputColorSpace);
    impl_->inputTransferFunction = new QComboBox();
    impl_->inputTransferFunction->addItems({tr("Auto (source metadata)"), tr("Linear"), tr("sRGB"),
                                            tr("Rec.709"), tr("PQ"), tr("HLG"), tr("ACEScc"),
                                            tr("ACEScct"), tr("Sony S-Log3"), tr("Cineon")});
    formLayout->addRow(tr("Input transfer:"), impl_->inputTransferFunction);
    mainLayout->addLayout(formLayout);

    auto* modeGroup = new QGroupBox(tr("Preserve mode"));
    auto* modeLayout = new QVBoxLayout(modeGroup);

    impl_->keepKeyframesBtn = new QRadioButton(tr("Keep keyframes — scale keyframe times to match new frame rate"));
    impl_->keepTimeBtn = new QRadioButton(tr("Keep time — adjust layer in/out points to preserve timing"));
    impl_->reSampleBtn = new QRadioButton(tr("Re-sample — re-interpret source timing from scratch"));

    modeLayout->addWidget(impl_->keepKeyframesBtn);
    modeLayout->addWidget(impl_->keepTimeBtn);
    modeLayout->addWidget(impl_->reSampleBtn);
    impl_->keepKeyframesBtn->setChecked(true);
    mainLayout->addWidget(modeGroup);

    QString impactMsg;
    if (affectedLayerCount > 0 || hasTimeRemap) {
        impactMsg += tr("<b>Impact:</b><br>");
        if (affectedLayerCount > 0) {
            impactMsg += tr("- %1 layer(s) reference this footage<br>").arg(affectedLayerCount);
        }
        if (affectedKeyframeCount > 0) {
            impactMsg += tr("- %1 keyframe(s) may shift<br>").arg(affectedKeyframeCount);
        }
        if (hasTimeRemap) {
            impactMsg += tr("<b>- Time remap is active — review affected layers</b><br>");
        }
    } else {
        impactMsg = tr("<i>No layers reference this footage.</i>");
    }
    impl_->impactLabel = new QLabel(impactMsg);
    impl_->impactLabel->setWordWrap(true);
    mainLayout->addWidget(impl_->impactLabel);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
}

InterpretFootageDialog::~InterpretFootageDialog() = default;

double InterpretFootageDialog::selectedFrameRate() const {
    return impl_->frameRateSpin->value();
}

int InterpretFootageDialog::selectedPreserveMode() const {
    if (impl_->keepKeyframesBtn->isChecked()) return 0;
    if (impl_->keepTimeBtn->isChecked()) return 1;
    return 2;
}

QString InterpretFootageDialog::selectedInputColorSpace() const {
    const int index = impl_->inputColorSpace->currentIndex();
    return index == 0 ? QString() : impl_->inputColorSpace->currentText();
}

QString InterpretFootageDialog::selectedInputTransferFunction() const {
    const int index = impl_->inputTransferFunction->currentIndex();
    return index == 0 ? QString() : impl_->inputTransferFunction->currentText();
}

} // namespace ArtifactWidgets
