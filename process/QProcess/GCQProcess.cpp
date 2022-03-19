//
// Created by gaochong on 2020/10/12.
//

#include "GCQProcess.h"
#include <QDebug>


qint64 GCQProcess::writeData(const char *data, qint64 len) {

    qDebug() << "GCQProcess::writeData";

    return QProcess::writeData(data, len);
}

qint64 GCQProcess::readData(char *data, qint64 maxlen) {

    qDebug() << "GCQProcess::readData";

    return QProcess::readData(data, maxlen);
}

qint64 GCQProcess::readLineData(char *data, qint64 maxlen) {

    qDebug() << "GCQProcess::readLineData";

    return QIODevice::readLineData(data, maxlen);
}

void *GCQProcess::qt_metacast(const char *string) {

    qDebug() << "GCQProcess::qt_metacast";

    return QProcess::qt_metacast(string);
}

bool GCQProcess::waitForReadyRead(int msecs) {

    qDebug() << "GCQProcess::waitForReadyRead";

    return QProcess::waitForReadyRead(msecs);
}

bool GCQProcess::waitForBytesWritten(int msecs) {

    qDebug() << "GCQProcess::waitForBytesWritten";

    return QProcess::waitForBytesWritten(msecs);
}

qint64 GCQProcess::bytesToWrite() const {

    qDebug() << "GCQProcess::bytesToWrite";

    return QProcess::bytesToWrite();
}
