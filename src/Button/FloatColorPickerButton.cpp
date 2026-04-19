module;
#include <QPainter>
#include <QPushButton>
#include <wobjectimpl.h>

module FloatColorPickerButton;

import Color.Float;
import FloatColorPickerDialog;

namespace ArtifactWidgets {

W_OBJECT_IMPL(FloatColorPickerButton)

class FloatColorPickerButton::Impl {
public:
  Impl();
  ~Impl();
  void updateButtonColor();
  ArtifactCore::FloatColor currentColor;
};

void FloatColorPickerButton::Impl::updateButtonColor() {
  // ボタンの背景色を更新（この関数はボタンのインスタンスから呼ばれる）
}

FloatColorPickerButton::Impl::Impl() {
  currentColor =
      ArtifactCore::FloatColor(1.0f, 1.0f, 1.0f, 1.0f); // White by default
}

FloatColorPickerButton::Impl::~Impl() {}

FloatColorPickerButton::FloatColorPickerButton(QWidget *parent)
    : QPushButton(parent), impl_(new Impl()) {
  setText("");
  setFixedSize(30, 24);

  // 初期スタイルを設定
  updateButtonStyle();

  connect(this, &QPushButton::clicked, this, [this]() {
    FloatColorPicker picker(this);
    picker.setInitialColor(impl_->currentColor);
    if (picker.exec() == QDialog::Accepted) {
      impl_->currentColor = picker.getColor();
      updateButtonStyle();
      emit colorChanged(impl_->currentColor);
    }
  });
}

FloatColorPickerButton::~FloatColorPickerButton() { delete impl_; }

ArtifactCore::FloatColor FloatColorPickerButton::getColor() const {
  return impl_->currentColor;
}

void FloatColorPickerButton::setColor(const ArtifactCore::FloatColor &color) {
  impl_->currentColor = color;
  updateButtonStyle();
}

void FloatColorPickerButton::updateButtonStyle() {
  QColor qcolor =
      QColor::fromRgbF(impl_->currentColor.r(), impl_->currentColor.g(),
                       impl_->currentColor.b(), impl_->currentColor.a());

  // チェッカーボードパターンで透明度を表現
  QString style = QString(R"(
    QPushButton {
      background-color: %1;
      border: 1px solid #3E3E42;
      border-radius: 4px;
    }
    QPushButton:hover {
      border: 1px solid #0E639C;
    }
  )")
                      .arg(qcolor.name());

  setStyleSheet(style);
}

} // namespace ArtifactWidgets
