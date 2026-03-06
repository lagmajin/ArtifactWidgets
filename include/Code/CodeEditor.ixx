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
 private:
  class Impl;
  Impl* impl_;
  QWidget* lineNumberArea;
 private slots:
  void updateLineNumberAreaWidth(int newBlockCount);
  void updateLineNumberArea(const QRect& rect, int dy);
 public:
  explicit CodeEditor(QWidget* parent = nullptr);
  ~CodeEditor();
  void setLineNumbersVisible(bool visible);
  bool isLineNumbersVisible() const;
 signals:
  void textChanged(const QString& text) W_SIGNAL(textChanged, text);
 };

}
