module;

#include <QString>
#include <QPlainTextEdit>
#include <wobjectdefs.h>

export module ConsoleWidget;



export namespace ArtifactWidgets {

 class ConsoleWidgetPrivate;

 class ConsoleWidget : public QPlainTextEdit {
        W_OBJECT(ConsoleWidget)
 private:
        class Impl;
        Impl* impl_;
    public:
        explicit ConsoleWidget(QWidget* parent = nullptr);
        virtual ~ConsoleWidget();

        void appendInfo(const QString& message);
        void appendWarning(const QString& message);
        void appendError(const QString& message);
        
        void clearConsole();
        void setMaxLines(int maxLines);
    };



}