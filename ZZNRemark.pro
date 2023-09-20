QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

RC_FILE = ZZNAppIco.rc

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DefaultPathDialog.cpp \
    SettingTypeDialog.cpp \
    ZZNCommon.cpp \
    ZZNGlobal.cpp \
    ZZNGraphicsScene.cpp \
    ZZNRemark.cpp \
    main.cpp

HEADERS += \
    DefaultPathDialog.h \
    SettingTypeDialog.h \
    ZZNCommon.h \
    ZZNGlobal.h \
    ZZNGraphicsScene.h \
    ZZNRemark.h

FORMS += \
    DefaultPathDialog.ui \
    SettingTypeDialog.ui \
    ZZNRemark.ui

#INCLUDEPATH += -I C:\Users\zzn\AppData\Local\Programs\Python\Python39\include
#LIBS += -LC:\Users\zzn\AppData\Local\Programs\Python\Python39\libs -lpython39

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc
