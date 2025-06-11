#include "RotatingAlbumArt.h"

RotatingAlbumArt::RotatingAlbumArt(QWidget* parent)
{
    // 创建场景
    scene = new QGraphicsScene(parent);

    // 加载专辑图片
    QPixmap albumArt(":/Icon/musicArt.png"); // 使用资源系统路径
    albumArt =  albumArt.scaled(parent->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    // 创建并添加图片项到场景
    pixmapItem = scene->addPixmap(albumArt);

    pixmapItem->setTransformationMode(Qt::SmoothTransformation);
    
    
    pixmapItem->setOffset(-albumArt.width() / 2, -albumArt.height() / 2); // 居中
    QRectF rect = pixmapItem->boundingRect();
    pixmapItem->setTransformOriginPoint(rect.center());

    // 创建视图并设置场景
    graphicsView = new QGraphicsView(parent);
    graphicsView->setScene(scene);
    graphicsView->setRenderHint(QPainter::Antialiasing); // 抗锯齿
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setStyleSheet("background: transparent; border: none;");

    // 创建定时器实现旋转动画
    rotationTimer = new QTimer(this);
    connect(rotationTimer, &QTimer::timeout, [=]() {
        // 每帧旋转0.5度
        pixmapItem->setRotation(pixmapItem->rotation() + 0.5);
        });
}

void RotatingAlbumArt::startRotation()
{
    rotationTimer->start(16); // ~60fps
}

void RotatingAlbumArt::stopRotation()
{
    rotationTimer->stop();
}

QGraphicsView* RotatingAlbumArt::view() const
{
    return graphicsView;
}



