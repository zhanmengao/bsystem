#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
QT_BEGIN_NAMESPACE
class QHostInfo;
QT_END_NAMESPACE
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
protected slots:
    void lookUp(const QHostInfo &host);
    void btnClicked();
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
