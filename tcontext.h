#ifndef TCONTEXT_H
#define TCONTEXT_H

#include <QList>
#include <QDir>
#include "ttableviewmodel.h"
#include "tmediafile.h"

class TContext
{
public:
    TContext();
    ~TContext();
    int rootpath;
    QStringList filters;
    void init(QString rootpath);
    TTableViewModel* tab(QString name);
    TTableViewModel* newTable(QString name);
    void getFiles(QStringList* str);

private:
    QList<TTableViewModel*> *tabs;
};

#endif // TCONTEXT_H
