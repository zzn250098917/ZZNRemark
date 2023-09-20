#include "DefaultPathDialog.h"
#include "ui_DefaultPathDialog.h"
#include "ZZNGlobal.h"
#include <QSettings>
#include <QFileDialog>
#include "ZZNCommon.h"
#include <QTextCodec>

DefaultPathDialog::DefaultPathDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DefaultPathDialog)
{
    ui->setupUi(this);
    ui->editPath->setText(ZZNCommon::getImageDefaultPath());
}

DefaultPathDialog::~DefaultPathDialog()
{
    delete ui;
}

void DefaultPathDialog::on_btnBrowser_clicked()
{
    QString srcDirPath = QFileDialog::getExistingDirectory(this, "请选择图片默认路径",ui->editPath->text());
    if (srcDirPath.isEmpty()){
        return;
    }
    ui->editPath->setText(QString(srcDirPath += "/").replace("//","/"));
}

void DefaultPathDialog::on_btnConfirm_clicked()
{
    QSettings *settings = new QSettings(JSBZGlobal::g_AppConfigFileName, QSettings::IniFormat);
    settings->setIniCodec(QTextCodec::codecForName("GB2312"));
    settings->beginGroup("DefaultSetting");
    settings->setValue("DefaultPath",ui->editPath->text());
    settings->endGroup();
    delete settings;
    close();
}
