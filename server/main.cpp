#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QHostAddress>
#include <QLockFile>
#include <QDir>

#include "tserver.h"

bool checkWorkingInstance(){
    QLockFile lockFile(QDir::temp().absoluteFilePath("lurity.lock"));

    if(!lockFile.tryLock(100)){
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (checkWorkingInstance()){
        qInfo("Couldnot take a lock. Running instance already exists?");
        return 0;
    }

    QCoreApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "server_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    TServer* server;

    if (a.arguments().count()>1){
        if (!QHostAddress(a.arguments()[1]).isNull()){
            server = new TServer(a.arguments()[1]);
            return a.exec();
        }

    }
    server = new TServer();
    if (!server->listening)
        return 0;
    return a.exec();
}
