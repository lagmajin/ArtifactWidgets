module;
#include <windows.h>
#include <shobjidl.h>
#include <propkey.h>
#include <propvarutil.h>
#include <QString>
#include <QCoreApplication>
#include "../Define/DllExportMacro.hpp"

export module WinJumpList;

export namespace ArtifactWidgets
{
    class WinJumpListPrivate;

    /**
     * @brief Windowsのジャンプリストを管理するクラス
     */
    class WIDGET_LIBRARY_DLL_API WinJumpList
    {
    public:
        WinJumpList();
        ~WinJumpList();

        /**
         * @brief 「新しいウィンドウ」タスクを追加する
         * @param title 表示名（例：「新しいウィンドウ」）
         * @param description 説明文
         */
        bool addNewWindowTask(const QString& title, const QString& description = "");

        /**
         * @brief 設定を適用してジャンプリストを更新する
         */
        bool apply();

    private:
        WinJumpListPrivate* d;
    };
}