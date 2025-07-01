#pragma once
#include <QtWidgets/QWidget>
#include <QPixMap.h>
#include <qpainter.h>
#include "QAnimated.h"

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsProxyWidget>
#include <QWheelEvent>
class GraphMenu : public QGraphicsScene
{
	Q_OBJECT
private:
    int flag = 0;
	QAnimatedNode** graph_node;
	graph* G;
	vector<pair<QPoint, QPoint>> edge_line;
    QAnimatedButton* back;
	QPoint start, end;
    QPoint star_point;
    QLabel* star[12];
public:
	GraphMenu(QWidget* parent = 0);
    //void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void initGraph();
    void mousePressEvent(QMouseEvent* event);
    void enter();
    //QRectF boundingRect() const;
signals:
    void mouseClick();
    void waitTM();
};
class View : public QGraphicsView
{
public:
    View(QGraphicsScene* scene) : QGraphicsView(scene), isDragging(false) {}

protected:
    void wheelEvent(QWheelEvent* event) override
    {
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        double scaleFactor = 1.15;
        if (event->angleDelta().y() > 0) {
            scale(scaleFactor, scaleFactor);
        }
        else {
            scale(1 / scaleFactor, 1 / scaleFactor);
        }
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton) {
            isDragging = true;
            lastPos = event->pos();
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (isDragging) {
            QPointF delta = mapToScene(event->pos()) - mapToScene(lastPos);
            lastPos = event->pos();
            setSceneRect(sceneRect().translated(-delta.x(), -delta.y()));
        }
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton) {
            isDragging = false;
        }
    }

private:
    bool isDragging;
    QPoint lastPos;
};
#include <QtWidgets/QWidget>
#include <QWheelEvent>
class graphMenu : public QWidget
{
	Q_OBJECT
protected:
    void wheelEvent(QWheelEvent* event) override
    {
        if (event->angleDelta().y() > 0)
            scale_factor *= 1.1;
        else
            scale_factor /= 1.1;

        // 在这里，你可以使用scale_factor来调整你的widget的大小
        // 例如，你可以调整widget的字体大小，或者如果它是一个图像，你可以调整图像的大小
        // this->resize(scale_factor * this->size());
    }
private:
	double scale_factor = 1.0;
};
