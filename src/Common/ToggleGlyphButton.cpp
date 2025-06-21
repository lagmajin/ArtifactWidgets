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
  QString style = QString(
   "QPushButton {"
   "background-color: %1;"
   "color: %2;"
   "font-weight: %3;"
   "font-size: %4pt;"
   "border: none;"
   "border-radius: 4px;"
   "}"
   "QPushButton:hover {"
   "background-color: %5;"
   "}"
   "QPushButton:pressed {"
   "background-color: %6;"
   "}"
  )
   .arg(m_backgroundColor.name())
   .arg(m_textColor.name())
   .arg(m_bold ? "bold" : "normal")
   .arg(m_fontSize)
   .arg(m_backgroundColor.darker(120).name())
   .arg(m_backgroundColor.darker(150).name());

  setStyleSheet(style);
 }











};