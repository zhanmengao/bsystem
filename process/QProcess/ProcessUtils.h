#ifndef PROCESSUTILS_H
#define PROCESSUTILS_H
#include<QProcess>

class QProcessHelper
{
public:
    static QProcess* Fork(const QString& processPath,const QStringList& parameters)
    {
        QProcess *pPro = new QProcess();
        pPro->start(processPath,parameters);
        return pPro;
    }
    static QProcess* Fork(const QString& processPath,const QStringList& parameters,const QStringList& env)
    {
        QProcess *pPro = new QProcess();
        pPro->setEnvironment(env);
        pPro->start(processPath,parameters);
        return pPro;
    }
    static void Wait(QProcess *pPro)
    {
        pPro->waitForFinished();
    }
};

#endif // PROCESSUTILS_H
