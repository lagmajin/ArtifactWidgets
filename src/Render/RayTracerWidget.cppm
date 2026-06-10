module;
#include <utility>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QPaintEvent>
#include <QFileDialog>
#include <QDateTime>
#include <QFileInfo>
#include <QKeyEvent>
#include <wobjectimpl.h>

module ArtifactWidgets.Render.RayTracerWidget;

import Render;

W_OBJECT_IMPL(ArtifactWidgets::RayTracerWidget)

namespace ArtifactWidgets {

RayTracerWidget::RayTracerWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("3D Ray Tracer"));
    setMinimumSize(800, 600);
    setFocusPolicy(Qt::StrongFocus);

    tracer_.setupCornellBox();

    timer_ = new QTimer(this);
    timer_->setInterval(100);
    connect(timer_, &QTimer::timeout, this, [this]() {
        renderScene();
    });
    timer_->start();
}

RayTracerWidget::~RayTracerWidget()
{
}

void RayTracerWidget::renderScene()
{
    tracer_.imageWidth = width();
    tracer_.imageHeight = height();
    auto buffer = tracer_.render();

    QImage img(buffer.width, buffer.height, QImage::Format_RGB888);
    for (int y = 0; y < buffer.height; ++y)
    {
        for (int x = 0; x < buffer.width; ++x)
        {
            int idx = (y * buffer.width + x) * 3;
            img.setPixel(x, y, qRgb(buffer.pixels[idx], buffer.pixels[idx + 1], buffer.pixels[idx + 2]));
        }
    }
    renderedImage_ = img;
    update();
}

void RayTracerWidget::savePNG(const QString& path)
{
    if (!renderedImage_.isNull())
        renderedImage_.save(path, "PNG");
}

void RayTracerWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    if (!renderedImage_.isNull())
    {
        painter.drawImage(0, 0, renderedImage_.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    painter.setPen(QColor(240, 240, 240));
    painter.drawText(
        QRect(10, 10, width() - 20, 44),
        Qt::AlignLeft | Qt::AlignTop,
        QStringLiteral("3D Ray Tracer - R: Render  S: Save"));
}

void RayTracerWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_R)
    {
        renderScene();
        event->accept();
        return;
    }
    if (event->key() == Qt::Key_S)
    {
        const QString defaultName = QStringLiteral("raytrace_%1.png")
            .arg(QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HHmmss")));
        const QString path = QFileDialog::getSaveFileName(
            this, QStringLiteral("Save Raytraced Image"), defaultName, QStringLiteral("PNG Image (*.png)"));
        if (!path.isEmpty())
            savePNG(path);
        event->accept();
        return;
    }
    QWidget::keyPressEvent(event);
}

} // namespace ArtifactWidgets