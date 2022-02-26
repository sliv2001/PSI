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
    ttabledelegate.cpp \
    ttableviewmodel.cpp

HEADERS += \
    mainwindow.h \
    tclient.h \
    tcontext.h \
    tmediafile.h \
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

DEFINES += NET_RECOGNITION

unix|win32: LIBS += -L$$PWD/../submodules/exiv2/build/lib/ -llibexiv2.dll
unix|win32: LIBS += -L$$PWD/../submodules/exiv2/build/lib/ -lexiv2-xmp
unix|win32: LIBS += -L$$PWD/../submodules/exiv2/build/lib/ -lwmain

INCLUDEPATH += $$PWD/../submodules/exiv2/include \
    $$PWD/../submodules/exiv2/build \
    $$PWD/test

DEPENDPATH += $$PWD/../submodules/exiv2/include \
    $$PWD/../submodules/exiv2/build
