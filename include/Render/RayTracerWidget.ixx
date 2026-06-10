module;
#include <utility>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <wobjectdefs.h>

export module ArtifactWidgets.Render.RayTracerWidget;

import Render;

export namespace ArtifactWidgets {

class RayTracerWidget : public QWidget
{
    W_OBJECT(RayTracerWidget)
public:
    explicit RayTracerWidget(QWidget* parent = nullptr);
    ~RayTracerWidget() override;

    void renderScene();
    void savePNG(const QString& path);

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    QTimer* timer_ = nullptr;
    QImage renderedImage_;
    ArtifactCore::RayTrace::SoftwareRayTracer tracer_;
};

} // namespace ArtifactWidgets