module;
#include <windows.h>
#include <shobjidl.h>
#include <QWidget>
#include "../Define/DllExportMacro.hpp"

export module WinTaskbarProgress;

namespace ArtifactWidgets
{
    /**
     * @brief Windowsタスクバーの進捗表示を管理するクラス
     */
    class WIDGET_LIBRARY_DLL_API WinTaskbarProgress
    {
    public:
        /**
         * @brief 指定したウィンドウのタスクバー進捗を設定する
         * @param widget 対象のウィジェット
         * @param value 現在値
         * @param max 最大値
         */
        static void setProgress(QWidget* widget, int value, int max = 100);

        /**
         * @brief プログレスバーの状態を設定する
         * @param widget 対象のウィジェット
         * @param paused trueで一時停止（黄色）、falseで通常（緑）
         */
        static void setPaused(QWidget* widget, bool paused);

        /**
         * @brief エラー状態を設定する
         * @param widget 対象のウィジェット
         * @param error trueでエラー（赤色）
         */
        static void setError(QWidget* widget, bool error);

        /**
         * @brief 進捗表示をクリアする
         * @param widget 対象のウィジェット
         */
        static void clear(QWidget* widget);
    };
}
