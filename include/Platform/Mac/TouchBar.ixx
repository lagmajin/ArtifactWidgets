module;
#include <QWidget>
#include <QString>
#include "../../Define/DllExportMacro.hpp"

export module Platform.Mac.TouchBar;

namespace ArtifactWidgets
{
    /**
     * @brief macOS Touch Bar 操作用クラス
     */
    class WIDGET_LIBRARY_DLL_API MacTouchBar
    {
    public:
        /**
         * @brief 指定したウィンドウにTouch Barをインストールする
         */
        static void install(QWidget* window);

        /**
         * @brief ボタンなどを追加する（デモ用）
         */
        static void addButton(const QString& label, int actionId);
    };
}
