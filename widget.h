#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QHBoxLayout>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QTableWidget>
#include <QList>
#include <QLine>

//Класс окна графического представления QGraphicsView
class MainWindow : public QGraphicsView {

    Q_OBJECT
public:
    QList<QPoint> list_points;
    QList<QGraphicsEllipseItem*> list_ellipse;
    QList<QGraphicsLineItem*> list_line;
    QList<QGraphicsItem*> list_move;
    MainWindow(QWidget *parent = 0);
protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
private:
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipseItem;
    QGraphicsEllipseItem *new_ellipseItem;
    QGraphicsLineItem *line;
    QPoint new_point;
    bool flag = false;
    int index_ellipse = 0;
    int index_point = 0;
    QPoint coordinate_newellipse;
signals:
    void signal(int, int, bool);
    void replace_signal(int, int, int);
public slots:


};

class Widget : public QWidget
{
    Q_OBJECT

public:
    QList<QPoint> list_tablewidget;
    QTableWidget *tableWidget;
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    QHBoxLayout *myLayoutScene;
    MainWindow *myGraphics;
public slots:
    void addItemTableWidget(int, int, bool);
    void replaceItemTableWidget(int,int, int);
};
#endif // WIDGET_H
