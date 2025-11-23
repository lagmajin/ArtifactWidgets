
module;
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QDesktopServices>
#include <QProcess>
#include <QString>
#include "../Define/DllExportMacro.hpp"
export module RevalFileManager;

export namespace ArtifactWidgets {

 WIDGET_LIBRARY_DLL_API bool  revealInFileManager(const QString& rawPath)
 {
  QFileInfo info(rawPath);
  if (!info.exists()) {
   qWarning() << "Path does not exist:" << rawPath;
   return false;
  }

  const QString path = info.absoluteFilePath();

#if defined(Q_OS_WIN)

  // フォルダはそのまま、ファイルは /select, でハイライト
  if (info.isDir()) {
   return QProcess::startDetached(
	"explorer.exe",
	{ QDir::toNativeSeparators(path) }
   );
  }
  else {
   return QProcess::startDetached(
	"explorer.exe",
	{ "/select,", QDir::toNativeSeparators(path) }
   );
  }

#elif defined(Q_OS_MACOS)

  if (info.isDir()) {
   // Finder でフォルダを直接開く
   QProcess::startDetached(
	"/usr/bin/osascript",
	{ "-e", QString("tell application \"Finder\" to open POSIX file \"%1\"").arg(path) }
   );
  }
  else {
   // ファイルを reveal
   QProcess::startDetached(
	"/usr/bin/osascript",
	{ "-e", QString("tell application \"Finder\" to reveal POSIX file \"%1\"").arg(path) }
   );
  }

  // Finder を前面に
  QProcess::startDetached(
   "/usr/bin/osascript",
   { "-e", "tell application \"Finder\" to activate" }
  );
  return true;

#else
  // Linux：フォルダは open、ファイルは親フォルダを開く
  QString dir = info.isDir() ? path : info.absolutePath();

  // 1st: xdg-open
  if (QProcess::startDetached("xdg-open", { dir }))
   return true;

  // 2nd: gio
  if (QProcess::startDetached("gio", { "open", dir }))
   return true;

  // 3rd: KDE dolphin
  if (QProcess::startDetached("dolphin", { dir }))
   return true;

  // 4th: Nautilus
  if (QProcess::startDetached("nautilus", { dir }))
   return true;

  return false;
#endif
 }


};