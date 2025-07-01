#pragma once

#include <QPushButton.h>
#include <qlabel.h>
#include "ui_test.h"
#include <qlistwidget.h>
#include "topsort.h"
#include <qpropertyanimation.h>
#include <qgraphicseffect.h>
#include <QParallelAnimationGroup>
#include <qevent.h>
#include <qpushbutton.h>
#include <qstring.h>
class QAnimatedButton : public QPushButton
{
	Q_OBJECT
public:
	QAnimatedButton(const QString& text, QWidget* parent = 0);
	QAnimatedButton();
	void InitAnimation(int x,int y,int s_x,int s_y);
	void hoverLeaveAnimation();
	void hoverEnterAnimation();
	void pressAnimation();
	bool eventFilter(QObject* obj, QEvent* event);
signals:
	void end();
	void clicked();
};
class QAnimatedLabel : public QLabel
{
	Q_OBJECT

public:
	QAnimatedLabel(QWidget* parent = 0);
	void InitAnimation(int x, int y);
	void hoverLeaveAnimation();
	void hoverEnterAnimation();
	void doublePressAnimation();
	void pressAnimation();
	bool eventFilter(QObject* obj, QEvent* event);
};
class QAnimatedNode : public QAnimatedButton
{
	Q_OBJECT
public:
	QAnimatedNode(const QString& text, QWidget* parent = 0);
	void hoverLeaveAnimation();
	void hoverEnterAnimation();
	bool eventFilter(QObject* obj, QEvent* event);
};