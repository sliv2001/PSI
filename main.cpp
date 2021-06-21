#include "mainwindow.h"
#include "test_mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTest>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "PSI_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
#ifndef TESTING
    MainWindow w;
    w.show();
    return a.exec();
#else
    freopen("testing.log", "w", stdout);
    QTest::qExec(new Test_mainwindow, argc, argv);
#endif
}
