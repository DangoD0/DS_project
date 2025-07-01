#include "MyWidget.h"

MyWidget::MyWidget(QWidget* parent) : QWidget(parent)
{
	setParent(parent);
	setFixedSize(_WIDTH_, _HEIGHT_);
	setStyleSheet(("QWidget{background-color: rgb(57,57,57);}"));
	setMouseTracking(true);
	srand((unsigned int)time(NULL));
	int i, j;
	mouseX = preMouseX = _WIDTH_ / 2;
	mouseY = preMouseY = _HEIGHT_ / 2;
	isMouseDown = 0;
	for (i = 0; i < NUM; i++) {
		ball[i].ball_x = rand() % _WIDTH_;
		ball[i].ball_y = rand() % _HEIGHT_;
		ball[i].color = QColor(rand() % 256, rand() % 256, rand() % 256);
		ball[i].v_x = float(cos(float(i))) * (rand() % 34);
		ball[i].v_y = float(sin(float(i))) * (rand() % 34);
		ball[i].r = (rand() % 34) / 15.0;
	}
	
	timer = new QTimer(this);
	timer->setTimerType(Qt::PreciseTimer);
	timer->setInterval(5);
	timer->start();
	connect(timer, &QTimer::timeout, this, [=]() {
			updateBall();
			update();
			timer->start();
		});
		
}

void MyWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	int i, j;
	for (i = 0; i < NUM; i++) {
		painter.setPen(ball[i].color);
		painter.setBrush(ball[i].color);
		painter.drawEllipse(QPointF(ball[i].ball_x, ball[i].ball_y), ball[i].r, ball[i].r);
	}
}

void MyWidget::mousePressEvent(QMouseEvent* event)
{
	isMouseDown = 1;
}
void MyWidget::mouseReleaseEvent(QMouseEvent* event)
{
	isMouseDown = 0;
}
void MyWidget::mouseMoveEvent(QMouseEvent* event)
{
	mouseX = event->pos().x();
	mouseY = event->pos().y();
}

void MyWidget::updateBall()
{
	float aDis = _WIDTH_ * 0.86;
	float blowDis = _HEIGHT_ * 0.5;
	float disturbDis = _WIDTH_ * 0.125;
	int mouseVX = mouseX - preMouseX;
	int mouseVY = mouseY - preMouseY;
	preMouseX = mouseX;
	preMouseY = mouseY;
	for (int i = 0; i < NUM; i++) {
		float dx = ball[i].ball_x - mouseX;
		float dy = ball[i].ball_y - mouseY;
		float d = sqrt(dx * dx + dy * dy);
		/*dx /= fabs(dx);
		dy /= fabs(dy);*/
		dx /= d;
		dy /= d;
		if (d < aDis) {
			float aAcc = (1 - d / aDis) * 1.0;
			ball[i].v_x -= dx * aAcc;
			ball[i].v_y -= dy * aAcc;
		}
		if (isMouseDown && d < blowDis) {
			float blowAcc = (1 - d / blowDis) * 10.0;
			ball[i].v_x = ball[i].v_x + dx * blowAcc + 0.5f - ((float)rand()) / RAND_MAX;
			ball[i].v_y = ball[i].v_y + dy * blowAcc + 0.5f - ((float)rand()) / RAND_MAX;
		}
		if (d < disturbDis) {
			float mAcc = (1 - d / disturbDis) * 0.26;
			ball[i].v_x += mouseVX * mAcc;
			ball[i].v_y += mouseVY * mAcc;
		}
		float x = ball[i].ball_x + ball[i].v_x;
		float y = ball[i].ball_y + ball[i].v_y;
		ball[i].ball_x = x > _WIDTH_ ? (ball[i].v_x = -ball[i].v_x, _WIDTH_) : x < 0 ? (ball[i].v_x = -ball[i].v_x, 0) : x;
		ball[i].ball_y = y > _HEIGHT_ ? (ball[i].v_y = -ball[i].v_y, _HEIGHT_) : y < 0 ? (ball[i].v_y = -ball[i].v_y, 0) : y;
		ball[i].v_x *= friction;
		ball[i].v_y *= friction;
		float avgVX = fabs(ball[i].v_x);
		float avgVY = fabs(ball[i].v_y);
		float avgV = (avgVX + avgVY) * 0.5f;
		avgVX < 0.1 ? (ball[i].v_x *= float(rand()) / RAND_MAX * 3) : NULL;
		avgVY < 0.1 ? (ball[i].v_y *= float(rand()) / RAND_MAX * 3) : NULL;
		ball[i].r = std::max(std::min(avgV * 0.45f, 3.5f), 0.4f);
	}
}