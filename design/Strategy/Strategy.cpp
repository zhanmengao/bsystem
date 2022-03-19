#include"Strategy.h"
#include<QGuiApplication>
#include<QScreen>
#include<QProcessEnvironment>
#include<QCryptographicHash>
#define des_encrypt(data,key) data
enum APP_MODE
{
    SHELL = 1,
    APP = 2,
};
void xFreerdp::SetHomeFilePath(const QString& homeFilePath)
{
    mHomeFilePath = homeFilePath;
}
bool xFreerdp::Build(const QJsonObject& jsonObject)
{
    RDS = jsonObject.value("ip").toString();
    PORT = jsonObject.value("port").toString();
    USERNAME = jsonObject.value("userName").toString();
    PASSWORD = jsonObject.value("passwordMD5").toString();
    DOMAINNAME = jsonObject.value("domain").toString();
    if (PASSWORD == "******")
    {
        /***
         * 6星 是平台生成的帐号，密码按规则重新生成
         */
        QByteArray bb2;
        bb2 = QCryptographicHash::hash(USERNAME.toLatin1(), QCryptographicHash::Md5);
        SSPASSWORD.append(bb2.toHex());
        SSPASSWORD = "Ve:" + SSPASSWORD.mid(0, 8).toUpper();

    }
    else
    {
        /**
         * 密码保持不变
         ***/
        SSPASSWORD = PASSWORD;
    }
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    App_width = screenGeometry.width();
    App_heigh = screenGeometry.height() - 80;

    if (!jsonObject.value("exePath").isNull())
    {

        exePath = jsonObject.value("exePath").toString();

    }
    else
    {
        QMessageBox::information(
                    NULL,
                    "",
                    "关键参数值 exePath 缺失，管理端版本过低，需联系技术支持中心升级管理端...",
                    QMessageBox::Yes);

        return false;
    }

    openMode = jsonObject.value("displayMode").toInt();
    appParameter = jsonObject.value("appParameter").toString();

    /**
     * alias 窗口标题
     ***/
    if (!jsonObject.value("appName").isNull())
    {
        appName = "" + jsonObject.value("appName").toString();
        if(appName.size()<=0)
        {
            appName = jsonObject.value("alias").toString();
        }
    }
    return true;
}
QString AbstractXf::GetTitle(const xFreerdp& xf)
{
    QString TITLE;
    if(xf.mHomeFilePath.isEmpty())
    {
        TITLE = xf.appName;
    }
    else
    {
        TITLE = "-打开: " + xf.mHomeFilePath;
    }
    TITLE = des_encrypt(TITLE, mKEY);        /** 加密字符串 */
    return TITLE;
}
QString AbstractXf::GetRDS(const xFreerdp& xf)
{
    QString RDS = des_encrypt(xf.RDS, mKEY);
    return RDS;
}
QString AbstractXf::GetUSERNAME(const xFreerdp& xf)
{
    QString USERNAME = des_encrypt(xf.USERNAME, mKEY);
    return USERNAME;

}
QString AbstractXf::GetSSPASSWORD(const xFreerdp& xf)
{
    QString SSPASSWORD = des_encrypt(xf.SSPASSWORD, mKEY);
    return SSPASSWORD;
}
QString AbstractXf::GetDOMAINNAME(const xFreerdp& xf)
{
    QString DOMAINNAME = des_encrypt(xf.DOMAINNAME, mKEY);
    return DOMAINNAME;
}
QString AbstractXf::GetRunMode(const xFreerdp& xf)
{
    return "shell";
}
QString AbstractXf::GetXfreerdp(const xFreerdp& xf)
{
    QString processCMD = QString(
                "%1 /u:%2 /p:%3 /w:%4 /h:%5"
                " /%6:\"%7\""
                " /v:%8 /port:%9 /t:\"%10\" /d:%11")
            .arg("vecloudapp")
            .arg(GetUSERNAME(xf))
            .arg(GetSSPASSWORD(xf))
            .arg(xf.App_width)
            .arg(xf.App_heigh)
            .arg(GetRunMode(xf))
            .arg(GetRun(xf))
            .arg(GetRDS(xf))
            .arg(xf.PORT)
            .arg(GetTitle(xf))
            .arg(GetDOMAINNAME(xf));
    processCMD += GetExtern();
    processCMD +=
            " /cert-ignore"    //忽悠证书校验确认
            " /video"          //视频重定向
            " /microphone:sys:alsa /sound:sys:alsa"    //音频重定向
            " +gfx-progressive +gfx-small-cache +gfx-thin-client"   //gfx 优化
            " +auto-reconnect" //自动重连
            " +home-drive"     //挂载HOME
            " +clipboard";     //剪贴板重定向
    return processCMD;
}

QString V2019Xf::GetRun(const xFreerdp& xf)
{
    QString cmd;
    if(xf.mHomeFilePath.isEmpty())
    {
        cmd.push_back('\'');
        cmd.push_back(xf.exePath);
        cmd.push_back('\'');
        cmd.push_back(' ');
        cmd.push_back('\'');
        cmd.push_back(xf.appParameter);
        cmd.push_back('\'');
    }
    else
    {
        cmd.push_back("OPENHOMEFILE");
        cmd.push_back(' ');
        cmd.push_back('\'');
        cmd.push_back(xf.mHomeFilePath);
        cmd.push_back('\'');
    }
    return cmd;
}

QString V2019Xf::GetExtern()
{
    return "";
}
QString V2020Xf::GetRun(const xFreerdp& xf)
{
    QString run;
    run.push_back('\'');
    run.push_back(xf.exePath);
    run.push_back('\'');
    if (xf.appParameter.length() >= 1)
    {
        /**
         *  如果应用下发有参数，把参数接在可执行文件后面，中间隔一个空格
         ***/
        run.push_back(' ');
        run.push_back(xf.appParameter);
    }
    QString cmdPath;
    if(xf.openMode == APP_MODE::APP)
    {
        cmdPath = run;
    }
    else
    {
        cmdPath= "RDPINIT" + QString(" ") + run;
    }
    return cmdPath;
}

QString V2020Xf::GetExtern()
{
    QString ex;
    QString USBCODE = QProcessEnvironment::systemEnvironment().value("HOME") + "/.config/" + "RemoteUSBList";
    QString uamhost = " /uamhost:\"%1\"";
    uamhost = uamhost.arg("CHANNEL_UAMHOST");
    QString usb = " /usb:\"%1\"";
    usb = usb.arg(USBCODE);
    ex+=usb;
    ex+=uamhost;
    return ex;
}
QString V2020Xf::GetRunMode(const xFreerdp& xf)
{
    if(xf.openMode == APP_MODE::APP)
    {
        return "app";
    }
    else
    {
        return "shell";
    }
}
