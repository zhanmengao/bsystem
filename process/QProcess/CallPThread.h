//
// Created by gaochong on 2020/5/7.
//

#ifndef UAMLINUXCLIENT_CALLPTHREAD_H
#define UAMLINUXCLIENT_CALLPTHREAD_H


#include <QtCore/QThread>
#include <QtGui/QtGui>
#include <QProcess>
#include <QMetaType>

class CallPThread : public QThread{

    Q_OBJECT

private:
    QStringList cmmandStr;
    QProcess *cloudAppOpenProcess;

public:
    CallPThread( QStringList argString,  QObject *parent);
    virtual ~CallPThread();

    bool event(QEvent *event) override;

protected:
    void run() override;

public slots:
    void cloudAppOpenProcessFinished(int code, QProcess::ExitStatus exitStatus);
    void cloudAppOpenProcessError(QProcess::ProcessError processError);
    void cloudAppOpenProcessRedFromStdError();
    void cloudAppOpenProcessStarted();
    void cloudAppOpenProcessReadStandardOutput();
};


#endif //UAMLINUXCLIENT_CALLPTHREAD_H
