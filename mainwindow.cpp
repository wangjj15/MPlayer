#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <cstring>
#include <QTime>
#include "myvideowidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); //将用户界面文件给当前类使用
    this->playFlag = false; //播放标志位放到 暂停状态
    currentIndex=0;
    playList=new QMediaPlaylist; //创建播放列表
    playList->setPlaybackMode(QMediaPlaylist::Loop);//设置播放循环为列表循环
    //setFixedSize(1030,564);//固定播放器尺寸
    player=new QMediaPlayer; //创建媒体播放器对象
    videoWidget= new MyVideoWidget(); //创建用于显示媒体播放器的控件
    player->setPlaylist(playList); //给播放器添加播放列表
    ui->volume_label->setText(QString::number(ui->volume->value()));
    this->setStyleSheet("background-image:url(:/qrc/background.jpg)");//设置界面背景
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置tableWidget禁止编辑
    ui->videoLayout->addWidget(videoWidget);
    ui->videoLayout->addWidget(ui->horizontalSlider);
    ui->videoLayout->addLayout(ui->LhorizontalLayout);
    ui->play->setStyleSheet("image:url(:/qrc/play.png)"); //给所有按钮控件添加图标
    ui->open->setStyleSheet("image:url(:/qrc/open.png)");
    ui->stop->setStyleSheet("image:url(:/qrc/stop.png)");
    ui->next->setStyleSheet("image:url(:/qrc/next.png)");
    ui->last->setStyleSheet("image:url(:/qrc/last.png)");
    ui->volume_label->setStyleSheet("image:url(:/qrc/volume.png)");
    ui->deleteBtn->setStyleSheet("image:url(:/qrc/delete.png)"); 
    ui->fullscreenBtn->setStyleSheet("image:url(:/qrc/external.png)");
    ui->previous->setStyleSheet("image:url(:/qrc/previous.png)");
    ui->listBtn->setStyleSheet("image:url(:/qrc/list.png)");
    ui->open->setFixedSize(48,48);
    ui->play->setFixedSize(32,32);
    ui->stop->setFixedSize(32,32);
    ui->last->setFixedSize(32,32);
    ui->next->setFixedSize(32,32);
    ui->previous->setFixedSize(32,32);
    ui->listBtn->setFixedSize(24,24);
    ui->volume_label->setFixedSize(24,24);
    ui->fullscreenBtn->setFixedSize(24,24);
    ui->deleteBtn->setFixedSize(48,48);
    videoWidget->show();
    ui->open->hide();
    ui->deleteBtn->hide();
    ui->tableWidget->hide();
    listFlag = false;

    statusLbl = new QLabel("contact wangjj15@126.com for more information");
    this->statusBar()->addWidget(statusLbl);


    //链接信号和槽
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(playTo(int, int)));
    connect(ui->tableWidget,SIGNAL(itemSelectionChanged()),this,SLOT(itemSelectionChanged()));
    connect(playList, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSongList(int)));

    //打开用户保存的歌曲列表
    configIniRead = new QSettings("PlayList.ini", QSettings::IniFormat);
    QStringList songList;
    for(int i=0; ; ++i) //将所有的播放列表显示在列表控件中
    {
        if(configIniRead->value("/song/"+QString::number(i),1000).toInt()!=1000)
        {
            QString path = configIniRead->value("/song/"+QString::number(i)).toString();
            songList.append(path);
            playList->addMedia(QUrl::fromLocalFile(path));
            player->setVolume(ui->volume->value());
            QString fileName=path.split("\\").last();
            int rownum=ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rownum);
            ui->tableWidget->setItem(rownum, 0, new QTableWidgetItem((fileName.split(".").front()).split("/").last()));
            ui->tableWidget->setItem(rownum, 1, new QTableWidgetItem(fileName.split(".").last()));
            //configIniRead->clear();
        }
        else
        {
            break;
        }
    }
    playList->setCurrentIndex(currentIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_clicked() //定义打开文件按钮响应时间
{
    configIniWrite = new QSettings("PlayList.ini", QSettings::IniFormat);
    QStringList pathList=QFileDialog::getOpenFileNames(this, tr("选择文件"), ".", "Multimedia (*.mp3 *.wmv *.mpg *.avi);;All files (*.*)");
    for(int i=0; i<pathList.size(); ++i) //将打开的文件显示在列表控件中
    {
        QString path=QDir::toNativeSeparators(pathList.at(i));
        if(!path.isEmpty())
        {
            playList->addMedia(QUrl::fromLocalFile(path));
            player->setVolume(ui->volume->value());
            ui->volume_label->setText(QString::number(ui->volume->value()));
            QString fileName=path.split("\\").last();
            int rownum=ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rownum);
            ui->tableWidget->setItem(rownum, 0, new QTableWidgetItem((fileName.split(".").front()).split("/").last()));
            ui->tableWidget->setItem(rownum, 1, new QTableWidgetItem(fileName.split(".").last()));
            configIniWrite->setValue("/song/"+QString::number(rownum), path);//保存歌曲列表
        }
    }
}

void MainWindow::on_deleteBtn_clicked()  //定义删除按钮响应事件
{
    configIniDelete = new QSettings("PlayList.ini",QSettings::IniFormat);//打开播放列表配置文件
    currentIndex =ui->tableWidget->currentRow();
    if( currentIndex >= 0)
    {
        for (int i=currentIndex; ; i++)
        {
            if(configIniDelete->value("/song/"+QString::number(i+1),1000).toInt() != 1000) //如果下一行存在播放文件
            {
                configIniDelete->setValue("/song/"+QString::number(i),configIniDelete->value("/song/"+QString::number(i+1)).toString());
            }
            else
            {
                configIniDelete->remove("/song/"+QString::number(i));
                break;
            }
        }
        playList->removeMedia(currentIndex);
        cout<<currentIndex<<endl;
        ui->tableWidget->removeRow(currentIndex);
    }
}

void MainWindow::on_stop_clicked() //定义停止播放事件
{
    //videoWidget->close();
    player->stop();
    ui->play->setStyleSheet("image:url(:/qrc/play.png)");
    this->playFlag = false; //将播放标志位 放到暂停状态
    ui->label_8->setText("0:0");
    ui->labelProgress->setText("0:0");
}

void MainWindow::on_play_clicked()  //定义播放按钮事件
{
    if (!playList->isEmpty())
    {
        if(!this->playFlag)
        {
            if((player->position()==0)||(currentIndex!=playList->currentIndex()))
            {   //如果选择的是当前播放对象，那么将会继续当前播放进度播放

                QList<QTableWidgetItem*> items=ui->tableWidget->selectedItems();
                currentIndex=ui->tableWidget->row(items.at(0));
                if(currentIndex == -1)
                {
                    currentIndex = 0;
                }
                if(ui->tableWidget->item(currentIndex,1)->text()!="mp3")
                {
                    player->setVideoOutput(videoWidget);
                    videoWidget->show();
                    playList->setCurrentIndex(currentIndex);
                    player->play();
                    ui->play->setStyleSheet("image:url(:/qrc/pause.png)");
                    this->playFlag = true;
                }
                else
                {
                    playList->setCurrentIndex(currentIndex);
                    player->play();
                    ui->play->setStyleSheet("image:url(:/qrc/pause.png)");
                    this->playFlag = true;
                }
            }
            else
            {
                player->play();
                ui->play->setStyleSheet("image:url(:/qrc/pause.png)");
                this->playFlag = true;
            }
        }
        else
        {
            player->pause();
            ui->play->setStyleSheet("image:url(:/qrc/play.png)");
            this->playFlag = false;
        }
    }
    else
    {
        QMessageBox::warning(this,"播放错误","播放列表为空，请打开要播放的文件",QMessageBox::Ok);
    }

}

void MainWindow::on_volume_sliderMoved(int position) //定义声音滑条响应函数
{
    player->setVolume(position); //将播放器声音设置为滑条值
    if (position < 10)
        ui->volume_label->setText("0"+QString::number(position));
    else
        ui->volume_label->setText(QString::number(position));
}

void MainWindow::on_horizontalSlider_sliderMoved(int position) //定义播放器进度条函数
{
    player->setPosition(position);//将播放器进度移动到滑条当前进度
}

void MainWindow::positionChanged(qint64 position) //定义播放进度变化函数，用以改变时间进度值跟随当前时间
{
    ui->horizontalSlider->setValue(position);
    QTime n(0,0,0);
    QTime t;
    t=n.addMSecs(position);
    ui->labelProgress->setText(QString::number(t.minute())+ ":" +QString::number(t.second()));
}

void MainWindow::durationChanged(qint64 duration) //定义总时间函数，当播放内容变化时，总时间也随之变化
{
    ui->horizontalSlider->setRange(0, duration);
    QTime n(0,0,0);
    QTime t;
    t=n.addMSecs(duration);
    ui->label_8->setText(QString::number(t.minute())+ ":" +QString::number(t.second()));
}

void MainWindow::playTo(int i, int /* j */) //定义播放指定对象，用以响应双击列表中对象完成播放事件
{
    videoWidget->close();
    if(ui->tableWidget->item(i,1)->text()!="mp3")
    {
        player->setVideoOutput(videoWidget);
        videoWidget->show();
        playList->setCurrentIndex(i);
        player->play();
        ui->play->setStyleSheet("image:url(:/qrc/pause.png)");
        this->playFlag = true;
    }
    else
    {
        playList->setCurrentIndex(i);
        player->play();
        ui->play->setStyleSheet("image:url(:/qrc/pause.png)");
        this->playFlag = true;
    }
}

void MainWindow::updateSongList(int i) //定义响应函数，用以响应播放列表更新
{
    ui->tableWidget->selectRow(i);
}

void MainWindow::on_last_clicked() //定义播放至最后一个媒体文件的函数
{
    //videoWidget->close();
    currentIndex=playList->currentIndex();
    //if(--currentIndex<0) currentIndex=0;
    if (playList->mediaCount() >0 )
    {
        currentIndex = playList->mediaCount()-1;
        if(ui->tableWidget->item(currentIndex,1)->text()!="mp3")
        {
            player->setVideoOutput(videoWidget);
            videoWidget->show();
            playList->setCurrentIndex(currentIndex);
            player->play();
            ui->play->setStyleSheet("image:url(:/qrc/pause.png)");
            this->playFlag = true;
        }
        else
        {
            playList->setCurrentIndex(currentIndex);
            player->play();
            ui->play->setStyleSheet("image:url(:/qrc/pause.png)");
            this->playFlag = true;
        }
    }
}

void MainWindow::on_next_clicked() //定义播放下一个文件的函数
{
    //videoWidget->close();
    currentIndex=playList->currentIndex();
    if (playList->mediaCount()>0)
    {
        if(++currentIndex==playList->mediaCount()) currentIndex=0;
        if(ui->tableWidget->item(currentIndex,1)->text()!="mp3")
        {
            player->setVideoOutput(videoWidget);
            videoWidget->show();
            playList->setCurrentIndex(currentIndex);
            player->play();
            ui->play->setStyleSheet("image:url(:/qrc/pause.png)");
            this->playFlag = true;
        }
        else
        {
            playList->setCurrentIndex(currentIndex);
            player->play();
            ui->play->setStyleSheet("image:url(:/qrc/pause.png)");
            this->playFlag = true;
        }
    }
}

void MainWindow::on_volume_valueChanged(int value) //定义用户调节声音，用于改变声音的大小
{
    player->setVolume(value);
    if (value<10)
        ui->volume_label->setText("0"+QString::number(value));
    else
        ui->volume_label->setText(QString::number(value));
}

void MainWindow::itemSelectionChanged() //响应用户选择的播放项目的变化
{
    QList<QTableWidgetItem*>items=ui->tableWidget->selectedItems();
    currentIndex=ui->tableWidget->row(items.at(0));
}

void MainWindow::on_fullscreenBtn_clicked()
{
    videoWidget->setFullScreen(!videoWidget->isFullScreen());
    QMessageBox::warning(this,"Tips","Press Ese or Doubleclick to Exit Fullscreen",QMessageBox::Ok);
    this->hide();
}

void MainWindow::on_previous_clicked()
{
    //videoWidget->close();
    currentIndex=playList->currentIndex();
    if(playList->mediaCount()>0)
    {
        if(currentIndex==0)
            currentIndex=playList->mediaCount()-1;
        else
            currentIndex = currentIndex-1;
        if(ui->tableWidget->item(currentIndex,1)->text()!="mp3")
        {
            player->setVideoOutput(videoWidget);
            videoWidget->show();
            playList->setCurrentIndex(currentIndex);
            player->play();
            ui->play->setStyleSheet("image:url(:/qrc/pause.png)");
            this->playFlag = true;
        }
        else
        {
            playList->setCurrentIndex(currentIndex);
            player->play();
            ui->play->setStyleSheet("image:url(:/qrc/pause.png)");
            this->playFlag = true;
        }
    }
}

void MainWindow::on_listBtn_clicked()
{
    if(!listFlag)
    {
        ui->open->show();
        ui->deleteBtn->show();
        ui->tableWidget->show();
        listFlag = true;
    }
    else
    {
        ui->open->hide();
        ui->deleteBtn->hide();
        ui->tableWidget->hide();
        listFlag = false;
    }
}

//void MainWindow::keyReleaseEvent(QKeyEvent *event)
//{
//    int keyValue = event->key();
//    if (keyValue &Qt::Key_Escape)
//    {
//        //QMessageBox::warning(this,"a","a",QMessageBox::Ok);
//        videoWidget->setFullScreen(false);
//        this->show();
//    }
//}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    configIniWrite = new QSettings("PlayList.ini",QSettings::IniFormat);
    foreach (QUrl url,event->mimeData()->urls()){
        QString path=QDir::toNativeSeparators(url.toLocalFile());
        if(!path.isEmpty())
        {
            playList->addMedia(QUrl::fromLocalFile(path));
            player->setVolume(ui->volume->value());
            ui->volume_label->setText(QString::number(ui->volume->value()));
            QString fileName=path.split("\\").last();
            int rownum=ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rownum);
            ui->tableWidget->setItem(rownum, 0, new QTableWidgetItem((fileName.split(".").front()).split("/").last()));
            ui->tableWidget->setItem(rownum, 1, new QTableWidgetItem(fileName.split(".").last()));
            configIniWrite->setValue("/song/"+QString::number(rownum), path);//保存歌曲列表
            event->acceptProposedAction();
            //QMessageBox::warning(this,"a",url.toString(),QMessageBox::Ok);
        }
        else
        {
            event->ignore();
            QMessageBox::warning(this,"a","fail",QMessageBox::Cancel);

        }
    }
}
