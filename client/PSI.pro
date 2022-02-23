QT       += core gui network concurrent testlib

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

win32: LIBS += -L$$PWD/../lib/ -llibexiv2.dll

INCLUDEPATH += $$PWD/../submodules/exiv2/include \
    $$PWD/../lib/include \
    $$PWD/test

DEPENDPATH += $$PWD/../submodules/exiv2/include \
    $$PWD/../lib/include \
    $$PWD/test

win32: LIBS += -L$$PWD/../lib/ -lexiv2-xmp

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/exiv2-xmp.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/libexiv2-xmp.a

win32: LIBS += -L$$PWD/../lib/ -lwmain

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/wmain.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/libwmain.a
