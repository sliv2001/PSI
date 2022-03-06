QT -= gui
QT += widgets

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/../submodules/exiv2/build/lib/ -llibexiv2.dll \
    -L$$PWD/../submodules/exiv2/build/lib/ -lexiv2-xmp \
    -L$$PWD/../submodules/exiv2/build/lib/ -lwmain \
    -L$$PWD/../submodules/libheif/libheif/.libs/ -llibheif.dll

INCLUDEPATH += $$PWD/../submodules/exiv2/include \
    $$PWD/../submodules/exiv2/build \
    $$PWD/test \
    $$PWD/../submodules/libheif

DEPENDPATH += $$PWD/../submodules/exiv2/include \
    $$PWD/../submodules/exiv2/build \
    $$PWD/../submodules/libheif
