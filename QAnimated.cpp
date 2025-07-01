
#include "QAnimated.h"


QAnimatedButton::QAnimatedButton(const QString& text, QWidget* parent)
	: QPushButton(text, parent)
{
	this->setText(text);
	this->installEventFilter(this);
	this->setMouseTracking(true);
}
QAnimatedButton::QAnimatedButton()
{
	this->installEventFilter(this);
	this->setMouseTracking(true);
}
void QAnimatedButton::InitAnimation(int x,int y,int s_x,int s_y)
{
	QPropertyAnimation *moveAnimation = new QPropertyAnimation(this,"pos");
	moveAnimation->setDuration(500);
	moveAnimation->setStartValue(QPoint(s_x, s_y));
	moveAnimation->setEndValue(QPoint(x, y));
	moveAnimation->setEasingCurve(QEasingCurve::OutCubic);
	moveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
void QAnimatedButton::hoverLeaveAnimation()
{
	QPropertyAnimation *hoverAnimation = new QPropertyAnimation(this, "geometry");
	hoverAnimation->setDuration(250);
	this->setGraphicsEffect(nullptr);
	int x = this->x(),y = this->y();
	int width = this->width(),height = this->height();
	hoverAnimation->setStartValue(QRect(x + width/2, y + height/2, 0, 0));
	hoverAnimation->setEndValue(QRect(x, y, width, height));
	hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);
	hoverAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
void QAnimatedButton::hoverEnterAnimation()
{
	QGraphicsColorizeEffect* pColorizeEffect = new QGraphicsColorizeEffect(this);
	pColorizeEffect->setColor(QColor(192,192,192));
	this->setGraphicsEffect(pColorizeEffect);
	QPropertyAnimation* hoverAnimation = new QPropertyAnimation(pColorizeEffect, "color");
	hoverAnimation->setDuration(300);
	if (this->icon().isNull())
	{
		hoverAnimation->setStartValue(QColor(192, 192, 192));
		hoverAnimation->setEndValue(QColor(0, 0, 0));
	}
	else
	{
		hoverAnimation->setStartValue(QColor(255, 255, 255));
		hoverAnimation->setEndValue(QColorConstants::Transparent);
	}
	hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);
	hoverAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
void QAnimatedButton::pressAnimation()
{
	QGraphicsOpacityEffect* Effect = new QGraphicsOpacityEffect(this);
	Effect->setOpacity(1);
	this->setGraphicsEffect(Effect);
	QPropertyAnimation* Animation2 = new QPropertyAnimation(Effect, "opacity");
	Animation2->setDuration(150);
	Animation2->setStartValue(1);
	//Animation2->setKeyValueAt(0.5, 0);
	Animation2->setEndValue(0);
	Animation2->setEasingCurve(QEasingCurve::InCubic);
	Animation2->setLoopCount(2);
	Animation2->start(QAbstractAnimation::DeleteWhenStopped);
	connect(Animation2, &QPropertyAnimation::finished, this, [=]() {emit end();});
}
bool QAnimatedButton::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == this)
	{
		if (event->type() == QEvent::HoverLeave)
		{
			this->hoverLeaveAnimation();
			return true;
		}
		else if (event->type() == QEvent::HoverEnter)
		{
			this->hoverEnterAnimation();
			return true;
		}
		else if (event->type() == QEvent::MouseButtonRelease)
		{
			this->pressAnimation();
			emit clicked();
			return true;
		}
	}
	return QWidget::eventFilter(obj, event);
}

QAnimatedLabel::QAnimatedLabel(QWidget* parent)
{
	this->installEventFilter(this);
	this->setMouseTracking(true);
	this->setAttribute(Qt::WA_Hover, true);
}

void QAnimatedLabel::InitAnimation(int x, int y)
{
	QPropertyAnimation* moveAnimation = new QPropertyAnimation(this, "pos");
	moveAnimation->setDuration(500);
	moveAnimation->setStartValue(QPoint(600, 500));
	moveAnimation->setEndValue(QPoint(x, y));
	moveAnimation->setEasingCurve(QEasingCurve::OutCubic);
	moveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
void QAnimatedLabel::hoverLeaveAnimation()
{
	QPropertyAnimation* hoverAnimation = new QPropertyAnimation(this, "geometry");
	hoverAnimation->setDuration(250);
	hoverAnimation->setStartValue(QRect(430, 360, 250, 150));
	hoverAnimation->setEndValue(QRect(500, 400, 250, 150));
	hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);
	hoverAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
void QAnimatedLabel::hoverEnterAnimation()
{

	QPropertyAnimation* Animation1 = new QPropertyAnimation(this, "geometry");
	Animation1->setDuration(250);
	Animation1->setStartValue(QRect(500, 400, 250, 150));
	Animation1->setEndValue(QRect(430, 360, 250, 150));
	Animation1->setEasingCurve(QEasingCurve::OutCubic);
	Animation1->start(QAbstractAnimation::DeleteWhenStopped);
}
void QAnimatedLabel::doublePressAnimation()
{
	QGraphicsOpacityEffect* Effect = new QGraphicsOpacityEffect(this);
	Effect->setOpacity(0);
	this->setGraphicsEffect(Effect);
	QPropertyAnimation* Animation2 = new QPropertyAnimation(Effect, "opacity");
	Animation2->setDuration(150);
	Animation2->setStartValue(0);
	Animation2->setEndValue(1);
	Animation2->setEasingCurve(QEasingCurve::InCubic);
	Animation2->setLoopCount(2);
	Animation2->start(QAbstractAnimation::DeleteWhenStopped);
}
bool QAnimatedLabel::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == this)
	{
		QCursor* cursor;
		int x = cursor->pos().x();
		int y = cursor->pos().y();
		int lx = this->mapToGlobal(QPoint(500, 400)).x();
		int ly = this->mapToGlobal(QPoint(500, 400)).y();
		//qDebug() << x << " " << y;
		if (event->type() == QEvent::HoverLeave)
		{
			if (x <= lx && y <= ly )
			{
				this->hoverLeaveAnimation();
				return true;
			}
		}
		else if (event->type() == QEvent::HoverEnter)
		{
			if (lx <= x <= 600 && ly <= y <= 500)
			{
				this->hoverEnterAnimation();
				return true;
			}
		}
		else if (event->type() == QEvent::MouseButtonDblClick)
		{
			this->doublePressAnimation();
			return true;
		}
	}
	return QWidget::eventFilter(obj, event);
}

QAnimatedNode::QAnimatedNode(const QString& text, QWidget* parent) : QAnimatedButton(text,parent)
{
	this->setText(text);
	this->setParent(parent);
	this->installEventFilter(this);
	this->setMouseTracking(true);
	this->setFont(QFont("ËÎÌו", 12, QFont::Bold));
	this->resize(50, 50);
	this->setStyleSheet("QPushButton{background-color: green;"\
		"border: 2px solid black;"\
		"border-radius: 25px;}"\
		"QPushButton:hover{background-color: red;}");
}
void QAnimatedNode::hoverLeaveAnimation()
{
	QPropertyAnimation* hoverAnimation = new QPropertyAnimation(this, "geometry");
	hoverAnimation->setDuration(250);
	int x = this->x(), y = this->y();
	int width = this->width();
	hoverAnimation->setStartValue(QRect(x, y, width, 50));
	hoverAnimation->setEndValue(QRect(x + width / 2 - 25, y, 50, 50));
	hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);
	hoverAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
void QAnimatedNode::hoverEnterAnimation()
{
	QPropertyAnimation* hoverAnimation = new QPropertyAnimation(this, "geometry");
	hoverAnimation->setDuration(250);
	int x = this->x(), y = this->y();
	QFontMetrics metrics(this->font());
	int change = metrics.boundingRect(this->text()).width() * 1.5;
	hoverAnimation->setStartValue(QRect(x, y, 50, 50));
	hoverAnimation->setEndValue(QRect(x + 25 - change / 2, y, change, 50));
	hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);
	hoverAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
bool QAnimatedNode::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == this)
	{
		if (event->type() == QEvent::HoverLeave)
		{
			this->hoverLeaveAnimation();
			return true;
		}
		else if (event->type() == QEvent::HoverEnter)
		{
			this->hoverEnterAnimation();
			return true;
		}
	}
	return QWidget::eventFilter(obj, event);
}