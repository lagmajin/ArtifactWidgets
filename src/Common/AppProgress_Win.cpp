module;
#include <windows.h>
#include <shobjidl.h>
#include <wrl/client.h>
#include <QWidget>
#include <QMenu>

module AppProgress;

using Microsoft::WRL::ComPtr;

namespace ArtifactWidgets
{
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

    void AppProgress::setProgress(QWidget* widget, int value, int max)
    {
        if (!widget) return;
        HWND hwnd = (HWND)widget->winId();
        auto pTaskbarList = getTaskbarList();
        if (pTaskbarList) {
            pTaskbarList->SetProgressValue(hwnd, value, max);
            pTaskbarList->SetProgressState(hwnd, TBPF_NORMAL);
        }
    }

    void AppProgress::clear(QWidget* widget)
    {
        if (!widget) return;
        HWND hwnd = (HWND)widget->winId();
        auto pTaskbarList = getTaskbarList();
        if (pTaskbarList) {
            pTaskbarList->SetProgressState(hwnd, TBPF_NOPROGRESS);
        }
    }

    void AppProgress::setDockMenu(QMenu* menu)
    {
        // Windows: No operation
    }
}
