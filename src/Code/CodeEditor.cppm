module;
#include <QPainter>
#include <QTextBlock>
#include <QScrollBar>
#include <QPlainTextEdit>
#include <QWidget>
#include <wobjectimpl.h>
module CodeEditor;

import Code.SyntaxHighlighter;

namespace ArtifactWidgets {

 W_OBJECT_IMPL(CodeEditor)

 class CodeEditor::LineNumberArea : public QWidget
 {
 public:
  LineNumberArea(CodeEditor* editor) : QWidget(editor), codeEditor(editor) {}

  QSize sizeHint() const override
  {
   return QSize(codeEditor->lineNumberAreaWidth(), 0);
  }

 protected:
  void paintEvent(QPaintEvent* event) override
  {
   codeEditor->lineNumberAreaPaintEvent(event);
  }

 private:
  CodeEditor* codeEditor;
 };

 class CodeEditor::Impl
 {
 public:
  Impl(CodeEditor* editor);
  ~Impl();
  void lineNumberAreaPaintEvent(QPaintEvent* event);
  int lineNumberAreaWidth() const;

  CodeEditor* editor;
  bool showLineNumbers = true;
  ArtifactCore::SyntaxHighlighter* highlighter = nullptr;
 };

 CodeEditor::Impl::Impl(CodeEditor* editor)
  : editor(editor)
 {
  highlighter = new ArtifactCore::SyntaxHighlighter(editor->document());
 }

 CodeEditor::Impl::~Impl()
 {
  delete highlighter;
 }

 void CodeEditor::Impl::lineNumberAreaPaintEvent(QPaintEvent* event)
 {
  QPainter painter(editor->lineNumberArea);
  painter.fillRect(event->rect(), QColor("#1E1E1E"));

  QTextBlock block = editor->firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = static_cast<int>(editor->blockBoundingGeometry(block).translated(editor->contentOffset()).top());
  int bottom = top + static_cast<int>(editor->blockBoundingRect(block).height());

  while (block.isValid() && top <= event->rect().bottom()) {
   if (block.isVisible() && bottom >= event->rect().top()) {
    QString number = QString::number(blockNumber + 1);
    painter.setPen(QColor("#858585"));
    painter.drawText(0, top, editor->lineNumberArea->width() - 5,
                     editor->fontMetrics().height(),
                     Qt::AlignRight, number);
   }

   block = block.next();
   top = bottom;
   bottom = top + static_cast<int>(editor->blockBoundingRect(block).height());
   ++blockNumber;
  }
 }

 int CodeEditor::Impl::lineNumberAreaWidth() const
 {
  if (!showLineNumbers) return 0;

  int digits = 1;
  int max = qMax(1, editor->blockCount());
  while (max >= 10) {
   max /= 10;
   ++digits;
  }

  int space = 10 + editor->fontMetrics().horizontalAdvance(QChar('0')) * digits;
  return space;
 }

 CodeEditor::CodeEditor(QWidget* parent)
  : QPlainTextEdit(parent)
  , lineNumberArea(new CodeEditor::LineNumberArea(this))
  , impl_(new Impl(this))
 {
  setViewportMargins(impl_->lineNumberAreaWidth(), 0, 0, 0);

  connect(this, &QPlainTextEdit::blockCountChanged,
          this, [this](int newBlockCount) {
              setViewportMargins(impl_->lineNumberAreaWidth(), 0, 0, 0);
          });
  connect(this, &QPlainTextEdit::updateRequest,
          this, [this](const QRect& rect, int dy) {
              if (dy) {
                  lineNumberArea->scroll(0, dy);
              }
              else {
                  lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
              }

              if (rect.contains(viewport()->rect().bottomRight())) {
                  setViewportMargins(impl_->lineNumberAreaWidth(), 0, 0, 0);
              }
          });
  connect(this, &QPlainTextEdit::textChanged,
          this, [this]() { emit textChanged(this->toPlainText()); });

  setStyleSheet(R"(
   QPlainTextEdit {
    background-color: #1E1E1E;
    color: #D4D4D4;
    font-family: 'Consolas', 'Monaco', monospace;
    font-size: 12px;
    border: none;
   }
  )");
 }

 CodeEditor::~CodeEditor()
 {
  delete impl_;
 }

 void CodeEditor::setLineNumbersVisible(bool visible)
 {
  impl_->showLineNumbers = visible;
  setViewportMargins(visible ? impl_->lineNumberAreaWidth() : 0, 0, 0, 0);
  lineNumberArea->setVisible(visible);
 }

 bool CodeEditor::isLineNumbersVisible() const
 {
  return impl_->showLineNumbers;
 }

 int CodeEditor::lineNumberAreaWidth() const
 {
  return impl_->lineNumberAreaWidth();
 }

 void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent* event)
 {
  impl_->lineNumberAreaPaintEvent(event);
 }

}
