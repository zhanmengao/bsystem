//
// Created by gaochong on 2020/5/31.
//

#include "Watermark.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QLayout>
#include <QMoveEvent>
#include <QPainter>
#include <QScreen>
#include<QDateTime>

Watermark::Watermark(QWidget *parent) : QWidget(parent)
{
    int dwidth  = QApplication::desktop()->width();
    int dheight = QApplication::desktop()->height();
    setFixedSize( dwidth, dheight );

    setWindowFlags( Qt::FramelessWindowHint |
                    Qt::WindowTransparentForInput |
                    Qt::Dialog |
                    Qt::Tool   |
                    Qt::WindowStaysOnTopHint |
                    Qt::BypassWindowManagerHint );

    this->setAttribute(Qt::WA_TranslucentBackground, true);

}


Watermark::~Watermark()
{
    //delete downloader;
}


void Watermark::closeEvent( QCloseEvent *event )
{

    qDebug()<< "closeEvent...";
    //event->ignore();  //阻止，事件不再传递
}

void Watermark::ClearDesktop()
{
    this->ShowText = 0;

    update();
}

void Watermark::UpDesktopText( QString Text  )
{
    this->ShowText = 1;
    this->DesktopText = Text;

    qDebug() << Text;

    update();
}

void Watermark::UpDesktopPIC( QString path  )
{
    this->ShowText = 2;
    this->DesktopPix = path;

    qDebug() << path;

    update();
}

void Watermark::SetTextVar( int Show ,QString textImage )
{
    DesktopText = textImage;
    DesktopPix = textImage;
    ShowText = Show;
}


void Watermark::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    Q_UNUSED(event);

    //qDebug() << "paintEvent(QPaintEvent *event)";


    if ( ShowText == 0 )
    {
        qDebug() << "取消水印";
        return;
    }


    QString head;

    //data
    if(bDate)
    {
        head = QDate::currentDate().toString("yyyy-MM-dd") + "   ";
    }
    if(bMachineName)
    {
        head += SysInfo::GetHostName() + "    ";
    }
    width = head.toUtf8().size()*0.9f;
    width += DesktopText.toUtf8().size()*0.5f;
    width *=mFontSize;
    if(width <=0)
    {
       exit(-1);
    }

    hight = mFontSize<44?64:mFontSize+20;
    QString Text = head + this->DesktopText;
    QDesktopWidget *des = QApplication::desktop();
    QPainter painter;
    painter.begin(this);
    painter.setPen(mColor); //颜色
    painter.setFont(QFont("Arial", mFontSize));   //字体字号
    QTransform transform;
    transform.rotate(mAngle);               //角度
    painter.setTransform(transform);

    x = -width;
    y = 100;

    int imageWidth = 0;

    qsrand( 255 );

    while( y < des->size().height() *2 )
    {   
        while( x < des->size().width() )
        {
            if( ShowText != 1 )
            {
               int colorV = rand() % 255;
                painter.setPen(QColor( 0, 0 , colorV, 16)); //颜色
                QPixmap image1;
                image1.load( DesktopPix );
                painter.setOpacity(0.2);//透明度设置
                painter.drawPixmap( x , y, image1);
                imageWidth = image1.width();
                x += imageWidth * 2; /** 列间隔 */
            }
            else
            {
                painter.drawText( QPoint( x, y ), Text);
                x += width; /** 列间隔 */
            }
        }

        if( ShowText != 1 )
        {
            y += imageWidth * 2;    /** 行间隔 */
            x = -width;   /** 列再起始位置 */
        }
        else
        {
            y += hight;    /** 行间隔 */
            x = -width;   /** 列再起始位置 */
        }
    }
}

void Watermark::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    //qDebug()<< "void MainWindow::showEvent(QShowEvent *event) ...";

    switch( ShowText )
    {
        case 2:
            UpDesktopPIC(  DesktopPix );
            break;

        case 1:
            UpDesktopText( DesktopText );
            break;

        case 0:
            ClearDesktop();
            break;
    }

}

void Watermark::SetShowDate(bool show)
{
    bDate = show;
}
void Watermark::SetShowMachineName(bool show)
{
    bMachineName = show;
}
void Watermark::SetFontSize(int fontsize)
{
    mFontSize = fontsize;
}
void Watermark::SetAngle(int angle)
{
    if(angle>0)
    {
        angle = -angle;
    }
    mAngle = angle;
}
void Watermark::SetColor(int r,int g,int b,int a)
{
    mColor = QColor(r,g,b,a);
}






