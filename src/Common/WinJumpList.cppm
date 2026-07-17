module;
#include <windows.h>
#include <shobjidl.h>
#include <propkey.h>
#include <propvarutil.h>
#include <wrl/client.h>
#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <vector>

module WinJumpList;

using Microsoft::WRL::ComPtr;

namespace ArtifactWidgets
{
    struct JumpTask {
        QString title;
        QString description;
        QString path;
        QString arguments;
    };

    class WinJumpListPrivate {
    public:
        std::vector<JumpTask> tasks;

        bool createJumpList() {
            ComPtr<ICustomDestinationList> pDestList;
            HRESULT hr = CoCreateInstance(CLSID_DestinationList, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDestList));
            if (FAILED(hr)) return false;

            UINT maxSlots;
            ComPtr<IObjectArray> pRemovedItems;
            hr = pDestList->BeginList(&maxSlots, IID_PPV_ARGS(&pRemovedItems));
            if (FAILED(hr)) return false;

            ComPtr<IObjectCollection> pTasksCollection;
            hr = CoCreateInstance(CLSID_EnumerableObjectCollection, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pTasksCollection));
            if (FAILED(hr)) return false;

            for (const auto& task : tasks) {
                ComPtr<IShellLinkW> pLink;
                hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pLink));
                if (FAILED(hr)) continue;

                pLink->SetPath(reinterpret_cast<LPCWSTR>(task.path.utf16()));
                pLink->SetArguments(reinterpret_cast<LPCWSTR>(task.arguments.utf16()));
                pLink->SetDescription(reinterpret_cast<LPCWSTR>(task.description.utf16()));

                ComPtr<IPropertyStore> pPropStore;
                hr = pLink.As(&pPropStore);
                if (SUCCEEDED(hr)) {
                    PROPVARIANT var;
                    hr = InitPropVariantFromString(reinterpret_cast<LPCWSTR>(task.title.utf16()), &var);
                    if (SUCCEEDED(hr)) {
                        pPropStore->SetValue(PKEY_Title, var);
                        pPropStore->Commit();
                        PropVariantClear(&var);
                    }
                }
                pTasksCollection->AddObject(pLink.Get());
            }

            ComPtr<IObjectArray> pTasksArray;
            hr = pTasksCollection.As(&pTasksArray);
            if (SUCCEEDED(hr)) {
                pDestList->AddUserTasks(pTasksArray.Get());
            }

            pDestList->CommitList();
            return true;
        }
    };

    WinJumpList::WinJumpList() : d(new WinJumpListPrivate()) {}
    WinJumpList::~WinJumpList() { delete d; }

    bool WinJumpList::addNewWindowTask(const QString& title, const QString& description) {
        JumpTask task;
        task.title = title;
        task.description = description;
        task.path = QCoreApplication::applicationFilePath();
        task.arguments = "--new-window"; // アプリ側でこの引数をハンドルする必要がある
        d->tasks.push_back(task);
        return true;
    }

    bool WinJumpList::apply() {
        return d->createJumpList();
    }
}
