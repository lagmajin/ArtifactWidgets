module;
#include <QPushButton>
#include <QColor>
#include <QFont>
#include <wobjectimpl.h>

export module ToggleGlyphButton;


export namespace ArtifactWidgets
{
 class TextIconButton : public QPushButton
 {
  //Q_OBJECT
 public:
  explicit TextIconButton(QWidget* parent = nullptr);

  void setTextColor(const QColor& color);
  void setBackgroundColor(const QColor& color);
  void setBold(bool bold);
  void setFontSize(int pointSize);

 protected:
  void updateStyle();

 private:
  QColor m_textColor = Qt::white;
  QColor m_backgroundColor = Qt::red;
  bool m_bold = true;
  int m_fontSize = 14;
 };











};