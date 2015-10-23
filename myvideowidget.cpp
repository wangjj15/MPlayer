#include "myvideowidget.h"

MyVideoWidget::MyVideoWidget()
{
    //QVideoWidget::QVideoWidget();

}

void MyVideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    cout<<"enter"<<endl;
    if(!this->isFullScreen())
    {
        cout<<"enter if"<<endl;
        this->parentWidget()->parentWidget()->hide();
        this->setFullScreen(!this->isFullScreen());
        QMessageBox::warning(this,"Tips","Press Ese or Doubleclick to Exit Fullscreen",QMessageBox::Ok);
    }
    else
    {
        cout<<"enter else"<<endl;
        this->parentWidget()->parentWidget()->show();
        this->setFullScreen(!this->isFullScreen());
    }


}

void MyVideoWidget::keyReleaseEvent(QKeyEvent *event)
{
    int keyValue = event->key();
    if(keyValue &Qt::Key_Escape)
    {
        this->setFullScreen(false);
        this->parentWidget()->parentWidget()->show();
    }
}

//void MyVideoWidget::dragEnterEvent(QDragEnterEvent *event)
//{
////    foreach(QString mimeType,"wmv"){
////        if(event->mimeData()->hasFormat(mimeType))
////        {
////            event->acceptProposedAction();
////            QMessageBox::warning(this,"drag","success",QMessageBox::Cancel);
////        }
////    }
//    QMessageBox::warning(this,"drag","success",QMessageBox::Cancel);

//}
