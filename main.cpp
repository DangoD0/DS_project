#include "test.h"
#include <QtWidgets/QApplication>
#include <qthread.h>
#include "MyWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    test* w_main = new test(0);
    test* w_wait = new test(1);
    test* w_course = new test(3);
    test* w_graph = new test(2);
    MyWidget* wa = new MyWidget();
    QScrollArea* scroller = new QScrollArea(wa);
    scroller->setFixedSize(1000, 700);
    w_graph->setParent(scroller);
    wa->setStyleSheet(("QWidget{background-color: rgb(57,57,57);}"));
    scroller->setWidget(w_graph);
    QScroller::grabGesture(scroller, QScroller::LeftMouseButtonGesture);
    w_graph->graphMenu();
    w_graph->paintGraph(0);
	MyStackedWidget* w = new MyStackedWidget();
    w->addWidget(w_main);
    w->addWidget(w_wait);
    w->addWidget(w_course);
    w->addWidget(wa);
    w->setFrameStyle(QFrame::Panel | QFrame::Raised);
    w->setStyleSheet("background-color: rgb(57,57,57);");
	w->move(400, 200);
    w->setCurrentIndex(0);
    w_main->mainMenu();
    w->show();
    w->setFixedSize(w->currentWidget()->size());
    w->adjustSize();
    QObject::connect(w_main, &test::waitTS, w, [=]() {
        w_wait->waitMenu(2);
        w->setCurrentIndex(1); 
        w->setFixedSize(w->currentWidget()->size()); 
        });
    QObject::connect(w_main, &test::waitTC, w, [=]() { 
        w_wait->waitMenu(3); w->setCurrentIndex(1);
        //w_course->courseMenu();
        w->setFixedSize(w->currentWidget()->size()); 
        });
    QObject::connect(w_main, &test::waitTG, w, [=]() {
		w_wait->waitMenu(4); w->setCurrentIndex(1);
		w->setFixedSize(w->currentWidget()->size()); 
		});
    QObject::connect(w_wait, &test::scheduleS, w, [=]() {
        w_course->scheduleMenu(); w->setCurrentIndex(2);
        w->move(200, 50);
        w->setFixedSize(w->currentWidget()->size()); 
        });
    QObject::connect(w_wait, &test::courseS, w, [=]() { 
        w_course->courseMenu(); 
        w->setCurrentIndex(2); 
        w->move(200, 50);
        w->setFixedSize(w->currentWidget()->size()); 
        });
    QObject::connect(w_wait, &test::mainS, w, [=]() {
        w_main->mainMenu(); w->setCurrentIndex(0);
        w->setFixedSize(w->currentWidget()->size());
        });
    QObject::connect(w_wait, &test::graphS, w, [=]() {
        w_graph->graphMenu();
        w->setCurrentIndex(3);
        w->setFixedSize(w->currentWidget()->size());
        w->move(200, 50);
        });
    QObject::connect(w_course, &test::waitTM, w, [=]() {
        w_wait->waitMenu(0); w->setCurrentIndex(1);
        w->move(400, 200);
        w->setFixedSize(w->currentWidget()->size());
        });
    QObject::connect(w_course, &test::waitTS, w, [=]() {
        w_wait->waitMenu(2); w->setCurrentIndex(1);
        w->move(400, 200);
        w->setFixedSize(w->currentWidget()->size());
        });
    QObject::connect(w_graph, &test::waitTM, w, [=]() {
        w_wait->waitMenu(0); w->setCurrentIndex(1);
        w->move(400, 200);
        w->setFixedSize(w->currentWidget()->size());
        });  
    /*
    MyWidget* w = new MyWidget();
    w->show();
    */
    return a.exec();
}