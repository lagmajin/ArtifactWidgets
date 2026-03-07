module;
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>
module Code.SyntaxHighlighter;

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <cmath>
#include <functional>
#include <optional>
#include <utility>
#include <array>
#include <mutex>
#include <thread>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <any>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <deque>
#include <list>
#include <tuple>
#include <numeric>
#include <regex>
#include <random>





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