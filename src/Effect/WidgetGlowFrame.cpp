module;
#include <QPainter>
#include <QPaintEvent>


module Widgets.Effects.GlowFrame;


namespace ArtifactWidgets
{
 class GlowFrame::Impl
 {
 private:

 public:
  Impl();
  ~Impl();
 };

 GlowFrame::Impl::Impl()
 {

 }

 void GlowFrame::paintEvent(QPaintEvent*)
 {
  if (!focused_) return;

  QPainter p(this);
  QColor glowColor = QColor(100, 180, 255, 180);  // 半透明の青
  p.setPen(QPen(glowColor, 3));
  p.drawRect(rect().adjusted(1, 1, -2, -2));
 }

 GlowFrame::GlowFrame(QWidget* parent /*= nullptr*/) : QWidget(parent), focused_(false)
 {
  setAttribute(Qt::WA_TransparentForMouseEvents);
  setAttribute(Qt::WA_NoSystemBackground);
  setAttribute(Qt::WA_TranslucentBackground);
  setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::BypassWindowManagerHint);
 }

 void GlowFrame::setFocused(bool f)
 {
  if (focused_ != f) {
   focused_ = f;
   update();
  }
 }

}