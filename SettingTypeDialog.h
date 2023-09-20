#ifndef SETTINGTYPEDIALOG_H
#define SETTINGTYPEDIALOG_H

#include <QDialog>
class QTreeWidgetItem;
namespace Ui {
class SettingTypeDialog;
}

class SettingTypeDialog : public QDialog
{
    Q_OBJECT

public:
    enum InfomationType{
        InfomationSuccess,
        RemarkNameEmpty,
        RemarkNoEmpty,
        RemarkNameRedefine,
        RemarkNoRedefine,
        RemarkInfoError
    };
    Q_ENUM(InfomationType);

private:
    QStringList m_settingInfo;
    int m_selectRow;
    void initTreeWidget();
    void initColorList();
    void createTreeItem(QStringList listInfo);
    InfomationType infomationType();
    QString getInfoContent(InfomationType infoType);
    void clearNameAndNo();

protected:
    void closeEvent(QCloseEvent *event);

public:
    explicit SettingTypeDialog(QWidget *parent = nullptr);
    ~SettingTypeDialog();

private slots:
    void on_treeWidget_clicked(const QModelIndex &index);
    void on_btnAdd_clicked();
    void on_btnUpdate_clicked();
    void on_btnDelete_clicked();

private:
    Ui::SettingTypeDialog *ui;
};

#endif // SETTINGTYPEDIALOG_H
