#include "fileitem.h"
#include "ui_fileitem.h"
#ifdef Q_OS_UNIX
#include<sys/stat.h>
#include<pwd.h>
#include<unistd.h>
#endif
#include<string>
#include<QFileInfo>
#include <errno.h>
FileItem::FileItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileItem)
{
    ui->setupUi(this);
}


FileItem::~FileItem()
{
    delete ui;
}

void FileItem::SetPath(const QString& nPath)
{
    ui->label->setText(nPath);
    ui->checkBox->setCheckState(Qt::CheckState::Checked);
    RefreshState();
    show();
}
void FileItem::RefreshState()
{
#ifdef Q_OS_UNIX
    struct stat fst;
    lstat(ui->label->text().toStdString().c_str(),&fst);
    //read user
    struct passwd* pw = getpwuid(fst.st_uid);
    if(pw)
    {
        ui->chmod->setText(pw->pw_name);
    }
    else
    {
        ui->chmod->setText(std::to_string(fst.st_uid).c_str());
    }
#else
    QFileInfo info(ui->label->text());
    ui->chmod->setText(info.owner());
#endif
}
bool FileItem::GetState()
{
    return ui->checkBox->checkState();
}
QString FileItem::GetPath()
{
    return ui->label->text();
}
void FileItem::SetOwner(uid_t uid,gid_t gid)
{
    int ret = chown(ui->label->text().toStdString().c_str(),uid,gid);
    if(ret == 0)
    {
        emit PrintLog(QString("%1 权限修改成功").arg(ui->label->text()));
    }
    else
    {
        QString error(strerror(errno));
        emit PrintLog(QString("%1 权限修改失败 %2").arg(ui->label->text()).arg(error));
    }
    RefreshState();
}
