#pragma once
#include "E:\Qt\Qt6.8.0\6.8.0\msvc2022_64\include\QtCore\qobject.h"
#include <QGraphicsView>
#include<qtimer.h>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include<qtimer.h>
class RotatingAlbumArt :
    public QWidget
{
public:
    RotatingAlbumArt(QWidget* parent = nullptr);
        
    

    void startRotation(); 
    

    void stopRotation(); 

    QGraphicsView* view() const;

private:
    QGraphicsScene* scene;
    QGraphicsPixmapItem* pixmapItem;
    QGraphicsView* graphicsView;
    QTimer* rotationTimer;

};

