#ifndef MYVIDEOWIDGET_H
#define MYVIDEOWIDGET_H

#include <QVideoWidget>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <iostream>
#include <QThread>

using namespace std;

class MyVideoWidget:public QVideoWidget
{
public:
    MyVideoWidget();

private:
    void keyReleaseEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    bool fullflag = false;
    //void dragEnterEvent(QDragEnterEvent *event);
};

#endif // MYVIDEOWIDGET_H
