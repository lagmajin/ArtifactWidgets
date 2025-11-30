module;
#include <QWidget>
#include <wobjectdefs.h>
#include "../Define/DllExportMacro.hpp"
export module Widgets.EditableLabel;

export namespace ArtifactWidgets
{

 class WIDGET_LIBRARY_DLL_API EditableLabel :public QWidget
 {
 	W_OBJECT(EditableLabel)
 private:
  class Impl;
  Impl* impl_;
 protected:
  bool eventFilter(QObject* obj, QEvent* event);
 public:
  explicit EditableLabel(QWidget* parent = nullptr);
  ~EditableLabel();
  QString text() const;
  void setText(const QString& text);
  QSize sizeHint() const;
  void startEdit();
  void finishEdit();
 };

};