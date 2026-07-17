module;

#include <QWidget>

module ToggleGlyphButton;


namespace ArtifactWidgets
{
 TextIconButton::TextIconButton(QWidget* parent)
  : QPushButton(parent)
 {
  updateStyle();
 }

 void TextIconButton::setTextColor(const QColor& color)
 {
  m_textColor = color;
  updateStyle();
 }

 void TextIconButton::setBackgroundColor(const QColor& color)
 {
  m_backgroundColor = color;
  updateStyle();
 }

 void TextIconButton::setBold(bool bold)
 {
  m_bold = bold;
  updateStyle();
 }

 void TextIconButton::setFontSize(int pointSize)
 {
  m_fontSize = pointSize;
  updateStyle();
 }

 void TextIconButton::updateStyle()
 {
  QPalette pal = palette();
  pal.setColor(QPalette::Button, m_backgroundColor);
  pal.setColor(QPalette::ButtonText, m_textColor);
  pal.setColor(QPalette::Light, m_backgroundColor.darker(120));
  pal.setColor(QPalette::Dark, m_backgroundColor.darker(150));
  setPalette(pal);
  setAutoFillBackground(true);

  QFont fnt = font();
  fnt.setBold(m_bold);
  fnt.setPointSize(m_fontSize);
  setFont(fnt);
 }











};