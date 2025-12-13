module; 
#include <QCheckBox>
#include <QHBoxLayout>
module Widgets.Knob.CheckBox;





namespace ArtifactWidgets {

	class KnobCheckBox::Impl {
	public:
		QHBoxLayout layout;
		QCheckBox checkbox;

		Impl(QWidget* parent)
			: layout(), checkbox(parent) {
			layout.setContentsMargins(0, 0, 0, 0);
			layout.addWidget(&checkbox);

			QObject::connect(&checkbox, &QCheckBox::toggled, parent, [this](bool checked) {
				emit static_cast<KnobCheckBox*>(checkbox.parent())->valueChanged(checked);
				});
		}
	};

	// コンストラクタ & デストラクタ
	KnobCheckBox::KnobCheckBox(QWidget* parent)
		: QWidget(parent), d(new Impl(this)) {
		setLayout(&d->layout);
	}

	KnobCheckBox::~KnobCheckBox() {
		delete d;
	}

	// 値の取得 & 設定
	void KnobCheckBox::setValue(bool checked) {
		d->checkbox.setChecked(checked);
	}

	bool KnobCheckBox::value() const {
		return d->checkbox.isChecked();
	}

}