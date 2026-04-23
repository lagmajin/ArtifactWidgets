module;
#include <QFileInfo>
#include <QProcess>
#include <QStringList>
#include <QString>

module Platform.QuickLook;

import File.Preview;

namespace ArtifactWidgets {

#if defined(Q_OS_MACOS)
bool showQuickLookPreview(const ArtifactCore::PreviewDescriptor& preview)
{
 if (!preview.canPreviewWithSystem) {
  return false;
 }

 const QFileInfo info(preview.filePath);
 if (!info.exists() || info.isDirectory()) {
  return false;
 }

 const QString filePath = info.absoluteFilePath();
 const QStringList arguments{QStringLiteral("-p"), filePath};
 return QProcess::startDetached(QStringLiteral("/usr/bin/qlmanage"), arguments);
}
#endif

}
