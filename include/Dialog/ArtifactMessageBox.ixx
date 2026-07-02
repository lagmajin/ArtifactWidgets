module;

#include <Define/DllExportMacro.hpp>
#include <QMessageBox>
#include <QString>
#include <QWidget>
#include <QtCore/qnamespace.h>

export module Widgets.Dialog.MessageBox;

export namespace ArtifactWidgets {

struct MessageBoxSpec {
    QString title;
    QString text;
    QString informativeText;
    QString detailedText;
    QString checkBoxText;
    QMessageBox::Icon icon = QMessageBox::NoIcon;
    QMessageBox::StandardButtons standardButtons = QMessageBox::Ok;
    QMessageBox::StandardButton defaultButton = QMessageBox::NoButton;
    Qt::TextFormat textFormat = Qt::AutoText;
};

class WIDGET_LIBRARY_DLL_API ArtifactMessageBox {
public:
    static QMessageBox::StandardButton show(QWidget* parent, const MessageBoxSpec& spec, bool* checked = nullptr);

    static QMessageBox::StandardButton question(
        QWidget* parent,
        const QString& title,
        const QString& text,
        QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No,
        QMessageBox::StandardButton defaultButton = QMessageBox::No,
        const QString& informativeText = {},
        const QString& detailedText = {},
        const QString& checkBoxText = {},
        bool* checked = nullptr);

    static bool confirmDelete(QWidget* parent, const QString& title, const QString& text);
    static bool confirmOverwrite(QWidget* parent, const QString& title, const QString& text);
    static bool confirmAction(QWidget* parent, const QString& title, const QString& text);
};

} // namespace ArtifactWidgets
