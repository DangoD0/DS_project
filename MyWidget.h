#pragma once
#include <QtWidgets/QWidget>
#include <qpainter.h>
#include <QMouseEvent>
#include <QTimer.h>
#define NUM 100
#define _HEIGHT_ 700
#define _WIDTH_ 1000
#define friction 0.96f
class MyWidget : public QWidget
{
	Q_OBJECT
private:
	struct Ball {
		float r;
		float ball_x, v_x;
		float ball_y, v_y;
		QColor color;
	};
	Ball ball[NUM];
	int mouseX, mouseY;
	int preMouseX, preMouseY;
	int isMouseDown;
	QTimer* timer;
public:
	MyWidget(QWidget* parent = 0);
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void updateBall();
};