#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPushButton>
#include<QVBoxLayout>
#include <libusb-1.0/libusb.h>
#include<QComboBox>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void read_usb();
private slots:
    void OnClickOpenUSB();
    void OnClickSend();
private:
    QPushButton* openBtn;
    QPushButton* sendBtn;
    QComboBox* USBList;
    QVBoxLayout *layout= nullptr;
    libusb_context *ctx= nullptr;
    libusb_device* dev= nullptr;
    libusb_device_handle *udev = nullptr;
};
#endif // MAINWINDOW_H
