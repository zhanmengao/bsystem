#ifndef STRATEGY_H
#define STRATEGY_H
#include<QString>
#include <QtCore/QJsonObject>
#include<QMessageBox>
class xFreerdp
{
public:
    QString RDS;
    QString PORT;
    QString USERNAME;
    QString PASSWORD;
    QString DOMAINNAME;

    QString SSPASSWORD;
    int App_width;
    int App_heigh;
    QString exePath;
    QString appParameter;
    int openMode;
    QString appName;
    QString mHomeFilePath;
public:
    bool Build(const QJsonObject& obj);
    void SetHomeFilePath(const QString& homeFilePath);
};
class AbstractXf
{
public:
    QString GetXfreerdp(const xFreerdp& xf);
    virtual QString GetRun(const xFreerdp& xf) = 0;
    virtual QString GetExtern() = 0;
public:
    virtual QString GetRunMode(const xFreerdp& xf);
    virtual QString GetTitle(const xFreerdp& xf);
    virtual QString GetRDS(const xFreerdp& xf);
    virtual QString GetUSERNAME(const xFreerdp& xf);
    virtual QString GetSSPASSWORD(const xFreerdp& xf);
    virtual QString GetDOMAINNAME(const xFreerdp& xf);
protected:
    QString mKEY = "G9891505";
};

class V2019Xf:public AbstractXf
{
    virtual QString GetRun(const xFreerdp& xf);
    virtual QString GetExtern();
};
class V2020Xf:public AbstractXf
{
    virtual QString GetRun(const xFreerdp& xf);
    virtual QString GetExtern();
    virtual QString GetRunMode(const xFreerdp& xf);
};
#endif // STRATEGY_H
