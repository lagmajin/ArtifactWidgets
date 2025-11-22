module;
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>
module Code.SyntaxHighlighter;

import std;


namespace ArtifactCore {

 class SyntaxHighlighter::Impl {

 public:
  Impl();
  ~Impl();
  QTextCharFormat keywordFormat;     // 青
  QTextCharFormat numberFormat;      // 紫
  QTextCharFormat stringFormat;      // 黄色
  QTextCharFormat commentFormat;     // 緑
  QTextCharFormat builtinFormat;

  QRegularExpression keywordRegex;
  QRegularExpression numberRegex;
  QRegularExpression stringRegex;
  QRegularExpression commentLineRegex;
  QRegularExpression builtinRegex;
 };

 SyntaxHighlighter::Impl::Impl()
 {
  keywordFormat.setForeground(QColor("#3DA5FF")); // AE 青
  keywordFormat.setFontWeight(QFont::Bold);

  numberFormat.setForeground(QColor("#C678DD")); // 紫

  stringFormat.setForeground(QColor("#E5C07B")); // 黄

  commentFormat.setForeground(QColor("#98C379")); // 緑

  builtinFormat.setForeground(QColor("#56B6C2"));


  keywordRegex = QRegularExpression(
   R"(\b(var|let|const|if|else|while|for|return|function)\b)");
 }

 SyntaxHighlighter::Impl::~Impl()
 {

 }

 SyntaxHighlighter::SyntaxHighlighter(QObject* parent /*= nullptr*/) : QSyntaxHighlighter(parent), impl_(new Impl)
 {


 }

 SyntaxHighlighter::~SyntaxHighlighter()
 {
  delete impl_;
 }

 void SyntaxHighlighter::highlightBlock(const QString& text)
 {

 }

};