#include "tcontext.h"

TContext::TContext()
{
    imageFilters<<"*.jpg"<<"*.png";
    tabs = new QList<TTableViewModel*>();
    watcher = nullptr;
    client = new TClient();
}

TContext::~TContext()
{
    delete tabs;
    if (watcher!=nullptr)
        delete watcher;
    delete client;
}

void TContext::getFiles(QPromise<void>* promise, int* value, QStringList* strs)
{
    foreach (QString path, *strs) {
        TMediaFile file(path);
        TTableViewModel* t = this->tab(QString::number(file.year));
        if (t!=nullptr){
            t->append(file);
        }
        else {
            t = newTable(QString::number(file.year));
            t->append(file);
        }
        if (promise!=nullptr&&value!=nullptr){
            promise->suspendIfRequested();
            *value+=1;
            promise->setProgressValue(*value);
        }
    }
}

int TContext::getImageCount(QString rpath)
{
    int result;
    QDir dir(rpath);
    dir.setFilter(QDir::Files);
    dir.setNameFilters(this->imageFilters);
    result=dir.count();

    dir.setFilter(QDir::NoFilter);
    QStringList s;
    s<<"*";
    dir.setNameFilters(s);
    s = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    foreach (QString path, s){
        result+=getImageCount(rpath+"/"+path);
    }
    return result;
}

bool TContext::isImage(TMediaFile *f)
{
    QString suffix = QFileInfo(f->fullPath).suffix();
    foreach(QString str, imageFilters){
        if (str.contains(suffix, Qt::CaseInsensitive))
            return 1;
    }
    return 0;
}

void TContext::recursiveInit(QPromise<void>* promise, int* value, QString rpath)
{
    QDir dir(rpath);

    QStringList listFiles = dir.entryList(imageFilters, QDir::Files, QDir::NoSort);
    for (int i=0; i<listFiles.count(); i++)
        listFiles[i]=rpath+"/"+listFiles[i];
    getFiles(promise, value, &listFiles);

    listFiles = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    foreach (QString path, listFiles){
        recursiveInit(promise, value, rpath+"/"+path);
    }

}

void TContext::startRecursiveInit(QPromise<void> &promise, QString rpath)
{
    int value=0;
    promise.setProgressRange(0, getImageCount(rpath));
    promise.setProgressValue(0);
    recursiveInit(&promise, &value, rpath);
}

QFuture<void> TContext::init(QString rpath)
{
    //recursiveInit(rpath);
    return QtConcurrent::run(&TContext::startRecursiveInit, this, rpath);
}

int TContext::tabCount()
{
    return tabs->count();
}

TTableViewModel* TContext::tab(QString name)
{
    foreach (TTableViewModel* t, *(this->tabs)){
        if (t->name == name){
            return t;
        }
    }
    return nullptr;
}

TTableViewModel* TContext::tabByIndex(int index)
{
    return (*tabs)[index];
}

TTableViewModel* TContext::newTable(QString name)
{
    TTableViewModel* m = new TTableViewModel(name);
    this->tabs->append(m);
    return m;
}
