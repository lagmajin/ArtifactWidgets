module;
#include <DockWidget.h>
#include <QBoxLayout>
#include <QPushButton>
#include <wobjectimpl.h>
module DockWidget;

import Widgets.Effects.GlowFrame;


namespace ArtifactWidgets
{

//import Widgets.Effects.GlowFrame;
 W_OBJECT_IMPL(Pane);

 class Pane::Impl
 {
 private:

 public:
  Impl();
  ~Impl();
  GlowFrame* glowFrame =nullptr;
 };

 Pane::Impl::Impl()
 {

 }

 Pane::Impl::~Impl()
 {

 }

 Pane::Pane(const QString& title, QWidget* contentWidget, QWidget* parent /*= nullptr*/) : ads::CDockWidget(title, parent),impl_(new Impl())
 {
  setAttribute(Qt::WA_NoSystemBackground);
  setAttribute(Qt::WA_TranslucentBackground);

  // ヘッダー部分を作成 (変更なし)
  auto header = new QWidget(this);
  auto hlayout = new QHBoxLayout(header);
  hlayout->setContentsMargins(4, 2, 4, 2);
  //auto label = new QLabel(title);
  //hlayout->addWidget(label);
  auto btnCollapse = new QPushButton("-");
  btnCollapse->setFixedSize(16, 16);
  hlayout->addWidget(btnCollapse);

  // コンテンツとヘッダーを格納するメインのレイアウトを作成
  auto mainLayout = new QVBoxLayout();
  mainLayout->setSpacing(0);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  mainLayout->addWidget(header);
  mainLayout->addWidget(contentWidget, 1); // contentWidget を直接レイアウトに追加

  // mainLayout を保持するための QWidget を作成し、これを Pane のコンテンツとして設定
  auto mainContentWrapper = new QWidget(this); // 親を Pane (this) に
  mainContentWrapper->setLayout(mainLayout);
  mainContentWrapper->setAutoFillBackground(false);
  mainContentWrapper->setAttribute(Qt::WA_NoSystemBackground);
  mainContentWrapper->setAttribute(Qt::WA_TranslucentBackground);
  mainContentWrapper->setStyleSheet("background-color: transparent;");


  impl_->glowFrame = new GlowFrame(nullptr);
  impl_->glowFrame->setParent(mainContentWrapper);

  impl_->glowFrame->setGeometry(rect());
  impl_->glowFrame->show();
  impl_->glowFrame->raise();

  // ads::CDockWidget のコンテンツとして mainContentWrapper を設定
  setWidget(mainContentWrapper);

  // contentWidget のサイズポリシーはここで設定
  contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  // ボタンの接続もそのまま
  connect(btnCollapse, &QPushButton::clicked, [=]() {
   contentWidget->setVisible(!contentWidget->isVisible());
   // コラプス/展開時にレイアウトの更新を強制する
   mainContentWrapper->updateGeometry();
   this->updateGeometry();
   });
 }

 void Pane::focusInEvent(QFocusEvent* event)
 {
  impl_->glowFrame->setFocused(true);
 }

 void Pane::focusOutEvent(QFocusEvent* event)
 {
  impl_->glowFrame->setFocused(false);
 }

 Pane::~Pane()
 {
  delete impl_;
 }

 void Pane::resizeEvent(QResizeEvent* event)
 {
  impl_->glowFrame->resize(event->size());
 }

};



