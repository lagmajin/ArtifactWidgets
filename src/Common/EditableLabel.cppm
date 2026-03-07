module;
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QBoxLayout>
#include <wobjectimpl.h>
#include <QEvent>
#include <QKeyEvent>

#include <QStackedLayout>

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <vector>
module Widgets.EditableLabel;

import Common.ClikableLabel;

namespace ArtifactWidgets
{

 class EditableLabel :: Impl
 {
 public:
  ClickableLabel* label = nullptr;
  QLineEdit* lineEdit = nullptr;
  QStackedLayout* stack = nullptr;
  QHBoxLayout* layout = nullptr;
  bool editing = false;
  bool skipFirstClick = false;
  QString beforeEditText;
 	
  void animateSwitch(QWidget* from, QWidget* to);
  void switchToEdit() {
   if (editing) return;
   editing = true;

   beforeEditText = label->text();

   //label->hide();
   lineEdit->setText(beforeEditText);
   lineEdit->selectAll();
   stack->setCurrentWidget(lineEdit);
   lineEdit->setFocus();
  	
   animateSwitch(label, lineEdit);
  }

  void switchToLabel(bool commit) {
   if (!editing) return;
   editing = false;

   if (commit)
	label->setText(lineEdit->text());
   else
	lineEdit->setText(beforeEditText);

   stack->setCurrentWidget(label);
  	
   animateSwitch(lineEdit, label);
  }
 };

 void EditableLabel::Impl::animateSwitch(QWidget* from, QWidget* to)
 {
  // アウト
  auto effOut = new QGraphicsOpacityEffect(from);
  from->setGraphicsEffect(effOut);

  auto animOut = new QPropertyAnimation(effOut, "opacity");
  animOut->setDuration(120);
  animOut->setStartValue(1.0);
  animOut->setEndValue(0.0);

  // イン
  auto effIn = new QGraphicsOpacityEffect(to);
  to->setGraphicsEffect(effIn);
  effIn->setOpacity(0.0);
  to->show();

  auto animIn = new QPropertyAnimation(effIn, "opacity");
  animIn->setDuration(120);
  animIn->setStartValue(0.0);
  animIn->setEndValue(1.0);

  // 終了処理
  QObject::connect(animOut, &QPropertyAnimation::finished, from, [from, effOut] {
   from->hide();
   from->setGraphicsEffect(nullptr);
   //effOut->deleteLater();
   });

  QObject::connect(animIn, &QPropertyAnimation::finished, to, [to, effIn] {
   to->setGraphicsEffect(nullptr);
   //effIn->deleteLater();
   });

  animOut->start(QAbstractAnimation::DeleteWhenStopped);
  animIn->start(QAbstractAnimation::DeleteWhenStopped);
 }

	W_OBJECT_IMPL(EditableLabel)

 EditableLabel::EditableLabel(QWidget* parent /*= nullptr*/) : QWidget(parent), impl_(new Impl())
 {
  impl_->label = new ClickableLabel(this);
  impl_->lineEdit = new QLineEdit(this);
  impl_->lineEdit->hide();
  impl_->stack = new QStackedLayout(this);
  impl_->stack->setContentsMargins(0, 0, 0, 0);
  impl_->stack->addWidget(impl_->label);
  impl_->stack->addWidget(impl_->lineEdit);
  impl_->stack->setCurrentWidget(impl_->label);

  //impl_->layout = new QHBoxLayout(this);
  //impl_->layout->addWidget(impl_->label);
  //impl_->layout->addWidget(impl_->lineEdit);
  //impl_->layout->setContentsMargins(0, 0, 0, 0);
  connect(impl_->label, &ClickableLabel::clicked, this, [this] { impl_->switchToEdit(); });
  connect(impl_->lineEdit, &QLineEdit::editingFinished, this, [this] { impl_->switchToLabel(true);
  ; });
  impl_->lineEdit->installEventFilter(this);
		
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
 }

 EditableLabel::~EditableLabel()
 {
  delete impl_;
 }
 QString EditableLabel::text() const
 {
  return impl_->label->text();
 }
	
	
 void EditableLabel::setText(const QString& text)
 {
  impl_->label->setText(text);
 }

void EditableLabel::startEdit()
{
  impl_->switchToEdit();
}

void EditableLabel::finishEdit()
{
  impl_->switchToLabel(true);
}

 bool EditableLabel::eventFilter(QObject* obj, QEvent* event)
 {
  if (obj == impl_->lineEdit && event->type() == QEvent::KeyPress) {
   QKeyEvent* key = static_cast<QKeyEvent*>(event);
   if (key->key() == Qt::Key_Escape) {
	impl_->switchToLabel(false);  // commit=false → 元に戻す
	return true;
   }
  }
  return QWidget::eventFilter(obj, event);
 }

 QSize EditableLabel::sizeHint() const
 {
  if (impl_->editing)
   return impl_->lineEdit->sizeHint();
  return impl_->label->sizeHint();
 }

};
