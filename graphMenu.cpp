#include "graphMenu.h"

GraphMenu::GraphMenu(QWidget* parent) :QGraphicsScene(parent)
{
	G = new graph(100);
    graph_node = new QAnimatedNode * [100];
	initGraph();
	G->sortForList();
    connect(this, &GraphMenu::mouseClick, this, [=]() {
        srand(time(0));
        static QPixmap* pix = new QPixmap[12];
        static int ano_idx = 0;
        for (int i = 0; i < 12; i++)
        {
            pix[i].load(tr("picture/star/sta") + QString::number(i + 1) + tr(".png"));
            star[i]->setPixmap(pix[i].scaled(80, 80));
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
    for (int i = 0; i < 12; i++)
    {
        star[i] = new QLabel();
        addWidget(star[i]);
    }
    for (int i = 0; i < 100; i++)
    {
        graph_node[i] = new QAnimatedNode(QString::fromLocal8Bit(G->getCourse(i).get_name()));
        if (graph_node[i]->text() == "none") graph_node[i]->hide();
        
        
    }
    for (int i = 0; i < 8; i++)
    {
        layer tmp;
        tmp = G->sortToMiddle(G->l[i]);
        int j = 0;
        for (auto p : tmp.a)
        {
            graph_node[p->get_index()]->move(100 + 100 * j, 20 + 100 * i);
            j++;
        }
    }
    for (int k = 0; k < 8; k++)
    {
        for (auto p : G->l[k].a)
        {
            vector<course> target = G->forReach(p->get_index());
            QPoint start_point = QPoint(graph_node[p->get_index()]->x() + 25, graph_node[p->get_index()]->y() + 50);
            for (auto q : target)
            {
                QPoint end_point = QPoint(graph_node[q.get_index()]->x() + 25, graph_node[q.get_index()]->y());
                edge_line.push_back({ start_point,end_point });
            }
        }
    }
}

void GraphMenu::enter()
{
    for (int i = 0; i < 100; i++) graph_node[i]->InitAnimation();
    back = new QAnimatedButton();
    addWidget(back);
    back->resize(80, 80);
    back->setStyleSheet("QPushButton{background-color: transparent;border: 0px ;border-radius: 40px}"\
        "QPushButton:hover{background-color: transparent;border: 0px;border-radius: 40px}"\
        "QPushButton:pressed{background-color: transparent;border: 0px;border-radius: 40px}");

    back->setIcon(QIcon(tr("picture/back.png")));
    back->setIconSize(back->size());
    back->setAutoFillBackground(true);
    back->InitAnimation(900, 600, 1100, 600);
    connect(back, &QAnimatedButton::end, this, [=]() {
        delete back;
        emit waitTM();
        for (int i = 0; i < 12; i++) delete[] star[i];
        });
}
void GraphMenu::initGraph()
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
    for (int i = 0; i < G->get_Coursenum(); i++)
    {
        G->get_max_depth(i);
    }
}
void GraphMenu::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    static QPixmap* pix = new QPixmap();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    srand(time(0));
    int color;
    for (auto p : edge_line)
    {
        color = rand() % 19;
        //painter.setPen(QPen(Qt::GlobalColor(color), 2));
        painter->setPen(QPen(Qt::white, 2));
        painter->drawLine(p.first, p.second);
    }
    painter->drawPixmap(0, 0, *pix);
    painter->drawRect(0, 0, 1000, 600);
}

void GraphMenu::mousePressEvent(QMouseEvent* event)
{
    star_point = event->pos();
    if (star[0] == NULL) return;
    if (this->flag == 0)
    {
        if (event->pos().x() <= 500 || event->pos().y() <= 400)
            emit mouseClick();
    }
    else emit mouseClick();
}
//QRectF GraphMenu::boundingRect() const
//{
//    return QRectF(0, 0, 1000, 600);
//}