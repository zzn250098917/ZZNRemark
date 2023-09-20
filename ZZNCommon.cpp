#include "ZZNCommon.h"
#include "ZZNGlobal.h"
#include <QSettings>
#include <QTextCodec>
#include <QColor>
#include <QStringList>
#include <QTime>
#include <QCoreApplication>
#include <QLocale>

ZZNCommon::ZZNCommon(QObject *parent) : QObject(parent)
{

}

QString ZZNCommon::getImageDefaultPath()
{
    QSettings *settings = new QSettings(JSBZGlobal::g_AppConfigFileName, QSettings::IniFormat);
    settings->setIniCodec(QTextCodec::codecForName("GB2312"));
    QString defaultPath = "D:/";
    if(settings->contains("DefaultSetting/DefaultPath")){
        defaultPath = settings->value("DefaultSetting/DefaultPath").toString();
    }
    delete settings;
    return defaultPath;
}

QStringList ZZNCommon::getRemarkInfo()
{
    QSettings *settings = new QSettings(JSBZGlobal::g_AppConfigFileName, QSettings::IniFormat);
    settings->setIniCodec(QTextCodec::codecForName("GB2312"));
    QString settingInfo ="";
    if(settings->contains("RemarkSetting/RemarkInfo")){
        settingInfo = settings->value("RemarkSetting/RemarkInfo").toString().trimmed();
    }
    delete settings;
    if(settingInfo.length()<=5) settingInfo = "person 1 绿色";
    return settingInfo.split(",");
}

QStringList ZZNCommon::colorList()
{
    QStringList colorList;
    colorList<<"浅灰"<<"灰色"<<"深灰"<<"红色"<<"深红"<<"绿色"<<"深绿"<<"蓝色"<<"深蓝"<<"青色"<<"深青"<<"粉色"<<"紫色"<<"黄色"<<"深黄";
    return colorList;
}

QColor ZZNCommon::nameToColor(QString colorName)
{
    if(colorName == "浅灰") return Qt::lightGray;
    if(colorName == "灰色") return Qt::gray;
    if(colorName == "深灰") return Qt::darkGray;
    if(colorName == "红色") return Qt::red;
    if(colorName == "深红") return Qt::darkRed;
    if(colorName == "绿色") return Qt::green;
    if(colorName == "深绿") return Qt::darkGreen;
    if(colorName == "蓝色") return Qt::blue;
    if(colorName == "深蓝") return Qt::darkBlue;
    if(colorName == "青色") return Qt::cyan;
    if(colorName == "深青") return Qt::darkCyan;
    if(colorName == "粉色") return Qt::magenta;
    if(colorName == "紫色") return Qt::darkMagenta;
    if(colorName == "黄色") return Qt::yellow;
    if(colorName == "深黄") return Qt::darkYellow;
    return Qt::black;
}

QString ZZNCommon::searchInfoFromIndex(QString remarkInfo,SearchTreeIndex remarkInfoIndex,SearchTreeIndex toIndex)
{
    QString retStr = "";
    foreach (QString item, ZZNCommon::getRemarkInfo()) {
        QStringList itemList = item.split(" ");
        if(itemList.count()==3){
            if(itemList.at(remarkInfoIndex) == remarkInfo){
                retStr = itemList.at(toIndex);
                break;
            }
        }
    }
    return retStr;
}

void ZZNCommon::processDelayMSleep(unsigned int msec)
{
    QTime time = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < time )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

const QString ZZNCommon::getBuildDate()
{
    static const QDate buildDate = QLocale( QLocale::English ).toDate( QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
    return buildDate.toString("yyyy.MM.dd");
}


