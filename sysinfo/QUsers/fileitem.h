#ifndef FILEITEM_H
#define FILEITEM_H

#include <QWidget>

namespace Ui {
class FileItem;
}

class FileItem : public QWidget
{
    Q_OBJECT

public:
    explicit FileItem(QWidget *parent = nullptr);
    ~FileItem();
public:
    void SetPath(const QString& nPath);
    bool GetState();
    QString GetPath();
    void RefreshState();
    void SetOwner(uid_t uid,gid_t gid);

signals:
    //定义信号函数
    void PrintLog(const QString& log);
private:
    Ui::FileItem *ui;
};

#endif // FILEITEM_H
