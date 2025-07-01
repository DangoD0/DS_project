#include "test.h"
test::test(int flag,QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::testClass)
{
    G = new graph(100);
    semester = new layer *[8];
    star = new QLabel * [12];
    course1 = -1;
    for (int i = 0; i < 8; i++) semester[i] = new layer();
    semester[0]->setLimit(20);
    semester[1]->setLimit(10);
    semester[2]->setLimit(10);
    semester[3]->setLimit(10);
    semester[4]->setLimit(10);
    semester[5]->setLimit(10);
    semester[6]->setLimit(10);
    semester[7]->setLimit(0);
    for (int i = 0; i < 12; i++)
    {
        star[i] = new QLabel(this);
    }
    ui->setupUi(this);
    setWindowTitle(tr("课程管理系统"));
    this->setMouseTracking(true);
    //set Widget
    if (!flag || flag == 1) setFixedSize(600, 500);
    else
    {
        initGraph();
        G->sortForList();
        if (flag == 3) setFixedSize(1200, 700);
        else setFixedSize(3000, 1400);
    }
    this->flag = flag;
    move(400, 200);
    setStyleSheet(tr("QWidget{background-color: rgb(57,57,57);}"));
    {
        srand((unsigned int)time(NULL));
        mouseX = preMouseX = this->width() / 2;
        mouseY = preMouseY = this->height() / 2;
        isMouseDown = 0;
        for (int i = 0; i < NUM; i++) {
            ball[i].ball_x = rand() % this->width();
            ball[i].ball_y = rand() % this->height();
            ball[i].color = QColor(rand() % 256, rand() % 256, rand() % 256);
            ball[i].v_x = float(cos(float(i))) * (rand() % 34);
            ball[i].v_y = float(sin(float(i))) * (rand() % 34);
            ball[i].r = (rand() % 34) / 15.0;
        }

        timer = new QTimer(this);
        timer->setTimerType(Qt::PreciseTimer);
        timer->setInterval(20);
        timer->start();
        connect(timer, &QTimer::timeout, this, [=]() {
            updateBall();
            update();
            timer->start();
            });

    }
    connect(this, &test::mouseClick, this, [=]() {
        srand(time(0));
        static QPixmap* pix = new QPixmap[12];
        static int ano_idx = 0;
        for (int i = 0; i < 12; i++)
        {
            pix[i].load(tr("picture/star/star") + QString::number(i + 1) + tr(".png"));
            star[i]->setPixmap(pix[i].scaled(80, 80));
            //star[i]->raise();
        }
        int idx = (rand() + ano_idx) % 12;
        ano_idx = (ano_idx + 1) % 12;
        star[idx]->setGeometry(star_point.x() - 40, star_point.y() - 40, 80, 80);
        star[idx]->setStyleSheet("QLabel{background-color: transparent;border: 0px;}");
        star[idx]->autoFillBackground();
        QGraphicsOpacityEffect* Effect = new QGraphicsOpacityEffect(this);
        Effect->setOpacity(0);
        star[idx]->setGraphicsEffect(Effect);
        QPropertyAnimation* Animation2 = new QPropertyAnimation(Effect, "opacity");
        Animation2->setDuration(1500);
        Animation2->setStartValue(0);
        Animation2->setKeyValueAt(0.5, 1);
        Animation2->setEndValue(0);
        Animation2->setEasingCurve(QEasingCurve::OutInCubic);
        Animation2->start(QAbstractAnimation::DeleteWhenStopped);
        });
}

test::~test()
{
    delete ui;
    delete G;
}

void test::mainMenu()
{
    QLabel* bg = new QLabel(this);
    QString strStyle = ReadQssFile("form.qss");
    show_schedule = new QAnimatedButton(tr("查看课表"));
    show_course = new QAnimatedButton(tr("选课名单"));
    show_graph = new QAnimatedButton(tr("选课名单"));
    icon = new QAnimatedLabel();
    QAudioOutput* audioOutput = new QAudioOutput(this);
    //set show_schedule
    show_schedule->setParent(this);
    show_schedule->resize(100, 50);
    show_schedule->setStyleSheet(strStyle);
    show_schedule->InitAnimation(50, 300,-100,400);
    show_course->setParent(this);
    show_course->resize(100, 50);
    show_course->setStyleSheet(strStyle);
    show_course->InitAnimation(20, 360, -100, 400);
    show_graph->setParent(this);
    show_graph->resize(100, 50);
    show_graph->setStyleSheet(strStyle);
    show_graph->InitAnimation(50, 420, -100, 400);
    //set icon
    QPixmap pix;
    pix.load(tr("picture/icon.png"));
    icon->setParent(this);
    icon->resize(250, 150);
    icon->setStyleSheet("QLabel{background-color: transparent;border: 0px ;border-radius: 40px}");
    icon->setPixmap(pix.scaled(icon->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation));
    icon->setAutoFillBackground(true);
    icon->InitAnimation(500, 400);
    icon->setMouseTracking(true);
    bg->setFixedSize(600, 500);
    bg->setMouseTracking(true);
    bg->setStyleSheet("QLabel{background-color: transparent;}");
    QMovie* bg_gif = new QMovie(tr("picture/bg.gif"));
    bg_gif->setScaledSize(QSize(600, 500));
    bg->setMovie(bg_gif);
    bg_gif->start();
    bg->lower();
    /*title->setGeometry(0, 181, 600, 150);
    title->setPixmap(QPixmap(tr("picture/icon2.png")).scaled(title->size()));*/
    //set video
    player = new QMediaPlayer();
    player->setSource(QUrl::fromLocalFile("picture/Danganropa.mp4"));
    player->play();
    player->setLoops(-1);
    player->setAudioOutput(audioOutput);
    vw = new QVideoWidget(this);
    player->setVideoOutput(vw);
    vw->setGeometry(0, 0, 600, 191);
    vw->setAutoFillBackground(true);
    vw->setStyleSheet(tr("{background-color: rgb(57,57,57),color:rgb(57,57,57);}"));
    vw->lower();
    //finished
    connect(show_schedule, &QAnimatedButton::end, this, [=]() {
        delete bg;
        delete audioOutput;
        this->MainMenu_endAnimation(1); 
        });
    connect(show_course, &QAnimatedButton::end, this, [=]() {
        delete bg;
        delete audioOutput;
        this->MainMenu_endAnimation(2); 
        });
    connect(show_graph, &QAnimatedButton::end, this, [=]() {
        delete bg;
        delete audioOutput;
        this->MainMenu_endAnimation(3); 
        });
   /* connect(show_schedule, &QAnimatedButton::end, this, [=]() {
        delete vw;
        delete player;
        delete icon;
        delete show_schedule;
        emit waitS(); });*/
}

void test::waitMenu(int f)
{
    wait = new QLabel(this);
    wait->setGeometry(0,0,600,500);
    wait->setParent(this);
    QMovie *m = new QMovie();
    m->setFileName("picture/wait.gif");
    wait->setMovie(m);
    m->start();
    connect(m, &QMovie::finished, this, [=]() {
        delete wait;
        delete m;
        if (f == 0) emit mainS();
        else if(f == 2) emit scheduleS();
		else if(f == 3) emit courseS();
        else if(f == 4) emit graphS();
        });
    
}

void test::scheduleMenu()
{
    static bool addFlag = true;
    static MyTableWidget** schedule = new MyTableWidget*[8];
    QAnimatedButton** chapter = new QAnimatedButton * [8];
    QAnimatedButton* back = new QAnimatedButton();
//set Pixmap
    QPixmap* pix = new QPixmap[8];
    pix[0].load(tr("picture/s1_L.png"));
    pix[1].load(tr("picture/s2_L.png"));
    pix[2].load(tr("picture/s3_L.png"));
    pix[3].load(tr("picture/s4_L.png"));
    pix[4].load(tr("picture/s5_L.png"));
    pix[5].load(tr("picture/s6_L.png"));
    pix[6].load(tr("picture/s7_L.png"));
    pix[7].load(tr("picture/s8_L.png"));
//set chapter button
    for (int i = 0; i < 8; i++)
    {
        chapter[i] = new QAnimatedButton();
        chapter[i]->setParent(this);
        chapter[i]->resize(100, 100);
        chapter[i]->setStyleSheet("QPushButton{background-color: transparent;border: 0px ;border-radius: 50px}"\
            "QPushButton:hover{background-color: transparent;border: 2px solid black;border-radius: 50px}"\
            "QPushButton:pressed{background-color: transparent;border: 0px;border-radius: 50px}");
        chapter[i]->setIcon(pix[i].scaled(chapter[i]->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));

        chapter[i]->setIconSize(chapter[i]->size());
        chapter[i]->setAutoFillBackground(true);
        chapter[i]->InitAnimation(5 + 150 * i, 5 + 50 * ((i + 1) % 2), 100 + 125 * i, -100);
    }
//set back button
    back->setParent(this);
    back->resize(80, 80);
    back->setStyleSheet("QPushButton{background-color: transparent;border: 0px ;border-radius: 40px}"\
        "QPushButton:hover{background-color: transparent;border: 0px;border-radius: 40px}"\
        "QPushButton:pressed{background-color: transparent;border: 0px;border-radius: 40px}");

    back->setIcon(QIcon(tr("picture/back.png")));
    back->setIconSize(back->size());
    back->setAutoFillBackground(true);
    back->InitAnimation(1100, 600, 1300, 600);
    if (addFlag)
    {
        addFlag = false;
        for (int k = 0; k < 8; k++)
        {
            schedule[k] = new MyTableWidget(12, 9, this);
            schedule[k]->hide();
        }
    }
    if (modifySchedule)
    {
        modifySchedule = false;
        for (int k = 0; k < 8; k++)
        {
            schedule[k]->clear();
            schedule[k]->clearSpans();
            schedule[k]->setHorizontalHeaderLabels(QStringList() << tr("时间段") << tr("节次") << tr("星期一") << tr("星期二") << tr("星期三") << tr("星期四") << tr("星期五") << tr("星期六") << tr("星期日"));
            for (int i = 0; i < 3; i++)
            {
                QStringList list;
                list << tr("上午") << tr("下午") << tr("晚上");
                QTableWidgetItem* item = new QTableWidgetItem(list.at(i));
                //item->setForeground(QBrush(QColor(102, 102, 102)));
                item->setFont(QFont("Times", 12, QFont::Bold));
                item->setTextAlignment(Qt::AlignCenter);
                schedule[k]->setSpan(i * 4, 0, 4, 1);
                schedule[k]->setItem(i * 4, 0, item);
            }
            for (int i = 0; i < 12; i++)
            {
                QTableWidgetItem* item = new QTableWidgetItem(QString::number(i + 1));
                //item->setForeground(QBrush(QColor(102, 102, 102)));
                item->setFont(QFont("Times", 12, QFont::ExtraBold, true));
                item->setTextAlignment(Qt::AlignCenter);
                schedule[k]->setItem(i, 1, item);
            }
            for (auto p : semester[k]->a)
            {
                QTableWidgetItem* item = new QTableWidgetItem();
                schedule[k]->setSpan(p->start_class - 1, p->day + 1, p->end_class - p->start_class + 1, 1);
                item->setFont(QFont("微软雅黑", 14));
                item->setText(QString::fromLocal8Bit(p->get_name()));
                item->setTextAlignment(Qt::AlignCenter);
                schedule[k]->setItem(p->start_class - 1, p->day + 1, item);
            }
        }
    }
    schedule[preScheduleidx]->show();
    for (int i = 0; i < 8; i++)
        connect(chapter[i], &QAnimatedButton::clicked, schedule[i], [=]() {
        schedule[preScheduleidx]->hide();
        schedule[i]->show();
        preScheduleidx = i;
            });
    connect(back, &QAnimatedButton::end, this, [=]() {this->scheduleMenu_endAnimation(schedule, chapter, back); });
    
}

void test::selectCourse(MyListWidget** list, int k, Line* l)
{
    if (l->cb->checkState() == 0)
    {
        int d = l->c.day;
        int s = l->c.start_class;
        int e = l->c.end_class;
        modifySchedule = true;
        for (int j = s; j <= e; j++)
        {
            semester[k]->occupy[d][j] = false;
            vector<course*>* a = &semester[k]->a;
            a->erase(remove(a->begin(), a->end(), (&l->c)), a->end());
        }
        semester[k]->credit -= l->c.get_score();
    }
    else if (l->cb->checkState() == 2)
    {
        int d = l->c.day;
        int s = l->c.start_class;
        int e = l->c.end_class;
        bool flag = true;
        for (int j = s; j <= e; j++)
        {
            if (semester[k]->occupy[d][j]) {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            modifySchedule = true;
            for (int j = s; j <= e; j++)
            {
                semester[k]->occupy[d][j] = true;
            }
            semester[k]->a.push_back(&(l->c));
            semester[k]->credit += l->c.get_score();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet(tr("QMessageBox{background-color: white;color:black;}"));
            //msgBox.information(this, tr("提示"), tr("该课程与已选课程冲突！"));
            msgBox.setText(tr("课程") + QString::fromLocal8Bit(l->c.get_name()) + tr("与已选课程冲突！"));
            msgBox.setInformativeText(tr("是否选择该课程？"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Yes)
            {
                course q;
                modifySchedule = true;
                for (auto p : semester[k]->a)
                {
                    if (p->start_class == l->c.start_class && p->end_class == l->c.end_class && p->day == l->c.day)
                    {
                        q = *p;
                        semester[k]->a.erase(remove(semester[k]->a.begin(), semester[k]->a.end(), p), semester[k]->a.end());
                        semester[k]->a.push_back(&l->c);
                        break;
                    }
                }
                for (int j = 0; j < list[k]->count(); j++)
                {
                    if (w[k][j].c == q) w[k][j].cb->setCheckState(Qt::Unchecked);
                }
            }
            else l->cb->setCheckState(Qt::Unchecked);
        }
    }
}

void test::courseMenu()
{
    static MyListWidget** list = new MyListWidget*[8];
    static QListWidgetItem** line = new QListWidgetItem*[8];
    static QListWidgetItem** head = new QListWidgetItem*[8];
    static LabelWidget* blackboard = new LabelWidget(this);
    static bool addFlag = true;
    static QString str;
    static QLabel * isCreditFull = new QLabel(this);
    static MyLineEdit* editScore = new MyLineEdit(this);
    QAnimatedButton** chapter = new QAnimatedButton*[8];
    QAnimatedButton* back = new QAnimatedButton();
    QAnimatedButton* right = new QAnimatedButton();
    QAnimatedButton* left = new QAnimatedButton();
    editScore->setGeometry(150, 120, 50, 50);
    editScore->setText(0);
    editScore->lower();
    editScore->show();
    isCreditFull->setGeometry(825, 600, 300, 100);
    isCreditFull->lower();
    isCreditFull->setStyleSheet("QLabel{background-color: transparent;font-size: 16px;color: white;}");
    if (semester[preListidx]->compareCredit())
    {
        str = "本学期学分修满";
    }
    else
    {
        str = tr("本学期学分未修满，还差") + QString::number(semester[preListidx]->restCredit());
    }
    isCreditFull->setText(str);
    isCreditFull->show();
//set Pixmap
    QPixmap* pix = new QPixmap[8];
    pix[0].load(tr("picture/s1.png"));
    pix[1].load(tr("picture/s2.png"));
    pix[2].load(tr("picture/s3.png"));
    pix[3].load(tr("picture/s4.png"));
    pix[4].load(tr("picture/s5.png"));
    pix[5].load(tr("picture/s6.png"));
    pix[6].load(tr("picture/s7.png"));
    pix[7].load(tr("picture/s8.png"));
//set chapter button
    for (int i = 0; i < 8; i++)
    {
        chapter[i] = new QAnimatedButton();
        chapter[i]->setParent(this);
        chapter[i]->resize(100, 100);
        chapter[i]->setStyleSheet("QPushButton{background-color: transparent;border: 0px ;border-radius: 50px}"\
			"QPushButton:hover{background-color: transparent;border: 2px solid black;border-radius: 50px}"\
			"QPushButton:pressed{background-color: transparent;border: 0px;border-radius: 50px}");
       chapter[i]->setIcon(pix[i].scaled(chapter[i]->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));
        
        chapter[i]->setIconSize(chapter[i]->size());
        chapter[i]->setAutoFillBackground(true);
        chapter[i]->InitAnimation(5 + 150 * i, 5 + 50 * ((i + 1) % 2), 100 + 125 * i, -100);
    }
//set back button
    //back->setStyleSheet("StyleSheet.qss");
    back->setParent(this);
    back->resize(80, 80);
    back->setStyleSheet("QPushButton{background-color: transparent;border: 0px ;border-radius: 40px}"\
        "QPushButton:hover{background-color: transparent;border: 0px;border-radius: 40px}"\
        "QPushButton:pressed{background-color: transparent;border: 0px;border-radius: 40px}");

    back->setIcon(QIcon(tr("picture/back.png")));
    back->setIconSize(back->size());
    back->setAutoFillBackground(true);
    back->InitAnimation(1100, 600, 1300, 550);
    right->setParent(this);
    right->resize(80, 80);
    right->setStyleSheet("QPushButton{background-color: transparent;border: 0px ;border-radius: 40px}"\
        "QPushButton:hover{background-color: transparent;border: 0px;border-radius: 40px}"\
        "QPushButton:pressed{background-color: transparent;border: 0px;border-radius: 40px}");

    right->setIcon(QIcon(tr("picture/right.png")));
    right->setIconSize(right->size());
    right->setAutoFillBackground(true);
    right->InitAnimation(1100, 500, 1300, 550);
    left->setParent(this);
    left->resize(80, 80);
    left->setStyleSheet("QPushButton{background-color: transparent;border: 0px ;border-radius: 40px}"\
        "QPushButton:hover{background-color: transparent;border: 0px;border-radius: 40px}"\
        "QPushButton:pressed{background-color: transparent;border: 0px;border-radius: 40px}");

    left->setIcon(QIcon(tr("picture/left.png")));
    left->setIconSize(left->size());
    left->setAutoFillBackground(true);
    left->InitAnimation(1100, 400, 1300, 500);
//set WidgetList
    if (addFlag)
    {
        addFlag = false;
        w = new Line * [8];
        blackboard->Hide();
        for (int i = 0; i < 8; i++)
        {
            list[i] = new MyListWidget(this);
        }
        for (int k = 0; k < 8; k++)
        {
            int i = 0;
            head[k] = new QListWidgetItem();
            head[k]->setText(tr("是否选中               "\
                "编号                    "\
                "名称                      "\
                "学分                     "\
                "时间                     "\
                "是否有先修"));
            head[k]->setFont(QFont("Times", 12, QFont::ExtraBold));
            head[k]->setForeground(QBrush(QColor(45, 80, 240)));
            w[k] = new Line[100];
            line[k] = new QListWidgetItem[100];
            list[k]->addItem(head[k]);
            for (auto p : G->l[k].a)
            {
                line[k][i].setSizeHint(QSize(40, 40));
                w[k][i].setParent(list[k]);
                w[k][i].c = *p;
                line[k][i].setText(QString::fromLocal8Bit(p->get_name()));
                line[k][i].setForeground(QBrush(Qt::transparent));
                w[k][i].setData(QString::fromLocal8Bit(p->get_name()), p->get_score(), p->get_index(), p->day, p->start_class, p->end_class, p->preCourse.size());
                list[k]->addItem(&line[k][i]);
                list[k]->setItemWidget(&line[k][i], &w[k][i]);
                i++;
            }
            list[k]->hide();
            for (int i = 0; i < 100; i++)
            {
                connect(w[k][i].cb, &QCheckBox::clicked, this, [=]()
                    {
                        selectCourse(list, k, &w[k][i]);
                        if (semester[preListidx]->compareCredit())
                        {
                            str = "本学期学分修满";
                        }
                        else
                        {
                            str = tr("本学期学分未修满，还差") + QString::number(semester[preListidx]->restCredit());
                        }
                        isCreditFull->setText(str);
                    });
                connect(w[k][i].primary, &ListLabel::hoverIn, this,[=]() 
                    {
                        QString str;
                        str.append(QString::fromLocal8Bit(w[k][i].c.get_name()) + tr("先修课程：\n"));
                        for (auto p : w[k][i].c.preCourse)
                        {
                            str.append(QString::fromLocal8Bit(p->get_name()) + tr("\n"));
                        }
                        blackboard->setText(str);
                        blackboard->Show();
                    });
                connect(w[k][i].primary, &ListLabel::hoverOut, this, [=]()
                    {
                        blackboard->Hide();
                    });
            }
        }
    }
    for(int i = 0;i < 8;i ++)
        connect(chapter[i], &QAnimatedButton::clicked, list[i], [=]() {
                list[preListidx]->hide(); 
                list[i]->show(); 
                preListidx = i; 
                if (semester[preListidx]->compareCredit())
                {
                    str = "本学期学分修满";
                }
                else
                {
                    str = tr("本学期学分未修满，还差") + QString::number(semester[preListidx]->restCredit());
                }
                isCreditFull->setText(str);
                editScore->setText(0);
				});
    connect(editScore->button, &QPushButton::clicked, this, [=]()
        {
            double tmp_score = editScore->text->text().toDouble();
            if (tmp_score < 0 || tmp_score > 50)
            {
                QMessageBox msgBox;
                msgBox.setStyleSheet(tr("QMessageBox{background-color: white;color:black;}"));
                msgBox.setText(tr("学分值无效"));
                msgBox.exec();
            }
            else
            {
                semester[preListidx]->setLimit(tmp_score);
                if (semester[preListidx]->compareCredit())
                {
                    str = "本学期学分修满";
                }
                else
                {
                    str = tr("本学期学分未修满，还差") + QString::number(semester[preListidx]->restCredit());
                }
                isCreditFull->setText(str);
            }
        });
    connect(right, &QAnimatedButton::end, this, [=]() {
        if (preListidx == 7)
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet(tr("QMessageBox{background-color: white;color:black;}"));
            msgBox.setText(tr("选课结束"));
            msgBox.setInformativeText(tr("是否查看课表？"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Yes)
            {
                isCreditFull->hide();
                editScore->hide();
                this->courseMenu_endAnimation(list, chapter, back, right, left, 1);
            }
        }
        else
        {
            stack<pair<int,int>> buffer1;
            stack<pair<int,int>> buffer2;
            for (int i = 0; i < list[preListidx]->count() - 1; i++)
            {
                if (w[preListidx][i].cb->checkState() == 0 && w[preListidx][i].isHidden() == false)
                {
                    if (w[preListidx][i].c.get_max_depth() + preListidx < 7)
                    {
                        buffer1.push({ preListidx,i });
                        buffer2.push({ preListidx,i });
                        w[preListidx][i].hide();
                        line[preListidx][i].setHidden(true);
                    }
                    else
                    {
                        QMessageBox msgBox;
                        msgBox.setStyleSheet(tr("QMessageBox{background-color: white;color:black;}"));
                        msgBox.setText(tr("课程") + QString::fromLocal8Bit(w[preListidx][i].c.get_name()) + tr("超出学期范围！"));
                        msgBox.setInformativeText(tr("是否继续？"));
                        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                        msgBox.setDefaultButton(QMessageBox::No);
                        int ret = msgBox.exec();
                        if (ret == QMessageBox::Yes) continue;
                        else break;
                    }
                }
            }
            while (buffer1.size())
            {
                auto tmp = buffer1.top();
                buffer1.pop();
                int k = tmp.first,idx = tmp.second;
                vector<course*> check = G->forReach(w[k][idx].c.get_index());
                for(auto p : check)
                {
                    int j = k + 1;
                    int count = list[j]->count() - 1;
                    for (int i = 0; i < count; i++)
                    {
                        if (w[j][i].c.get_index() == p->get_index())
                        {
                            if (p->get_max_depth() + j < 7)
                            {
                                buffer1.push({ j,i });
                                buffer2.push({ j,i });
                                w[j][i].hide();
                                line[j][i].setHidden(true);
                            }
                            else
                            {
                                QMessageBox msgBox;
                                msgBox.setStyleSheet(tr("QMessageBox{background-color: white;color:black;}"));
                                msgBox.setText(tr("课程") + QString::fromLocal8Bit(p->get_name()) + tr("超出学期范围！"));
                                msgBox.setInformativeText(tr("是否继续？"));
                                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                                msgBox.setDefaultButton(QMessageBox::No);
                                int ret = msgBox.exec();
                                if (ret == QMessageBox::Yes) continue;
                                else break;
                            }
                        }
                    }
                }
            } 
            while (buffer2.size())
            {
                auto p = buffer2.top();
                buffer2.pop();
                int k = p.first, idx = p.second;
                int j = k + 1,count = list[j]->count() - 1;
                if (w[k][idx].addFlag)
                {
                    w[k][idx].addFlag = false;
                    course c = w[k][idx].c;
                    line[j][count].setSizeHint(QSize(40, 40));
                    w[j][count].setParent(list[j]);
                    w[j][count].c = c;
                    line[j][count].setText(QString::fromLocal8Bit(c.get_name()));
                    line[j][count].setForeground(QBrush(Qt::transparent));
                    w[j][count].cb->setCheckState(w[k][idx].cb->checkState());
                    w[j][count].setData(QString::fromLocal8Bit(c.get_name()), c.get_score(), c.get_index(), c.day, c.start_class, c.end_class, true);
                    list[j]->addItem(&line[j][count]);
                    list[j]->setItemWidget(&line[j][count], &w[j][count]);
                    if(w[j][count].cb->checkState() == 2)
                        selectCourse(list,j,&w[j][count]);
                    if (w[k][idx].cb->checkState() == 2)
                    {
                        w[k][idx].cb->setCheckState(Qt::Unchecked);
                        emit w[k][idx].cb->clicked();
                    }
                }
                else
                {
                    QListWidgetItem* t1 = list[j]->findItems(QString::fromLocal8Bit(w[k][idx].c.get_name()), Qt::MatchExactly).front();
                    Line* t2 = (Line*)list[j]->itemWidget(t1);
                    t2->cb->setCheckState(w[k][idx].cb->checkState());
                    t1->setHidden(false);
                    t2->setHidden(false);
                    if (t2->cb->checkState() == 2)
                        selectCourse(list, j, t2);
                    if (w[k][idx].cb->checkState() == 2)
                    {
                        w[k][idx].cb->setCheckState(Qt::Unchecked);
                        emit w[k][idx].cb->clicked();
                    }
                }
            }
        }
        });
        connect(left, &QAnimatedButton::end, this, [=]() {
            if (preListidx == 0)
        {
            QMessageBox msgBox;
            msgBox.setStyleSheet(tr("QMessageBox{background-color: white;color:black;}"));
            msgBox.setText(tr("无法继续重溯"));
            msgBox.exec();
        }
        else
        {
            stack<pair<int,int>> buffer1;
            stack<pair<int,int>> buffer2;
            for (int i = 0; i < list[preListidx]->count() - 1; i++)
            {
                if (w[preListidx][i].cb->checkState() == 0 && w[preListidx][i].isHidden() == false)
                {
                    if (w[preListidx][i].c.get_min_depth() < preListidx)
                    {
                        buffer1.push({ preListidx,i });
                        buffer2.push({ preListidx,i });
                        w[preListidx][i].hide();
                        line[preListidx][i].setHidden(true);
                    }
                    else
                    {
                        QMessageBox msgBox;
                        msgBox.setStyleSheet(tr("QMessageBox{background-color: white;color:black;}"));
                        msgBox.setText(tr("课程") + QString::fromLocal8Bit(w[preListidx][i].c.get_name()) + tr("无法继续重溯！"));
                        msgBox.setInformativeText(tr("是否继续？"));
                        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                        msgBox.setDefaultButton(QMessageBox::No);
                        int ret = msgBox.exec();
                        if (ret == QMessageBox::Yes) continue;
                        else break;
                    }
                }
            }
            while (buffer1.size())
            {
                auto tmp = buffer1.top();
                buffer1.pop();
                int k = tmp.first,idx = tmp.second;
                vector<course*> check = w[k][idx].c.preCourse;
                for(auto p : check)
                {
                    int j = k - 1;
                    int count = list[j]->count() - 1;
                    for (int i = 0; i < count; i++)
                    {
                        if (w[j][i].c.get_index() == p->get_index())
                        {
                            if (p->get_min_depth() < j)
                            {
                                buffer1.push({ j,i });
                                buffer2.push({ j,i });
                                w[j][i].hide();
                                line[j][i].setHidden(true);
                            }
                            else
                            {
                                QMessageBox msgBox;
                                msgBox.setStyleSheet(tr("QMessageBox{background-color: white;color:black;}"));
                                msgBox.setText(tr("课程") + QString::fromLocal8Bit(p->get_name()) + tr("无法继续重溯！"));
                                msgBox.setInformativeText(tr("是否继续？"));
                                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                                msgBox.setDefaultButton(QMessageBox::No);
                                int ret = msgBox.exec();
                                if (ret == QMessageBox::Yes) continue;
                                else break;
                            }
                        }
                    }
                }
            } 
            while (buffer2.size())
            {
                auto p = buffer2.top();
                buffer2.pop();
                int k = p.first, idx = p.second;
                int j = k - 1;
                QListWidgetItem* t1 = list[j]->findItems(QString::fromLocal8Bit(w[k][idx].c.get_name()), Qt::MatchExactly).front();
                Line* t2 = (Line*)list[j]->itemWidget(t1);
                t2->cb->setCheckState(w[k][idx].cb->checkState());
                t1->setHidden(false);
                t2->setHidden(false);
                if (t2->cb->checkState() == 2)
                    selectCourse(list, j, t2);
                if (w[k][idx].cb->checkState() == 2)
                {
                    w[k][idx].cb->setCheckState(Qt::Unchecked);
                    emit w[k][idx].cb->clicked();
                }
            }
        }
            });
    connect(back, &QAnimatedButton::end, this, [=]() {
            isCreditFull->hide();
            editScore->hide();
            this->courseMenu_endAnimation(list,chapter,back,right,left); 
        });
    list[preListidx]->show();
}

void test::graphMenu()
{
    static QAnimatedNode** graph_node = new QAnimatedNode * [100];
    static bool addFlag = true;
    bg1 = new QLabel(this);
    bg2 = new QLabel(this);
    bg1->setFixedSize(3000, 1400);
    bg1->setStyleSheet("QLabel{background-color: transparent;}");
    bg1->setPixmap(QPixmap(tr("picture/graph_line.png")).scaled(bg1->size()));
    bg2->setFixedSize(3000, 1400);
    bg2->setStyleSheet("QLabel{background-color: transparent;}");
    bg1->lower();
    bg2->lower();
    bg1->setMouseTracking(true);
    bg2->setMouseTracking(true);
    if (!addFlag)
    {
        QAnimatedButton* back = new QAnimatedButton();
        back->setParent(this);
        back->resize(80, 80);
        back->setStyleSheet("QPushButton{background-color: transparent;border: 0px ;border-radius: 40px}"\
            "QPushButton:hover{background-color: transparent;border: 0px;border-radius: 40px}"\
            "QPushButton:pressed{background-color: transparent;border: 0px;border-radius: 40px}");

        back->setIcon(QIcon(tr("picture/back.png")));
        back->setIconSize(back->size());
        back->setAutoFillBackground(true);
        back->InitAnimation(20, 20, -100, -100);
        connect(back, &QAnimatedButton::end, this, [=]() 
            { 
                delete back; 
                delete bg1;
                delete bg2;
                cTc.clear();
                emit waitTM(); 
            });
    }
    if (addFlag)
    {
        addFlag = false;
        for (int i = 0; i < 100; i++)
        {
            graph_node[i] = new QAnimatedNode(QString::fromLocal8Bit(G->getCourse(i).get_name()),this);
            if (graph_node[i]->text() == "none") graph_node[i]->hide();
            connect(graph_node[i], &QPushButton::clicked, this, [=]()
                {
                    cTc.clear();
                    if (course1 == -1)
                    {
                        course1 = i;
                        QPoint start = QPoint(graph_node[course1]->x() + graph_node[course1]->width() / 2, graph_node[course1]->y() + 50);
                        for (auto p : G->forReach(course1))
                        {
                            QPoint end = QPoint(graph_node[p->get_index()]->x() + graph_node[p->get_index()]->width() / 2, graph_node[p->get_index()]->y());
                            cTc.push_back({ start,end });
                        }
                    }
                    else if (course1 != i)
                    {
                        QPoint start = QPoint(graph_node[course1]->x() + graph_node[course1]->width() / 2, graph_node[course1]->y() + 50);
                        vector<course*> check = G->forReach(course1);
                        if (count(check.begin(), check.end(), G->getCourse_Pointer(i)))
                        {
                            QPoint end = QPoint(graph_node[i]->x() + graph_node[i]->width() / 2, graph_node[i]->y());
                            cTc.push_back({ start,end });
                        }
                        course1 = -1;
                    }
                    else course1 = -1;
                    paintGraph(1);
                });
        }
        for (int i = 0; i < 8; i++)
        {
            layer tmp;
            tmp = G->sortToMiddle(G->l[i]);
            int j = 0;
            for (auto p : tmp.a)
            {
                graph_node[p->get_index()]->move(100 + 100 * j, 100 + 200 * i);
                j++;
            }
        }
        for (int k = 0; k < 8; k++)
        {
            for (auto p : G->l[k].a)
            {
                vector<course*> target = G->forReach(p->get_index());
                QPoint start_point = QPoint(graph_node[p->get_index()]->x() + graph_node[p->get_index()]->width()/2, graph_node[p->get_index()]->y() + 50);
                for (auto q : target)
                {
                    QPoint end_point = QPoint(graph_node[q->get_index()]->x() + graph_node[q->get_index()]->width() / 2, graph_node[q->get_index()]->y());
                    edge_line.push_back({ start_point,end_point });
                }
            }
        }
    }
}

void test::initGraph()
{
    ifstream ifs;
    ifs.open("graph/course.txt", ios::in);
    if (!ifs.is_open())
    {
        qDebug() << "error opening course.txt!";
        puts("");
    }
    string buf;
    while (getline(ifs, buf))
    {
        stringstream word(buf);
        course tmp1, tmp2;
        string name;
        double score;
        int day;
        int start_class;
        int end_class;
        int index;
        word >> name, word >> score, word >> day >> start_class, word >> end_class, word >> index;
        tmp1 = course(name, score, start_class, end_class, index, day);
        G->addCourse(tmp1);
        G->getCourse(index).print();
    }
    ifs.close();
    ifs.open("graph/edge.txt", ios::in);
    if (!ifs.is_open())
    {
        qDebug() << "error opening edge.txt!";
        puts("");
    }
    while (getline(ifs, buf))
    {
        stringstream word(buf);
        int index1, index2;
        word >> index1 >> index2;
        G->setEdge(index1, index2);
    }
    ifs.close();
}

QString test::ReadQssFile(const QString& fileName)
{
	QFile file("form.qss");
    if (file.open(QFile::ReadOnly))
    {
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		return styleSheet;
	}
    else
    {
        QMessageBox::information(this, "tip", "cannot find qss file");
    }
}

void test::MainMenu_endAnimation(int f)
{
    player->pause();
    QPropertyAnimation *ani_vw = new QPropertyAnimation(vw, "pos");
    ani_vw->setDuration(1000);
    ani_vw->setStartValue(QPoint(0, 0));
    ani_vw->setKeyValueAt(0.5, QPoint(0, 50));
    ani_vw->setEndValue(QPoint(0, -191));
    ani_vw->setEasingCurve(QEasingCurve::OutInQuad);

    QPropertyAnimation *ani_icon = new QPropertyAnimation(icon, "pos");
    ani_icon->setDuration(1000);
    ani_icon->setStartValue(QPoint(500, 400));
    ani_icon->setKeyValueAt(0.5,QPoint(430, 360));
    ani_icon->setEndValue(QPoint(600, 500));
    ani_icon->setEasingCurve(QEasingCurve::OutInQuad);

    QPropertyAnimation **ani_button = new QPropertyAnimation*[2];
    if (f == 1)
    {
        ani_button[0] = new QPropertyAnimation(show_course, "pos");
        ani_button[1] = new QPropertyAnimation(show_graph, "pos");
        delete show_schedule;
    }
    else if (f == 2)
    {
        ani_button[0] = new QPropertyAnimation(show_schedule, "pos");
        ani_button[1] = new QPropertyAnimation(show_graph, "pos");
        delete show_course;
    }
    else if (f == 3)
    {
        ani_button[0] = new QPropertyAnimation(show_schedule, "pos");
        ani_button[1] = new QPropertyAnimation(show_course, "pos");
        delete show_graph;
    }
    
    ani_button[0]->setDuration(500);
    ani_button[1]->setDuration(500);
    if (f == 1)
    {
        ani_button[0]->setStartValue(show_course->pos());
        ani_button[1]->setStartValue(show_graph->pos());
    }
    else if (f == 2)
    {
        ani_button[0]->setStartValue(show_schedule->pos());
        ani_button[1]->setStartValue(show_graph->pos());
    }
    else if (f == 3)
    {
        ani_button[0]->setStartValue(show_schedule->pos());
        ani_button[1]->setStartValue(show_course->pos());
    }
    ani_button[0]->setEndValue(QPoint(-100, 400));
    ani_button[1]->setEndValue(QPoint(-100, 400));
    ani_button[0]->setEasingCurve(QEasingCurve::OutCubic);
    ani_button[1]->setEasingCurve(QEasingCurve::OutCubic);

    QParallelAnimationGroup* ani = new QParallelAnimationGroup(this);
    ani->addAnimation(ani_vw);
    ani->addAnimation(ani_icon);
    ani->addAnimation(ani_button[0]);
    ani->addAnimation(ani_button[1]);
    ani->start(QAbstractAnimation::DeleteWhenStopped);
    connect(ani, &QPropertyAnimation::finished, this, [=]() {
        player->stop();
        delete vw;
        delete player;
        delete icon;
        if (f == 1) emit waitTS();
        else if (f == 2) emit waitTC();
        else if (f == 3) emit waitTG();
        });        
}

void test::courseMenu_endAnimation(MyListWidget** list, QAnimatedButton** chapter, QAnimatedButton* back, QAnimatedButton* right, QAnimatedButton* left, int flag)
{
    delete back;
    list[preListidx]->hide();
    QPropertyAnimation** ani = new QPropertyAnimation*[8];
    QParallelAnimationGroup* ani_group = new QParallelAnimationGroup(this);
    for (int i = 0; i < 8; i++)
    {
        ani[i] = new QPropertyAnimation(chapter[i],"pos");
        ani[i]->setDuration(1000);
        ani[i]->setStartValue(chapter[i]->pos());
        ani[i]->setEndValue(QPoint(1100,chapter[i]->y()));
        ani[i]->setEasingCurve(QEasingCurve::InCubic);
        ani_group->addAnimation(ani[i]);
    }
    QPropertyAnimation* b1 = new QPropertyAnimation(right,"pos");
    b1->setDuration(500);
    b1->setStartValue(right->pos());
    b1->setEndValue(QPoint(1300, 550));
    b1->setEasingCurve(QEasingCurve::InCubic);
    QPropertyAnimation* b2 = new QPropertyAnimation(left, "pos");
    b2->setDuration(500);
    b2->setStartValue(left->pos());
    b2->setEndValue(QPoint(1300, 500));
    b2->setEasingCurve(QEasingCurve::InCubic);
    ani_group->addAnimation(b1);
    ani_group->addAnimation(b2);
    ani_group->start(QAbstractAnimation::DeleteWhenStopped);
    connect(ani_group, &QPropertyAnimation::finished, this, [=]() {
        for (int i = 0; i < 8; i++) delete[] chapter[i];
        delete[] chapter;
        delete right;
        delete left;
        if(flag == 0) emit waitTM();
		else if(flag == 1) emit waitTS();
        });
}

void test::scheduleMenu_endAnimation(MyTableWidget** schedule, QAnimatedButton** chapter, QAnimatedButton* back)
{
    delete back;
    schedule[preScheduleidx]->hide();
    QPropertyAnimation** ani = new QPropertyAnimation * [8];
    QParallelAnimationGroup* ani_group = new QParallelAnimationGroup(this);
    for (int i = 0; i < 8; i++)
    {
        ani[i] = new QPropertyAnimation(chapter[i], "pos");
        ani[i]->setDuration(1000);
        ani[i]->setStartValue(chapter[i]->pos());
        ani[i]->setEndValue(QPoint(1100, chapter[i]->y()));
        ani[i]->setEasingCurve(QEasingCurve::InCubic);
        ani_group->addAnimation(ani[i]);
    }
    ani_group->start(QAbstractAnimation::DeleteWhenStopped);
    connect(ani_group, &QPropertyAnimation::finished, this, [=]() {
        for (int i = 0; i < 8; i++) delete[] chapter[i];
        delete[] chapter;
        emit waitTM();
        });
}
void test::showEvent(QShowEvent* e)
{

#if QT_VERSION >= 0x050000
    this->setAttribute(Qt::WA_Mapped);
#endif
    QWidget::showEvent(e);
}
void test::mousePressEvent(QMouseEvent* event)
{
    star_point = event->pos();
    isMouseDown = 1;
    if (star[0] == NULL) return;
    if (this->flag == 0)
    {
        if (event->pos().x() <= 500 || event->pos().y() <= 400)
            emit mouseClick();
    }
    else emit mouseClick();
}
void test::mouseReleaseEvent(QMouseEvent* event)
{
    isMouseDown = 0;
}
void test::mouseMoveEvent(QMouseEvent* event)
{
    mouseX = event->pos().x();
    mouseY = event->pos().y();
}
void test::updateBall()
{
    float aDis = this->width() * 0.86;
    float blowDis = this->height() * 0.2;
    float disturbDis = this->width() * 0.125;
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
        ball[i].ball_x = x > this->width() ? (ball[i].v_x = -ball[i].v_x, this->width()) : x < 0 ? (ball[i].v_x = -ball[i].v_x, 0) : x;
        ball[i].ball_y = y > this->height() ? (ball[i].v_y = -ball[i].v_y, this->height()) : y < 0 ? (ball[i].v_y = -ball[i].v_y, 0) : y;
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

void test::paintGraph(int flag)
{
    QPixmap* pix = new QPixmap(3000, 1400);
    pix->fill(Qt::transparent);
    QPainter* painter = new QPainter();
    if (flag == 0)
    {
        painter->begin(pix);
        painter->setPen(QPen(Qt::white, 2));
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        for (auto p : edge_line)
        {
            painter->drawLine(p.first, p.second);
        }
        painter->end();
        pix->save("picture/graph_line.png");
    }
    else if (flag == 1)
    {
        srand(time(0));
        int color;
        painter->begin(pix);
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter->setOpacity(1);
        for (auto p : cTc)
        {
            color = rand() % 19;
            painter->setPen(QPen(Qt::GlobalColor(color), 8));
            painter->drawLine(p.first, p.second);
        }
        painter->end();
        bg2->setPixmap(pix->scaled(bg2->size()));
    }
    
}
void test::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    {
        for (int i = 0; i < NUM; i++) {
            painter.setPen(ball[i].color);
            painter.setBrush(Qt::white);
            painter.drawEllipse(QPointF(ball[i].ball_x, ball[i].ball_y), ball[i].r, ball[i].r);
        }
    }
    /*
    if (flag == 2)
    {
        static QPixmap* pix = new QPixmap(3000,1400);
        pix->fill(Qt::transparent);
        QPainter* painter = new QPainter();
        srand(time(0));
        int color;
        if (!cTc.empty())
        {
            for (auto p : cTc)
            {
                color = rand() % 19;
                painter->begin(pix);
                painter->setPen(QPen(Qt::GlobalColor(color), 8));
                painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
                painter->drawLine(p.first, p.second);
                painter->end();
            }
            cTc.clear();
            bg2->setPixmap(pix->scaled(bg2->size()));
        }
        painter->begin(this);
        painter->drawPixmap(0, 0, *pix);
        painter->end();
    }
    */
}
