//
// Created by gaochong on 2020/5/31.
//

#ifndef CLOUDAPPOPEN_WATERMARK_H
#define CLOUDAPPOPEN_WATERMARK_H

#include <QWidget>
#define _LOCAL 1
#if _LOCAL
#include"../../SystemInfo/QSystemInfo/SystemInfo.h"
#else
#endif
#ifdef Q_OS_WINDOWS
#include<tchar.h>
#include<windows.h>
#else
#include <pwd.h>
#include <unistd.h>
#endif
class Watermark  :public QWidget{

    Q_OBJECT

public:
    Watermark( QWidget *parent = 0 );
    ~Watermark();
    void SetTextVar( int Show ,QString textImage );
    void SetShowDate(bool show);
    void SetShowMachineName(bool show);
    void SetFontSize(int fontsize);
    void SetAngle(int angle);
    void SetColor(int r,int g,int b,int a);
protected:

    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:

    QString DesktopText;
    QString DesktopPix;
    int     ShowText = 1;
    int width = 100;
    int hight = 64;
    int x = 64;
    int y = 100;
    void ClearDesktop();
    void UpDesktopText( QString Text);
    void UpDesktopPIC( QString FileName  );
private:
    //parameter
    bool bDate = false;
    bool bMachineName = false;
    int mFontSize = 0;
    int mAngle;
    QColor mColor;
private Q_SLOTS:
    //void DownloadOK( QString filePath );


};


#endif //CLOUDAPPOPEN_WATERMARK_H
