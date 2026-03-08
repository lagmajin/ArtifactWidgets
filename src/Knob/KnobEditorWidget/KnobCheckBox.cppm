#include <QCheckBox>
#include <QHBoxLayout>
#include "../../../include/Knob/KnobCheckBox.ixx"


namespace ArtifactWidgets {

	class KnobCheckBox::Impl {
	public:
		QHBoxLayout layout;
		QCheckBox checkbox;

		Impl(QWidget* parent)
			: layout(), checkbox(parent) {
			layout.setContentsMargins(0, 0, 0, 0);
			layout.addWidget(&checkbox);
		}
	};

	// コンストラクタ & デストラクタ
	KnobCheckBox::KnobCheckBox(QWidget* parent)
		: QCheckBox(parent), d(new Impl(this)) {
		setLayout(&d->layout);
	}

	KnobCheckBox::~KnobCheckBox() {
		delete d;
	}

	// 値の取得 & 設定
	void KnobCheckBox::setValue(bool checked) {
		d->checkbox.setChecked(checked);
	}

	bool KnobCheckBox::checkBox() const {
		return d->checkbox.isChecked();
	}

}