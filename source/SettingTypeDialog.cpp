#include "SettingTypeDialog.h"
#include "ui_SettingTypeDialog.h"
#include <QTreeWidgetItem>
#include <QDebug>
#include "ZZNCommon.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include "ZZNGlobal.h"
#include <QTextCodec>

SettingTypeDialog::SettingTypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingTypeDialog)
{
    ui->setupUi(this);
    initTreeWidget();
    initColorList();
}

SettingTypeDialog::~SettingTypeDialog()
{
    delete ui;
}

void SettingTypeDialog::on_treeWidget_clicked(const QModelIndex &index)
{
    m_selectRow = index.row();
    QStringList itemInfo = m_settingInfo.at(index.row()).split(" ");
    if(itemInfo.count() ==3){
        ui->lineEditName->setText(itemInfo.at(0));
        ui->lineEditNo->setText(itemInfo.at(1));
        ui->comboBoxColor->setCurrentText(itemInfo.at(2));
    }else{
        QMessageBox::question(this,"配制信息错误","配置文件格式错误，请检查配制文件信息",QMessageBox::Ok);
    }
}

void SettingTypeDialog::initTreeWidget()
{
    QStringList strList;
    m_selectRow = -1;
    strList<<"名称"<<"编号"<<"颜色";
    ui->treeWidget->setHeaderLabels(strList);
    ui->treeWidget->setRootIsDecorated(false);
    m_settingInfo = ZZNCommon::getRemarkInfo();
    createTreeItem(m_settingInfo);
}

void SettingTypeDialog::initColorList()
{
    ui->comboBoxColor->addItems(ZZNCommon::colorList());
}

void SettingTypeDialog::createTreeItem(QStringList listInfo)
{
    if(listInfo.at(0) !="" || !listInfo.empty()){
        foreach (QString infoItem, listInfo) {
            QStringList itemList = infoItem.split(" ");
            if(itemList.count()==3){
                QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);
                treeItem->setText(0,itemList.at(0));
                treeItem->setText(1,itemList.at(1));
                treeItem->setText(2,itemList.at(2));
                treeItem->setBackground(2,ZZNCommon::nameToColor(itemList.at(2)));
                ui->treeWidget->addTopLevelItem(treeItem);
            }else{
                QMessageBox::question(this,"配制信息错误","配置文件格式错误，请检查配制文件信息",QMessageBox::Ok);
                break;
            }
        }
    }
}

void SettingTypeDialog::on_btnAdd_clicked()
{
    if(InfomationSuccess != infomationType()){
        QMessageBox::information(this,"添加提示",getInfoContent(infomationType()),QMessageBox::Ok);
        return;
    }
    ui->treeWidget->clear();
    QString item = ui->lineEditName->text().trimmed()+" "+ui->lineEditNo->text()+" "+ui->comboBoxColor->currentText();
    m_settingInfo.append(item);
    createTreeItem(m_settingInfo);
    clearNameAndNo();
}

void SettingTypeDialog::on_btnUpdate_clicked()
{
    if(m_selectRow == -1){
        QMessageBox::information(this,"更新提示","请选择你要更新的列！",QMessageBox::Ok);
        return;
    }
    QString itemTemp = m_settingInfo.at(m_selectRow);
    m_settingInfo.removeAt(m_selectRow);
    if(InfomationSuccess != infomationType()){
        m_settingInfo.insert(m_selectRow,itemTemp);
        QMessageBox::information(this,"更新提示",getInfoContent(infomationType()),QMessageBox::Ok);
        return;
    }

    ui->treeWidget->clear();
    QString item = ui->lineEditName->text().trimmed()+" "+ui->lineEditNo->text()+" "+ui->comboBoxColor->currentText();
    m_settingInfo.insert(m_selectRow,item);
    createTreeItem(m_settingInfo);
    clearNameAndNo();
}

void SettingTypeDialog::on_btnDelete_clicked()
{
    if(m_selectRow == -1){
        QMessageBox::information(this,"删除提示","请选择你要删除的列！",QMessageBox::Ok);
        return;
    }
    ui->treeWidget->clear();
    m_settingInfo.removeAt(m_selectRow);
    if(m_settingInfo.empty()){
        QMessageBox::information(this,"删除提示","列表已清空完毕！",QMessageBox::Ok);
    }else{
        createTreeItem(m_settingInfo);
    }
    clearNameAndNo();
}

SettingTypeDialog::InfomationType SettingTypeDialog::infomationType()
{
    if(ui->lineEditName->text().trimmed().length() <=0) return RemarkNameEmpty;
    if(ui->lineEditNo->text().trimmed().length() <=0) return RemarkNoEmpty;
    InfomationType infoType = InfomationSuccess;
    foreach (QString info, m_settingInfo) {
        QStringList item = info.split(" ");
        if(item.count() == 3){
            if(ui->lineEditName->text() == item.at(0)){
                infoType = RemarkNameRedefine;
                break;
            }
            if(ui->lineEditNo->text() == item.at(1)){
                infoType = RemarkNoRedefine;
                break;
            }
        }
    }
    return infoType;
}

QString SettingTypeDialog::getInfoContent(InfomationType infoType)
{
    if(infoType == RemarkNameEmpty) return "标注名称不能为空！";
    if(infoType == RemarkNoEmpty) return "标注编号不能为空!";
    if(infoType == RemarkNameRedefine) return "标注名称与列表中的名称重复，请重新输入！";
    if(infoType == RemarkNoRedefine) return "标注编号与列表中的编号重复，请重新输入！";
    return "程序代码逻辑有误！请联系13534189523更新！";
}

void SettingTypeDialog::clearNameAndNo()
{
    ui->lineEditName->clear();
    ui->lineEditNo->clear();
    ui->comboBoxColor->setCurrentIndex(0);
    m_selectRow = -1;
}

void SettingTypeDialog::closeEvent(QCloseEvent *event)
{
    QString settingInfo = "";
    foreach (QString itemStr, m_settingInfo) {
        settingInfo +=itemStr+",";
    }

    QSettings *settings = new QSettings(JSBZGlobal::g_AppConfigFileName, QSettings::IniFormat);
    settings->setIniCodec(QTextCodec::codecForName("GB2312"));
    settings->beginGroup("RemarkSetting");
    settings->setValue("RemarkInfo",settingInfo.left(settingInfo.length()-1));
    settings->endGroup();
    delete settings;

    event->accept();
}
