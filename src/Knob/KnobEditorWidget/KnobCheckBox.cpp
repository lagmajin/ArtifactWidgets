import module KnobCheckBox;
#include <QtWidgets/QCheckBox>




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

	// �R���X�g���N�^ & �f�X�g���N�^
	KnobCheckBox::KnobCheckBox(QWidget* parent)
		: QWidget(parent), d(new Impl(this)) {
		setLayout(&d->layout);
	}

	KnobCheckBox::~KnobCheckBox() {
		delete d;
	}

	// �l�̎擾 & �ݒ�
	void KnobCheckBox::setValue(bool checked) {
		d->checkbox.setChecked(checked);
	}

	bool KnobCheckBox::value() const {
		return d->checkbox.isChecked();
	}

}