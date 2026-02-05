module;
#include <QPlainTextEdit>
#include <QDateTime>
#include <QTextCharFormat>
#include <QMenu>
#include <QColor>
#include <QFont>
#include <wobjectimpl.h>

module ConsoleWidget;

namespace ArtifactWidgets {

    W_OBJECT_IMPL(ConsoleWidget)

    class ConsoleWidget::Impl {
    public:
        Impl() {}
        ~Impl() {}
        
        void append(QPlainTextEdit* editor, const QString& type, const QString& message, const QColor& color) {
            QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");
            
            editor->moveCursor(QTextCursor::End);
            
            QTextCharFormat format;
            format.setForeground(Qt::gray);
            editor->setCurrentCharFormat(format);
            editor->insertPlainText(QString("[%1] ").arg(timestamp));
            
            format.setForeground(color);
            format.setFontWeight(QFont::Bold);
            editor->setCurrentCharFormat(format);
            editor->insertPlainText(QString("[%1] ").arg(type));
            
            format.setFontWeight(QFont::Normal);
            editor->setCurrentCharFormat(format);
            editor->insertPlainText(message + "\n");
            
            // Reset format
            editor->setCurrentCharFormat(QTextCharFormat());
            
            editor->ensureCursorVisible();
        }
    };

    ConsoleWidget::ConsoleWidget(QWidget* parent)
        : QPlainTextEdit(parent), impl_(new Impl())
    {
        setReadOnly(true);
        setWordWrapMode(QTextOption::NoWrap);
        
        // Dark theme console style
        setStyleSheet("color: #CCCCCC; background-color: #1E1E1E; font-family: 'Consolas', 'Monaco', monospace; font-size: 10pt;");
        
        setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, &QWidget::customContextMenuRequested, [this](const QPoint& pos) {
            QMenu* menu = createStandardContextMenu();
            menu->addSeparator();
            QAction* clearAction = menu->addAction("Clear Console");
            connect(clearAction, &QAction::triggered, this, &ConsoleWidget::clearConsole);
            menu->exec(mapToGlobal(pos));
            delete menu;
        });
    }

    ConsoleWidget::~ConsoleWidget()
    {
        delete impl_;
    }

    void ConsoleWidget::appendInfo(const QString& message)
    {
        impl_->append(this, "INFO", message, QColor("#4CAF50")); // Green
    }

    void ConsoleWidget::appendWarning(const QString& message)
    {
        impl_->append(this, "WARNING", message, QColor("#FF9800")); // Orange
    }

    void ConsoleWidget::appendError(const QString& message)
    {
        impl_->append(this, "ERROR", message, QColor("#F44336")); // Red
    }

    void ConsoleWidget::clearConsole()
    {
        clear();
    }

    void ConsoleWidget::setMaxLines(int maxLines)
    {
        setMaximumBlockCount(maxLines);
    }
}