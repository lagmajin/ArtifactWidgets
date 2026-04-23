module;
#include <QFileInfo>
#include <QProcess>
#include <QString>
#include "../Define/DllExportMacro.hpp"

export module Platform.QuickLook;

import File.Preview;

export namespace ArtifactWidgets {

#if defined(Q_OS_MACOS)
 WIDGET_LIBRARY_DLL_API bool showQuickLookPreview(const ArtifactCore::PreviewDescriptor& preview);
#else
 inline bool showQuickLookPreview(const ArtifactCore::PreviewDescriptor&) {
  return false;
 }
#endif

}
