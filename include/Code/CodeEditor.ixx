module;
#include <QWidget>
#include <QPlainTextEdit>
#include <wobjectdefs.h>
#include "../Define/DllExportMacro.hpp"

export module CodeEditor;

export namespace ArtifactWidgets {

 class CodeEditor : public QPlainTextEdit
 {
  W_OBJECT(CodeEditor)
 public:
  class LineNumberArea;
 private:
  class Impl;
  Impl* impl_;
  QWidget* lineNumberArea;
  
 public:
  explicit CodeEditor(QWidget* parent = nullptr);
  ~CodeEditor();
  void setLineNumbersVisible(bool visible);
  bool isLineNumbersVisible() const;
  
  int lineNumberAreaWidth() const;
  void lineNumberAreaPaintEvent(QPaintEvent* event);

 signals:
  void textChanged(const QString& text) W_SIGNAL(textChanged, text);
 };

}
