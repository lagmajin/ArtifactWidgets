module;
#include <QWidget>
#include <QMenu>
#include "../Define/DllExportMacro.hpp"

export module AppProgress;

namespace ArtifactWidgets
{
    /**
     * @brief OSのシェル（Taskbar/Dock）に対する進捗表示やメニューを管理するクラス
     */
    class WIDGET_LIBRARY_DLL_API AppProgress
    {
    public:
        /**
         * @brief 進捗を設定する (Windows: Taskbar, Mac: Dock)
         */
        static void setProgress(QWidget* widget, int value, int max = 100);

        /**
         * @brief 表示をクリアする
         */
        static void clear(QWidget* widget);

        /**
         * @brief [macOS専用] Dockメニューを設定する
         * Windowsでは無視されます。
         */
        static void setDockMenu(QMenu* menu);
    };
}
