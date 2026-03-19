module;

#include <QMouseEvent>
#include <QApplication>
#include <QCursor>

module DragSpinBox;

namespace ArtifactWidgets {

class DragSpinBox::Impl {
public:
    QPoint pressPos;
    int startValue = 0;
    bool dragging = false;
    double speed = 1.0;
};

DragSpinBox::DragSpinBox(QWidget* parent) : QSpinBox(parent), impl_(new Impl) {
    setAlignment(Qt::AlignRight);
    setMaximumWidth(100);
}

DragSpinBox::~DragSpinBox() {
    delete impl_;
}

void DragSpinBox::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        impl_->pressPos = event->globalPosition().toPoint();
        impl_->startValue = value();
        impl_->dragging = false;
        event->accept();
    } else {
        QSpinBox::mousePressEvent(event);
    }
}

void DragSpinBox::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->globalPosition().toPoint() - impl_->pressPos;
        if (!impl_->dragging && delta.manhattanLength() > QApplication::startDragDistance()) {
            impl_->dragging = true;
            setCursor(Qt::SizeHorCursor);
        }

        if (impl_->dragging) {
            int step = singleStep();
            if (event->modifiers() & Qt::ShiftModifier) step *= 10;
            if (event->modifiers() & Qt::ControlModifier) step = std::max(1, step / 10);

            int newValue = impl_->startValue + (delta.x() * step);
            setValue(newValue);
            event->accept();
            return;
        }
    }
    QSpinBox::mouseMoveEvent(event);
}

void DragSpinBox::mouseReleaseEvent(QMouseEvent* event) {
    if (impl_->dragging) {
        impl_->dragging = false;
        unsetCursor();
        event->accept();
    } else {
        QSpinBox::mouseReleaseEvent(event);
    }
}

class DoubleDragSpinBox::Impl {
public:
    QPoint pressPos;
    double startValue = 0.0;
    bool dragging = false;
};

DoubleDragSpinBox::DoubleDragSpinBox(QWidget* parent) : QDoubleSpinBox(parent), impl_(new Impl) {
    setAlignment(Qt::AlignRight);
    setMaximumWidth(100);
}

DoubleDragSpinBox::~DoubleDragSpinBox() {
    delete impl_;
}

void DoubleDragSpinBox::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        impl_->pressPos = event->globalPosition().toPoint();
        impl_->startValue = value();
        impl_->dragging = false;
        event->accept();
    } else {
        QDoubleSpinBox::mousePressEvent(event);
    }
}

void DoubleDragSpinBox::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->globalPosition().toPoint() - impl_->pressPos;
        if (!impl_->dragging && delta.manhattanLength() > QApplication::startDragDistance()) {
            impl_->dragging = true;
            setCursor(Qt::SizeHorCursor);
        }

        if (impl_->dragging) {
            double step = singleStep();
            if (event->modifiers() & Qt::ShiftModifier) step *= 10.0;
            if (event->modifiers() & Qt::ControlModifier) step *= 0.1;

            double newValue = impl_->startValue + (delta.x() * step);
            setValue(newValue);
            event->accept();
            return;
        }
    }
    QDoubleSpinBox::mouseMoveEvent(event);
}

void DoubleDragSpinBox::mouseReleaseEvent(QMouseEvent* event) {
    if (impl_->dragging) {
        impl_->dragging = false;
        unsetCursor();
        event->accept();
    } else {
        QDoubleSpinBox::mouseReleaseEvent(event);
    }
}

} // namespace ArtifactWidgets
