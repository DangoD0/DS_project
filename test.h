#pragma once

#include <QtWidgets/QWidget>
#include "ui_test.h"
#include "QAnimated.h"
#include <QLabel.h>
#include <QFile.h>
#include <qmessagebox.h>
#include <QFont.h>
#include <qdebug.h>
#include <QPixMap.h>
#include <qfileinfo.h>
#include <QMediaPlayer.h>
#include <QAudioOutput.h>
#include <QVideoWidget>
#include <qthread.h>
#include <qpainter.h>
#include <QTimer.h>
#include <qpropertyanimation.h>
#include <QParallelAnimationGroup>
#include <QMovie.h>
#include <QStackedWidget>
#include <qcheckbox.h>
#include <QLayout>
#include <qlistview.h>
#include <QTableWidget>
#include <QHeaderView>
#include <QAbstractItemView>
#include <qscroller.h>
#include <qscrollarea.h>
#include <qevent.h>
#include <stack>
#include <random>
#include <qlineedit.h>
#pragma execution_character_set("utf-8")
#define NUM 800
#define friction 0.96f
class ListLabel;
class MyListWidget;
class Line;
class MyLineEdit;
class MyTableWidget;
class test : public QWidget
{
    Q_OBJECT
public:
    test(int flag,QWidget* parent = 0);
    ~test();
    QString ReadQssFile(const QString& fileName);
    void MainMenu_endAnimation(int f);
    void courseMenu_endAnimation(MyListWidget** list,QAnimatedButton** chapter,QAnimatedButton* back, QAnimatedButton* right, QAnimatedButton* left,int flag = 0);
    void scheduleMenu_endAnimation(MyTableWidget** schedule, QAnimatedButton** chapter, QAnimatedButton* back);
    void mainMenu();
    void waitMenu(const int f);
    void scheduleMenu();
    void graphMenu();
    void initGraph();
    void courseMenu();
    void selectCourse(MyListWidget** list,int k,Line *l);
    void paintEvent(QPaintEvent* event);
    void paintGraph(int flag);
    void showEvent(QShowEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void updateBall();

private:
    Ui::testClass* ui;
    int flag = 0;
    int preListidx = 0;
    int preScheduleidx = 0;
    bool modifySchedule = false;
    QPoint star_point;
    QAnimatedLabel* icon = NULL;
    QLabel** star = NULL;
    QAnimatedButton* show_course = NULL;
    QAnimatedButton* show_schedule = NULL;
    QAnimatedButton* show_graph = NULL;
    QMediaPlayer* player = NULL;
    QVideoWidget* vw = NULL;
    QLabel* wait;
    graph* G;
    layer** semester;
    Line** w;
    vector<pair<QPoint, QPoint>> edge_line;
    int course1;
    vector<pair<QPoint, QPoint>> cTc;
    QLabel* bg1, * bg2;
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
signals:
    void waitTS();
    void waitTC();
    void waitTM();
    void waitTG();
    void scheduleS();
    void mainS();
    void courseS();
    void graphS();
    void mouseClick();
public slots:
    
};
class ListLabel : public QAnimatedLabel
{
    Q_OBJECT
public:
    ListLabel() : QAnimatedLabel()
    {
        this->resize(40, 40);
        this->setStyleSheet("QLabel{background-color: transparent;border: 0px ;border-radius: 20px}"\
            "QLabel:hover{background-color: transparent;border: 0px;border-radius: 20px}"\
            "QLabel:pressed{background-color: transparent;border: 0px;border-radius: 20px}");
        this->setAutoFillBackground(true);
    }
    void hoverLeaveAnimation()
    {
        QPropertyAnimation* hoverAnimation = new QPropertyAnimation(this, "geometry");
        hoverAnimation->setDuration(500);
        this->setGraphicsEffect(nullptr);
        int x = this->x(), y = this->y();
        int width = this->width(), height = this->height();
        hoverAnimation->setStartValue(QRect(x, y, width, height));
        hoverAnimation->setKeyValueAt(0.5, QRect(x + width / 2, y + height / 2, 0, 0));
        hoverAnimation->setEndValue(QRect(x, y, width, height));
        hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);
        hoverAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    void hoverEnterAnimation()
    {
        QGraphicsColorizeEffect* pColorizeEffect = new QGraphicsColorizeEffect(this);
        pColorizeEffect->setColor(QColor(192, 192, 192));
        this->setGraphicsEffect(pColorizeEffect);
        QPropertyAnimation* hoverAnimation = new QPropertyAnimation(pColorizeEffect, "color");
        hoverAnimation->setDuration(300);
        if (this->pixmap().isNull())
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
    bool eventFilter(QObject* obj, QEvent* event)
    {
        if (obj == this)
        {
            if (event->type() == QEvent::HoverLeave)
            {
                this->hoverLeaveAnimation();
                emit hoverOut();
                return true;
            }
            else if (event->type() == QEvent::HoverEnter)
            {
                this->hoverEnterAnimation();
                emit hoverIn();
                return true;
            }
        }
        return QWidget::eventFilter(obj, event);
    }
signals:
    void hoverIn();
    void hoverOut();
};
class MyStackedWidget : public QStackedWidget
{
	Q_OBJECT
public:
    MyStackedWidget(QWidget* parent = 0) :QStackedWidget(parent)
    {
		this->setMouseTracking(true);
        setWindowTitle(tr("课程管理系统"));
        setWindowIcon(QIcon(tr("picture/icon3.png")));
    }
    QSize sizeHint() const override
    {
        return currentWidget()->sizeHint();
    }

    QSize minimumSizeHint() const override
    {
        return currentWidget()->minimumSizeHint();
    }
    void addWidget(QWidget* w)
    {
        w->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
		QStackedWidget::addWidget(w);
	}
    void currentChanged(int index)
    {
        QWidget* pWidget = widget(index);
        Q_ASSERT(pWidget);
        pWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        pWidget->adjustSize();
        adjustSize();
    }
};
class Line:public QWidget
{
	Q_OBJECT
public:
    bool addFlag = true;
    Line(QWidget* parent = 0):QWidget(parent)
    {
        cb = new QCheckBox(this);
        idx = new QLabel(this);
        name = new QLabel(this);
        score = new QLabel(this);
        time = new QLabel(this);
        primary = new ListLabel();
        primary->setParent(this);
        layout = new QHBoxLayout(this);
        setStyleSheet(tr("QWidget{background-color: transparent;font:14px}"));
    }
    ~Line()
    {
        delete cb;
        delete idx;
        delete name;
        delete score;
        delete time;
        delete primary;
        delete layout;
    }
    void setData(QString name, float score, int idx, int day, int start_class, int end_class, bool flag)
    {
        cb->setText(tr("选中"));
        this->name->setText(name);
        this->score->setText(tr("   ") + QString::number(score));
        this->idx->setText(tr(" ") + QString::number(idx));
        this->time->setText(tr("   星期") + QString::number(day) + " " + QString::number(start_class) + "-" + QString::number(end_class) + tr("节"));
        if (flag) primary->setPixmap(QPixmap(tr("picture/YES.png")).scaled(primary->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));
        else primary->setPixmap(QPixmap(tr("picture/NO.png")).scaled(primary->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));
        layout->addWidget(cb);
        layout->addWidget(this->idx);
        layout->addWidget(this->name);
        layout->addWidget(this->score);
        layout->addWidget(this->time);
        layout->addWidget(primary);
        setLayout(layout);
        show();
    }
public:
    QCheckBox* cb = NULL;
    course c;
    QHBoxLayout* layout = NULL;
    QLabel* idx = NULL;
    QLabel* name = NULL;
    QLabel* score = NULL;
    QLabel* time = NULL;
    ListLabel* primary = NULL;
};
class MyListWidget : public QListWidget
{
	Q_OBJECT
public:
    MyListWidget(QWidget* parent = 0) :QListWidget(parent)
    {
        this->setMouseTracking(true);
        this->setViewMode(QListView::ListMode);
        this->setAlternatingRowColors(true);
        QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
        this->setVerticalScrollMode(QListWidget::ScrollPerPixel);
        //list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setStyleSheet("QListWidget{font-size:20px;background-color: white;}"\
            "color:rgb(0,0,0);"\
            "QListWidget::item{height:20px}"
            "QListWidget::item:hover{background-color:skyblue}");
        this->setGeometry(5, 200, 800, 500);
	}
};

class MyTableWidget : public QTableWidget
{
	Q_OBJECT
public:

    MyTableWidget(int rows, int columns, QWidget* parent = 0) :QTableWidget(rows, columns, parent)
    {
        this->setEditTriggers(QAbstractItemView::NoEditTriggers);
        this->setMouseTracking(true);
        this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        this->verticalHeader()->setVisible(false);
        this->setHorizontalHeaderLabels(QStringList() << tr("时间段") << tr("节次") << tr("星期一") << tr("星期二") << tr("星期三") << tr("星期四") << tr("星期五") << tr("星期六") << tr("星期日"));
        this->resizeColumnsToContents();
        this->setSelectionMode(QAbstractItemView::NoSelection);
        this->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        this->setStyleSheet("QTableWidget{background-color: white;gridline-color: #999999;border:1px solid #999999;}"\
            "QTableWidget::item{color:rgb(102,102,102);}"\
            "QTableWidget::item:hover{background-color: silver;color:white;}"
        );
        
        this->setSelectionBehavior(QAbstractItemView::SelectRows);
        this->setGeometry(5, 200, 800, 500);
        for (int i = 0; i < 3; i++)
        {
            QStringList list;
            list << tr("上午") << tr("下午") << tr("晚上");
            QTableWidgetItem* item = new QTableWidgetItem(list.at(i));
            //item->setForeground(QBrush(QColor(102, 102, 102)));
            item->setFont(QFont("Times", 12, QFont::Bold));
            item->setTextAlignment(Qt::AlignCenter);
            this->setSpan(i * 4, 0, 4, 1);
            this->setItem(i * 4, 0, item);
        }
        for (int i = 0; i < 12; i++)
        {
			QTableWidgetItem* item = new QTableWidgetItem(QString::number(i + 1));
			//item->setForeground(QBrush(QColor(102, 102, 102)));
			item->setFont(QFont("Times", 12, QFont::ExtraBold,true));
			item->setTextAlignment(Qt::AlignCenter);
			this->setItem(i, 1, item);
		}
	}
};

class MyLineEdit : public QWidget
{
	Q_OBJECT
public:
    QLineEdit* text;
    QLabel* textScore;
    QPushButton* button;
public:
    MyLineEdit(QWidget* parent = 0)
    {
        text = new QLineEdit(parent);
        button = new QPushButton(parent);
        textScore = new QLabel(parent);
        textScore->setStyleSheet("QLabel{color:white;font-size:18px}");
		text->setStyleSheet("QLineEdit{background-color: white;border:1px solid #999999;border-radius: 5px;}"\
        			"QLineEdit:hover{background-color: silver;border:1px solid #999999;border-radius: 5px;}"\
        			"QLineEdit:pressed{background-color: silver;border:1px solid #999999;border-radius: 5px;}");
        button->setStyleSheet("QPushButton{color:white;font-size: 14px;}");
        button->setText(tr("确定"));
        textScore->setText(tr("学分"));
    }
    void setGeometry(int x, int y, int w, int h)
    {
        textScore->setGeometry(x, y, 50, 50);
        text->setGeometry(x + 50, y, w, h);
        button->setGeometry(x + 100, y, 50, 50);
    }
    void setText(QString str)
    {
        text->setText(str);
    }
    void lower()
    {
        text->lower();
        textScore->lower();
        button->lower();
    }
    void hide()
    {
        text->hide();
		textScore->hide();
		button->hide();
    }
    void show()
    {
		text->show();
        textScore->show();
        button->show();
    }
};

class LabelWidget : public QLabel
{
    Q_OBJECT
private:
    QLabel* text;
public:
    LabelWidget(QWidget* parent = 0) : QLabel(parent)
    {
        this->setParent(parent);
        text = new QLabel(parent);
        this->setGeometry(800, 131, 400, 540);
        text->move(820, 200);
        this->setPixmap(QPixmap(tr("picture/blackboard.png")).scaled(this->size()));
        text->setFont(QFont("微软雅黑", 14, QFont::ExtraBold));
        text->setStyleSheet(tr("QLabel{background-color: transparent;color: white;}"));
        text->setAlignment(Qt::AlignTop);
    }
    void Hide()
    {
        this->hide();
        text->hide();
    }
    void Show()
    {
        this->show();
        text->show();
    }
    void setText(QString str)
    {
        text->setText(str);
        text->adjustSize();
    }
};
