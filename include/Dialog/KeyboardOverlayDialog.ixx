module ;
#include <wobjectdefs.h>
#include <QDialog>

#include "../Define/DllExportMacro.hpp"
export module KeyboardOverlayDialog;

import Widget.AbstractDialog;

export namespace ArtifactWidgets
{
 struct ShortcutEntry {
  QString action;     // "Move Layer Up"
  QString key;        // "Ctrl+Shift+↑"
  QString category;   // "Layer", "Timeline"
 };

 class WIDGET_LIBRARY_DLL_API KeyboardOverlayDialog:public QDialog
 {

 private:
  class Impl;
  Impl* impl_;
 public:
  explicit KeyboardOverlayDialog(QWidget* parent = nullptr);
  ~KeyboardOverlayDialog();
  //void test();
  void setCompactMode(bool enabled);      // AE風の密度高いテーブル形式
  void setOverlayOpacity(float opacity);  // 背景半透明
  void setAlwaysOnTop(bool enabled);
  void showCentered();

 public slots:

 };


}
