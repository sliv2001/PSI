#ifndef TCONTEXT_H
#define TCONTEXT_H

#include <QList>
#include <QDir>
#include <QtConcurrent>

#include "ttableviewmodel.h"
#include "tmediafile.h"
#include "tclient.h"

/**
 * @brief The TContext class is main class for application to work with. It represents structure of main window and contains models.
 */
class TContext : public QObject
{
    Q_OBJECT
public:

    explicit TContext(QObject *parent = nullptr);

    ~TContext();

    /**
     * @brief rootpath is path to dir which contains all photos program works with.
     * It though may (and must) be changed when add some photos from other dirs.
     */
    int rootpath;

    /**
     * @brief filters are list of strings imported from config, which represent image files.
     */
    QStringList imageFilters;

    QFuture<void> init(QString rootpath);
    int tabCount();
    TTableViewModel* tab(QString name);
    TTableViewModel* tabByIndex(int index);
    TTableViewModel* newTable(QString name);
    void getFiles(QPromise<void>* promise, int* value, QStringList* str);
    QFutureWatcher<void>* fileSystemScanWatcher;
    QFutureWatcher<void>* exportWatcher;
    QFutureWatcher<void>* recognizeWatcher;
    int getImageCount(QString rpath);

    bool isImage(TMediaFile* f);

    void recognize();

    QFuture<void> Export(QDir path);

    void updateModelWithResult(QVector<QByteArray> result);

private:
    QList<TTableViewModel*> *tabs;
    TClient* client;
    void recursiveInit(QPromise<void>* promise, int* value, QString rpath);
    void startRecursiveInit(QPromise<void> &promise, QString rpath);
    void recognizeWorker(QPromise<void> &promise);
    void exportWorker(QPromise<void> &promise, QDir path);

    int totalImageCount;

};

#endif // TCONTEXT_H
