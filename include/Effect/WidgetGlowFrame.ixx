module;
#include <QWidget>
export module Widgets.Effects.GlowFrame;

export namespace ArtifactWidgets
{
 class GlowFrame : public QWidget {
 private:
  class Impl;
  Impl* impl_;

 public:
  GlowFrame(QWidget* parent = nullptr);

 protected:
  void paintEvent(QPaintEvent*) override;

 public slots:
  void setFocused(bool f);

 private:
  bool focused_;
 };












};