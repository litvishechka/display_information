#include "widget.h"
#include <QMessageBox>
#include <QList>
#include <QProcess>
#include <QLabel>
#include <QHeaderView>
//Конструктор виджета
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);
    myLayoutScene = new QHBoxLayout(this);
    myGraphics = new MainWindow;
    myLayoutScene->addWidget(myGraphics);
    setLayout(myLayoutScene);

    Widget::tableWidget = new QTableWidget();
    tableWidget->setColumnCount(2);
    tableWidget->setColumnWidth(0, 50);
    tableWidget->setColumnWidth(1, 50);
    tableWidget->setFixedWidth(103);
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("X")));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Y")));
    tableWidget->setShowGrid(true);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->verticalHeader()->setVisible(false);
    myLayoutScene->addWidget(tableWidget);
    connect(myGraphics, SIGNAL(signal(int,int, bool)),  this, SLOT(addItemTableWidget(int,int, bool)));
    connect(myGraphics, SIGNAL(replace_signal(int,int, int)),  this, SLOT(replaceItemTableWidget(int,int, int)));
}

Widget::~Widget() {}

void Widget::addItemTableWidget(int x, int y, bool flag)
{
    QList<QPoint>::iterator it_list_tablewidget = list_tablewidget.begin();
    if (flag) {
        while (it_list_tablewidget != list_tablewidget.end()) {
            if (*it_list_tablewidget == QPoint(x, y)) it_list_tablewidget = list_tablewidget.erase(it_list_tablewidget);
            else it_list_tablewidget++;
        }
        tableWidget->clear();
        tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("X")));
        tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Y")));
        tableWidget->setColumnCount(2);
        tableWidget->setRowCount(0);
        for (int i = 0; i < list_tablewidget.length(); i++) {
            tableWidget->insertRow(tableWidget->rowCount());
            tableWidget->setItem(tableWidget->rowCount() - 1,  0,  new QTableWidgetItem(QString::number(list_tablewidget[i].x())));
            tableWidget->setItem(tableWidget->rowCount() - 1,  1,  new QTableWidgetItem(QString::number(list_tablewidget[i].y())));
        }
    }
    else {
        list_tablewidget.append(QPoint(x, y));
        tableWidget->insertRow(tableWidget->rowCount());
        tableWidget->setItem(tableWidget->rowCount() - 1,  0,  new QTableWidgetItem(QString::number(x)));
        tableWidget->setItem(tableWidget->rowCount() - 1,  1,  new QTableWidgetItem(QString::number(y)));
    }
}


void Widget::replaceItemTableWidget(int x, int y, int row_count)
{
    list_tablewidget[row_count] = QPoint(x, y);
    tableWidget->setItem(row_count,  0,  new QTableWidgetItem(QString::number(x)));
    tableWidget->setItem(row_count,  1,  new QTableWidgetItem(QString::number(y)));
}

//Конструктор графического представления
MainWindow::MainWindow(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(0, 0, 660, 720, this);
    scene->addRect(scene->sceneRect());
    setScene(scene);
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (flag) {
        for (int i = 0; i < list_ellipse.length(); i++) {
            if(list_ellipse[i] == new_ellipseItem) {
                index_ellipse = i;
            }
        }
        for (int i = 0; i < list_points.length(); i++) {
            if (list_points[i] == new_point) {
                index_point = i;
                // qDebug() << i;
            }
        }
        list_ellipse[index_ellipse]->setRect(mapToScene(event->pos()).x(),mapToScene(event->pos()).y(), 10, 10);
        list_points[index_point].setX(mapToScene(event->pos()).x() + 5);
        list_points[index_point].setY(mapToScene(event->pos()).y() + 5);
        emit replace_signal(mapToScene(event->pos()).x() + 5,mapToScene(event->pos()).y() + 5, index_ellipse);
        if (index_point != 0 && index_point != list_points.length() - 1) {
            list_line[index_point - 1]->setLine(list_points[index_point-1].x(), list_points[index_point-1].y(), mapToScene(event->pos()).x() + 5, mapToScene(event->pos()).y() + 5);
            list_line[index_point]->setLine(mapToScene(event->pos()).x() + 5, mapToScene(event->pos()).y() + 5, list_points[index_point+1].x(), list_points[index_point+1].y());
        }
        if (index_point == 0) {
            list_line[index_point]->setLine(mapToScene(event->pos()).x() + 5, mapToScene(event->pos()).y() + 5, list_points[index_point+1].x(), list_points[index_point+1].y());
        }
        if (index_point == list_points.length() - 1) {
            list_line[index_point-1]->setLine(list_points[index_point-1].x(), list_points[index_point-1].y(), mapToScene(event->pos()).x() + 5, mapToScene(event->pos()).y() + 5);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    bool last_element = false;
    if(event->buttons() == Qt::LeftButton && event->modifiers()!= Qt::ALT) {
        ellipseItem = new QGraphicsEllipseItem(mapToScene(event->pos()).x(), mapToScene(event->pos()).y(), 10, 10);
        ellipseItem->setBrush(Qt::darkMagenta);
        list_ellipse.append(ellipseItem);
        scene->addItem(ellipseItem);

        emit signal(mapToScene(event->pos()).x() + 5, mapToScene(event->pos()).y() + 5, false);

        list_points.append(QPoint(mapToScene(event->pos()).x() + 5, mapToScene(event->pos()).y() + 5));

        if (list_points.length() > 1) {
            line = new QGraphicsLineItem(list_points[list_points.length()-2].x(), list_points[list_points.length()-2].y(), list_points.last().x(), list_points.last().y());
            list_line.append(line);
            scene->addItem(line);
        }
    }
    else if(event->buttons() == Qt::RightButton) {
        QList<QGraphicsEllipseItem*>::iterator it = list_ellipse.begin();
        QList<QPoint>::iterator it_points = list_points.begin();
        QList<QGraphicsLineItem*>::iterator it_line = list_line.begin();
        while (it != list_ellipse.end()) {
            if ((*it)->contains(mapToScene(event->pos())) == true) {
                while (it_points != list_points.end()) {
                    if ((*it)->contains(mapToScene(*it_points))) {
                        while (it_line != list_line.end()) {
                            if ((*it)->collidesWithItem(*it_line)) {
                                last_element = true;
                                it_line = list_line.erase(it_line);
                                emit signal((*it_points).x(), (*it_points).y(), true);
                                it_points = list_points.erase(it_points);
                                if (it_points != list_points.begin()) (*(it_line))->setLine((*(it_points - 1)).x(), (*(it_points - 1)).y(), (*it_points).x(), (*it_points).y());
                            }
                            else it_line++;
                        }
                        if (!last_element) {
                        emit signal((*it_points).x(), (*it_points).y(), true);
                        it_points = list_points.erase(it_points);
                        }
                    }
                    else it_points++;
                }

                while ((*it)->collidingItems().length() > 1) {
                    scene->removeItem((*it)->collidingItems().front());
                }
                scene->removeItem(*it);

                it = list_ellipse.erase(it);
            }
            else it++;
        }
    }
}


void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    QList<QGraphicsEllipseItem*>::iterator it = list_ellipse.begin();
    QList<QPoint>::iterator it_points = list_points.begin();
    if (event->buttons() == Qt::LeftButton && event->modifiers()== Qt::ALT) {
        while (it != list_ellipse.end()) {
            if ((*it)->contains(mapToScene(event->pos())) == true) {
                new_ellipseItem = *it;
                flag = true;
                while (it_points != list_points.end()) {
                    if ((*it)->contains(mapToScene(*it_points))) {
                        new_point = *it_points;
                    }
                    it_points++;
                }
            }
            it++;
        }
    }
}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    this->setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event);
    if (flag)  emit replace_signal(mapToScene(event->pos()).x() + 5, mapToScene(event->pos()).y() + 5, index_ellipse);
    flag = false;
}







