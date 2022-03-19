#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMenu>
#include<QAction>
#include<QHBoxLayout>
#include<QPlainTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void OnClickOpen();
    void OnClickOpenURL();
private:
    QMenu *fileMenu;
    QAction *openAct;
    QAction *openURL;
    QHBoxLayout *layout;
    QPlainTextEdit *textEdit;
    QString fileName;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
