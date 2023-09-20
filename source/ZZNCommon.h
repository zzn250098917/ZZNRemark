#ifndef ZZNCOMMON_H
#define ZZNCOMMON_H

#include <QObject>

class ZZNCommon : public QObject
{
    Q_OBJECT

public:
    enum SearchTreeIndex{
        SearchDefault = -1,
        SearchRemark = 0,
        SearchRemarkNo=1,
        SearchRemarkColor = 2
    };
    Q_ENUM(SearchTreeIndex);

public:
    explicit ZZNCommon(QObject *parent = nullptr);

    static QString getImageDefaultPath();
    static QStringList getRemarkInfo();
    static QStringList colorList();
    static QColor nameToColor(QString colorName);
    static QString searchInfoFromIndex(QString remarkInfo,SearchTreeIndex remarkInfoIndex,SearchTreeIndex toIndex);
    static void processDelayMSleep(unsigned int msec);
    static const QString getBuildDate();
};

#endif // ZZNCOMMON_H
