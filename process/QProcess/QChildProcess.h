#ifndef QCHILDPROCESS_H
#define QCHILDPROCESS_H
#include<QProcess>
class QtProcess
{
public:
    QtProcess()
    {
    }
    QtProcess(const QString& cmdPath)
    {
        SetPath(cmdPath);
    }
    void SetPath(const QString& cmdPath)
    {
        mPath = cmdPath;
    }
    void AddParameter(const QString& paramater)
    {
        mParamaters.append(paramater);
    }
    void Start()
    {
        mPro.start(mPath,mParamaters);
    }
    QStringList GetEnv()
    {
        return mPro.environment();
    }
    void Wait()
    {
        mPro.waitForFinished();
    }
    void WaitStart()
    {
        mPro.waitForStarted();
    }
    QByteArray ReadSync()
    {
        mPro.waitForReadyRead();
        return mPro.readAll();
    }

private:
    QProcess mPro;
    QString mPath;
    QStringList mParamaters;
};

#endif // QCHILDPROCESS_H
