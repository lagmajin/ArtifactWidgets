module;
#include <QStyledItemDelegate>
#include <QPainter>
#include "../Define/DllExportMacro.hpp"

export module AccentDelegate;

export namespace ArtifactWidgets {

 class WIDGET_LIBRARY_DLL_API AccentDelegate : public QStyledItemDelegate {
 public:
  using QStyledItemDelegate::QStyledItemDelegate;

  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
   // 1. まず標準の描画を行う（背景やテキストなど）
   QStyledItemDelegate::paint(painter, option, index);

   // 2. 選択中かつアクティブな場合のみ、左側に縦線を入れる
   if (option.state & QStyle::State_Selected) {
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);

	// 線の設定（色と太さ）
	const int lineWidth = 3; // 縦線の太さ
	const int padding = 4;   // 上下の余白（お好みで）
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor("#0078d4")); // Windows風のアクセントカラー

	// 描画範囲の計算（左端に配置）
	QRect accentRect = option.rect;
	accentRect.setWidth(lineWidth);
	accentRect.adjust(0, padding, 0, -padding);

	painter->drawRect(accentRect);
	painter->restore();
   }
  }
 };



};