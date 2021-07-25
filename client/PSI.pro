QT       += core gui network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG -= console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    tclient.cpp \
    tcontext.cpp \
    tmediafile.cpp \
    tserverresponce.cpp \
    ttabledelegate.cpp \
    ttableviewmodel.cpp

HEADERS += \
    mainwindow.h \
    tclient.h \
    tcontext.h \
    tmediafile.h \
    tserverresponce.h \
    ttabledelegate.h \
    ttableviewmodel.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    PSI_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    TODO

win32: LIBS += -L$$PWD/'../../../../Program Files (x86)/exiv2/lib/' -llibexiv2.dll

INCLUDEPATH += $$PWD/'../../../../Program Files (x86)/exiv2/include'
DEPENDPATH += $$PWD/'../../../../Program Files (x86)/exiv2/include'
