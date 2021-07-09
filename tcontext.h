#ifndef TCONTEXT_H
#define TCONTEXT_H

#include <QList>
#include "ttableviewmodel.h"

class TContext
{
public:
    TContext();
    ~TContext();
private:
    QList<TTableViewModel*> *tabs;
};

#endif // TCONTEXT_H
