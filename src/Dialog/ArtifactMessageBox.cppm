module;

#include <QCheckBox>
#include <QMessageBox>
#include <QWidget>

module Widgets.Dialog.MessageBox;

namespace ArtifactWidgets {
namespace {

void applySpec(QMessageBox& box, const MessageBoxSpec& spec, bool* checked)
{
    box.setWindowTitle(spec.title);
    box.setText(spec.text);

    if (!spec.informativeText.isEmpty()) {
        box.setInformativeText(spec.informativeText);
    }
    if (!spec.detailedText.isEmpty()) {
        box.setDetailedText(spec.detailedText);
    }

    box.setIcon(spec.icon);
    box.setStandardButtons(spec.standardButtons);

    if (spec.defaultButton != QMessageBox::NoButton) {
        box.setDefaultButton(spec.defaultButton);
    }

    box.setTextFormat(spec.textFormat);

    if (!spec.checkBoxText.isEmpty()) {
        auto* checkBox = new QCheckBox(spec.checkBoxText, &box);
        box.setCheckBox(checkBox);
    }

    if (checked) {
        *checked = false;
    }
}

} // namespace

QMessageBox::StandardButton ArtifactMessageBox::show(QWidget* parent, const MessageBoxSpec& spec, bool* checked)
{
    QMessageBox box(parent);
    applySpec(box, spec, checked);

    const auto result = static_cast<QMessageBox::StandardButton>(box.exec());
    if (checked) {
        *checked = box.checkBox() && box.checkBox()->isChecked();
    }
    return result;
}

QMessageBox::StandardButton ArtifactMessageBox::question(
    QWidget* parent,
    const QString& title,
    const QString& text,
    QMessageBox::StandardButtons buttons,
    QMessageBox::StandardButton defaultButton,
    const QString& informativeText,
    const QString& detailedText,
    const QString& checkBoxText,
    bool* checked)
{
    MessageBoxSpec spec;
    spec.title = title;
    spec.text = text;
    spec.informativeText = informativeText;
    spec.detailedText = detailedText;
    spec.checkBoxText = checkBoxText;
    spec.icon = QMessageBox::Question;
    spec.standardButtons = buttons;
    spec.defaultButton = defaultButton;
    return show(parent, spec, checked);
}

bool ArtifactMessageBox::confirmDelete(QWidget* parent, const QString& title, const QString& text)
{
    QMessageBox box(parent);
    box.setWindowTitle(title);
    box.setText(text);
    box.setIcon(QMessageBox::Warning);

    auto* deleteButton = box.addButton(QStringLiteral("削除"), QMessageBox::DestructiveRole);
    auto* cancelButton = box.addButton(QStringLiteral("キャンセル"), QMessageBox::RejectRole);
    box.setDefaultButton(cancelButton);

    box.exec();
    return box.clickedButton() == deleteButton;
}

bool ArtifactMessageBox::confirmOverwrite(QWidget* parent, const QString& title, const QString& text)
{
    QMessageBox box(parent);
    box.setWindowTitle(title);
    box.setText(text);
    box.setIcon(QMessageBox::Warning);

    auto* overwriteButton = box.addButton(QStringLiteral("上書き"), QMessageBox::AcceptRole);
    auto* cancelButton = box.addButton(QStringLiteral("キャンセル"), QMessageBox::RejectRole);
    box.setDefaultButton(cancelButton);

    box.exec();
    return box.clickedButton() == overwriteButton;
}

bool ArtifactMessageBox::confirmAction(QWidget* parent, const QString& title, const QString& text)
{
    return question(parent, title, text, QMessageBox::Yes | QMessageBox::No, QMessageBox::No) ==
           QMessageBox::Yes;
}

} // namespace ArtifactWidgets
