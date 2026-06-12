module;

#include <wobjectdefs.h>
#include <QDialog>
#include <QVector>
#include <QString>
#include <memory>

export module Widgets.Dialog.InterpretFootage;

export namespace ArtifactWidgets {

class InterpretFootageDialog : public QDialog {
    W_OBJECT(InterpretFootageDialog)
public:
    explicit InterpretFootageDialog(const QString& footageName,
                                    double currentFrameRate,
                                    double sourceFrameRate,
                                    int affectedLayerCount,
                                    int affectedKeyframeCount,
                                    bool hasTimeRemap,
                                    QWidget* parent = nullptr);
    ~InterpretFootageDialog();

    double selectedFrameRate() const;
    int selectedPreserveMode() const; // 0=KeepKeyframes, 1=KeepTime, 2=ReSample

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace ArtifactWidgets
