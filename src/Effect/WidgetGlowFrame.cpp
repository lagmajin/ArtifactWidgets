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
  p.setRenderHint(QPainter::Antialiasing);
  QColor glowColor = QColor(100, 180, 255, 180);
  p.setPen(QPen(glowColor, 3));
  p.setBrush(Qt::NoBrush);
  p.drawRect(rect().adjusted(1, 1, -2, -2));
 }

 GlowFrame::GlowFrame(QWidget* parent /*= nullptr*/) : QWidget(parent), focused_(false)
 {
  // 子ウィジェットとして透過オーバーレイにする
  setAttribute(Qt::WA_TransparentForMouseEvents);  // マウスイベントを親に透過
  setAttribute(Qt::WA_TranslucentBackground);       // 背景を透過描画
  // ※ WA_NoSystemBackground と FramelessWindowHint は設定しない
  // → 子ウィジェットとして親に重ねるだけ
  raise();  // 最前面に表示
 }

 void GlowFrame::setFocused(bool f)
 {
  if (focused_ != f) {
   focused_ = f;
   update();
  }
 }

}