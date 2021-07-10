#include "tcontext.h"

TContext::TContext()
{
    filters<<"*.jpg"<<"*.png";
    tabs = new QList<TTableViewModel*>();
}

TContext::~TContext()
{
    delete tabs;
}

void TContext::getFiles(QStringList* strs)
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
    }
}

void TContext::recursiveInit(QString rpath)
{
    QDir dir(rpath);

    QStringList listFiles = dir.entryList(filters, QDir::Files, QDir::NoSort);
    for (int i=0; i<listFiles.count(); i++)
        listFiles[i]=rpath+"/"+listFiles[i];
    getFiles(&listFiles);

    listFiles = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    foreach (QString path, listFiles){
        recursiveInit(rpath+"/"+path);
    }

}

QFuture<void> TContext::init(QString rpath)
{
    //recursiveInit(rpath);
    return QtConcurrent::run(this, &TContext::recursiveInit, rpath);
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
