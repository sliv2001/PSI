#include "tcontext.h"

TContext::TContext()
{
    tabs = new QList<TTableViewModel*>();
}

TContext::~TContext()
{
    delete tabs;
}
