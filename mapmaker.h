#ifndef MapMaker_H
#define MapMaker_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QScrollArea>
#include <string>

using namespace std;

class MovableLabel;

namespace Ui {
class MapMaker;
}

class MovableLabel;

class MapMaker : public QWidget
{
    Q_OBJECT

    QScrollArea * scrollArea;

    int startX;
    int startY;
    int time;
    
public:
    explicit MapMaker(QWidget *parent = 0);
    ~MapMaker();

    void makePlatform(MovableLabel *label, QString file, bool& successful, QString& errorMSG);
    void makeDecor(MovableLabel *label, QString file, int w, int h);
    void makeItem(MovableLabel *label, QString file,string colType);
    
private slots:

    void labelClicked();

    void on_PBmakeObject_clicked();

    void on_PBresize_clicked();

    void on_PBsetTime_clicked();

    void on_Save_clicked();

    void on_any_click();

private:
    Ui::MapMaker *ui;
};


class MovableLabel : public QLabel
{
    Q_OBJECT

    bool mouseDragging;
    QPoint offset;

public:
    string type;
    string file;
    int xRange;
    int yRange;
    int xSpeed;
    int ySpeed;
    string collectableType;

    explicit MovableLabel(QWidget *parent): QLabel(parent) {
        setMouseTracking(true); // enable mouse move events
        mouseDragging = false;
    }
    void mouseMoveEvent(QMouseEvent *ev) {
        if (mouseDragging)
            this->move(mapToParent(ev->pos() - this->offset));
    }

    void mousePressEvent(QMouseEvent *ev) {
        mouseDragging = true;
        offset = ev->pos(); // location where mouse was clicked within the label
    }

    void mouseReleaseEvent(QMouseEvent *) {
        mouseDragging = false;
        emit clicked();
    }

signals:
    void clicked();

};

#endif // MapMaker_H
