#include "ZZNGraphicsScene.h"
#include <QGraphicsSceneMouseEvent>

ZZNGraphicsScene::ZZNGraphicsScene(QObject *parent) : QGraphicsScene(parent)
{

}

void ZZNGraphicsScene::keyPressEvent(QKeyEvent *event)
{
    emit sceneKeyPress(event);
    QGraphicsScene::keyPressEvent(event);
}

void ZZNGraphicsScene::keyReleaseEvent(QKeyEvent *event)
{
    emit sceneKeyRelease(event);
    QGraphicsScene::keyReleaseEvent(event);
}

void ZZNGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        emit sceneMousePressPoint(event->scenePos());
    }
    QGraphicsScene::mousePressEvent(event);
}

void ZZNGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        emit sceneMouseReleasePoint(event->scenePos());
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void ZZNGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        emit sceneMouseDoubleClicked(event->scenePos());
    }
    QGraphicsScene::mouseDoubleClickEvent(event);
}

void ZZNGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if(event->delta()>0){
        emit sceneWheelSlideUp(true);
    }else{
        emit sceneWheelSlideUp(false);
    }
    QGraphicsScene::wheelEvent(event);
}
