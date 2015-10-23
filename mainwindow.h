#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QSettings>
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_open_clicked();//打开文件按钮的槽

    void on_stop_clicked();//停止播放按钮的槽

    void on_play_clicked();//播放按钮的槽

    void on_volume_sliderMoved(int position);//音量滑动条拖动的槽

    void on_horizontalSlider_sliderMoved(int position);//播放进度滑动条拖动的槽

    void positionChanged(qint64);//播放进度滑动条进度改变的槽
    void durationChanged(qint64);//播放进度滑动条总时长改变的槽
    void playTo(int, int);//切换歌曲的槽
    void updateSongList(int);//播放列表更新的槽

    void on_last_clicked();//上一曲按钮的槽

    void on_next_clicked();//下一曲按钮的槽

    void on_volume_valueChanged(int value);//音量滑动条值改变的槽

    void itemSelectionChanged();

    void on_deleteBtn_clicked(); //删除列表中文件的槽

    void on_fullscreenBtn_clicked(); //全屏按钮的槽

    void on_previous_clicked();//上一首的槽

    void on_listBtn_clicked();//显示列表的槽

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player; //媒体播放器指针
    QMediaPlaylist *playList; //播放列表指针
    QVideoWidget *videoWidget; //视频播放窗体指针
    QSettings *configIniWrite; //配置文件写指针
    QSettings *configIniRead; //配置文件写指针
    QSettings *configIniDelete; //配置文件删除指针
    QWidget *widget;
    QLabel *statusLbl;
    int currentIndex;
    bool playFlag;//播放标志位，true--playing, false--pause
    bool listFlag;//列表标志位，用于显示列表和不显示播放列表
    void dragEnterEvent(QDragEnterEvent *event);
   // void keyReleaseEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
