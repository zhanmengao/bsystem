#ifndef DESKTOP_H
#define DESKTOP_H
#include<QList>
#include<QDesktopWidget>
#include<QApplication>
#include<QGuiApplication>
#include<QScreen>
class DesktopInfo
{
public:
    static QList<QScreen*> GetScreenList()
    {
        return QGuiApplication::screens();

    }

    static int GetScreenCount()
    {
        QDesktopWidget desktop;
        return desktop.screenCount();
    }

    static int GetWidth()
    {
        return QApplication::desktop()->width();
    }
    static int GetHeight()
    {
        return QApplication::desktop()->height();
    }
    static int GetDesktopID()
    {
        return QApplication::desktop()->winId();
    }

    static int GetScreenWidth(QScreen* s)
    {
        return s->size().width();
    }
    static int GetScreenHeight(QScreen* s)
    {
        return s->size().height();
    }
    static int GetMonitWidth(QScreen* s)
    {
        return s->physicalSize().width();
    }
    static int GetMonitHeight(QScreen* s)
    {
        return s->physicalSize().height();
    }
    static QString GetMonitSerialNumber(QScreen* s)
    {
        return s->serialNumber();
    }
};


#endif // DESKTOP_H
