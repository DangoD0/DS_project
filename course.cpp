#include <QPushButton>
#include <QtWidgets/QWidget>
#include <qlabel.h>
#include "QAnimated.h"
course_widget::course_widget(QWidget *parent) :QListWidget(parent)
{
	G = new graph(100);
}
void course_widget::initGraph()
{
	ifstream ifs;
	ifs.open("course.txt", ios::in);
	if (!ifs.is_open())
	{
		cout << "error opening course.txt!" << endl;
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
	ifs.open("edge.txt", ios::in);
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
	G->sortForSchedule();
	//for (int i = 0; i < 8; i++)
	//{
	//	cout << "semester " << i << ":";
	//	for (auto p : G->l[i].getA())
	//	{
	//		qDebug() << p.get_name() << " ";
	//	}
	//	puts("");
	//}
	//qDebug() << G->get_max_depth(0);

}
course_widget::~course_widget()
{
	delete G;
}