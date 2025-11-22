module;
#include <QSyntaxHighlighter>
export module Code.SyntaxHighlighter;

export namespace ArtifactCore {



 class SyntaxHighlighter:public QSyntaxHighlighter {
 private:
  class Impl;
  Impl* impl_;
  SyntaxHighlighter(const SyntaxHighlighter&) = delete;
  SyntaxHighlighter& operator=(const SyntaxHighlighter&) = delete;
 protected:
  void highlightBlock(const QString& text) override;
 public:
  explicit SyntaxHighlighter(QObject* parent = nullptr);
  ~SyntaxHighlighter() override;
 };

}