#include "tcontext.h"

TTableViewModel* ModelBeingRecognized;
int IndexBeingRecognized;

TContext::TContext(QObject *parent)
{
    Q_UNUSED(parent);
    imageFilters<<"*.jpg"<<"*.png";
    tabs = new QList<TTableViewModel*>();
    fileSystemScanWatcher = nullptr;
    recognizeWatcher = new QFutureWatcher<void>(this);
    client = new TClient(this);
}

TContext::~TContext()
{
    recognizeWatcher->future().cancel();
    recognizeWatcher->future().waitForFinished();
    delete tabs;
    if (fileSystemScanWatcher!=nullptr)
        delete fileSystemScanWatcher;
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

void TContext::recognize()
{
    QFuture future = QtConcurrent::run(&TContext::recognizeWorker, this);
    recognizeWatcher->setFuture(future);

}

QFuture<void> TContext::Export(QDir path)
{
    QFuture future = QtConcurrent::run(&TContext::exportWorker, this, path);
    exportWatcher->setFuture(future);
    return future;
}

void TContext::updateModelWithResult(QVector<QByteArray> result)
{
    TTableViewModel* model=nullptr;
    int i=-1;
    if (ModelBeingRecognized!=nullptr
            &&result[0]==ModelBeingRecognized->value(IndexBeingRecognized).pictureCode){
        model=ModelBeingRecognized;
        i=IndexBeingRecognized;
    } else
        foreach(TTableViewModel* m, *(this->tabs)){
            for (int iter=0; iter<m->rowCount(QModelIndex()); iter++){
                if (m->value(iter).pictureCode==result[0]){
                    model=m;
                    i=iter;
                    break;
                }
            }
        }
    if (model==nullptr||i==-1)
        return;
    TMediaFile f=model->value(i);
    f.tags=f.concatTags(result);
    f.updatePropertiesWithResponse(result);
    model->update(i, f);
}

void TContext::recognizeWorker(QPromise<void> &promise)
{
    foreach(TTableViewModel* model, *(this->tabs)){
        int iter=0;
        for (int i=0; i<model->rowCount(QModelIndex()); i++){
            if (model->value(i).tags==""){
                TMediaFile f=model->value(i);
                if (client->connected){
                    this->client->sendFile(model->value(i));
                    ModelBeingRecognized=model;
                    IndexBeingRecognized=i;
                    f.tags = "In Progress";
                    model->update(i, f);
                    iter++;
                    if ((iter)%1==0){
                        int k=client->imgsProcessed;
                        while(client->imgsProcessed-k<1)
                            QThread::msleep(100);
                    }
                }
                if (promise.isCanceled())
                    return;
            }
        }
    }
}

QString TContext::prepareFileToWriting(QString path, int i)
{
    if (i==0){
        if (!QFileInfo::exists(path))
            return path;
        else {
            i++;
            return prepareFileToWriting(path, i);
        }
    }
    else {
        QString newPath = QFileInfo(path).absolutePath()+QFileInfo(path).baseName()+" ("+
                QString::number(i)+")"+QFileInfo(path).suffix();
        if (!QFileInfo::exists(newPath))
            return newPath;
        else {
            i++;
            return prepareFileToWriting(path, i);
        }
    }
}

void TContext::exportWorker(QPromise<void> &promise, QDir path)
{
    promise.setProgressRange(0, totalImageCount);
    int progressValue = 0;
    foreach(TTableViewModel* model, *(this->tabs)){
        if (!QDir(path.absolutePath()+"/"+model->name).exists())
            if (model->name==""||!path.mkdir(model->name)){
                promise.setException(QException());
                return;
            }
        path.cd(model->name);
        for (int i=0; i<model->rowCount(QModelIndex()); i++){
            //model->value(i).move(path.absolutePath()+"/"+QFileInfo(model->value(i).fullPath).fileName());

            QFile f(prepareFileToWriting(path.absolutePath()+"/"
                                         +QFileInfo(model->value(i).fullPath).baseName()+".heic", 0), this);
            f.open(QIODeviceBase::WriteOnly);
            f.write(model->value(i).encodeImage());
            f.close();
            progressValue++;
            promise.setProgressValue(progressValue);
        }
        path.cdUp();
    }
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
