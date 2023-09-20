#ifndef ZZNGRAPHICSSCENE_H
#define ZZNGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QObject>

class ZZNGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);

public:
    explicit ZZNGraphicsScene(QObject *parent = nullptr);

signals:
    void sceneMousePressPoint(QPointF point);
    void sceneMouseReleasePoint(QPointF point);
    void sceneMouseDoubleClicked(QPointF point);
    void sceneKeyPress(QKeyEvent *event);
    void sceneKeyRelease(QKeyEvent *event);
    void sceneWheelSlideUp(bool isUp);

};

#endif // ZZNGRAPHICSSCENE_H
