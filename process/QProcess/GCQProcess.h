//
// Created by gaochong on 2020/10/12.
//

#ifndef CLOUDAPPOPEN_GCQPROCESS_H
#define CLOUDAPPOPEN_GCQPROCESS_H

#include <QProcess>

class GCQProcess : public QProcess
{
protected:
    qint64 writeData(const char *data, qint64 len) override;

    qint64 readData(char *data, qint64 maxlen) override;

public:
    void *qt_metacast(const char *string) override;

    bool waitForReadyRead(int msecs) override;

    bool waitForBytesWritten(int msecs) override;

    qint64 bytesToWrite() const override;

protected:
    qint64 readLineData(char *data, qint64 maxlen) override;
};


#endif //CLOUDAPPOPEN_GCQPROCESS_H
