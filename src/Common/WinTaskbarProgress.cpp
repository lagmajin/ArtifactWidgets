module;
#include <windows.h>
#include <shobjidl.h>
#include <wrl/client.h>
#include <QWidget>

module WinTaskbarProgress;

using Microsoft::WRL::ComPtr;

namespace ArtifactWidgets
{
    // ヘルパー: ITaskbarList3のインスタンスを取得
    static ComPtr<ITaskbarList3> getTaskbarList()
    {
        static ComPtr<ITaskbarList3> pTaskbarList;
        if (!pTaskbarList) {
            CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pTaskbarList));
            if (pTaskbarList) {
                pTaskbarList->HrInit();
            }
        }
        return pTaskbarList;
    }

    void WinTaskbarProgress::setProgress(QWidget* widget, int value, int max)
    {
        if (!widget) return;
        HWND hwnd = (HWND)widget->winId();
        auto pTaskbarList = getTaskbarList();
        if (pTaskbarList) {
            pTaskbarList->SetProgressValue(hwnd, value, max);
            pTaskbarList->SetProgressState(hwnd, TBPF_NORMAL);
        }
    }

    void WinTaskbarProgress::setPaused(QWidget* widget, bool paused)
    {
        if (!widget) return;
        HWND hwnd = (HWND)widget->winId();
        auto pTaskbarList = getTaskbarList();
        if (pTaskbarList) {
            pTaskbarList->SetProgressState(hwnd, paused ? TBPF_PAUSED : TBPF_NORMAL);
        }
    }

    void WinTaskbarProgress::setError(QWidget* widget, bool error)
    {
        if (!widget) return;
        HWND hwnd = (HWND)widget->winId();
        auto pTaskbarList = getTaskbarList();
        if (pTaskbarList) {
            pTaskbarList->SetProgressState(hwnd, error ? TBPF_ERROR : TBPF_NORMAL);
        }
    }

    void WinTaskbarProgress::clear(QWidget* widget)
    {
        if (!widget) return;
        HWND hwnd = (HWND)widget->winId();
        auto pTaskbarList = getTaskbarList();
        if (pTaskbarList) {
            pTaskbarList->SetProgressState(hwnd, TBPF_NOPROGRESS);
        }
    }
}
