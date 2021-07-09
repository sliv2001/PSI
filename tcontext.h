#ifndef TCONTEXT_H
#define TCONTEXT_H

#include <QList>
#include <QDir>
#include "ttableviewmodel.h"
#include "tmediafile.h"

/**
 * @brief The TContext class is main class for application to work with. It represents structure of main window and contains models.
 */
class TContext
{
public:

    TContext();

    ~TContext();

    /**
     * @brief rootpath is path to dir which contains all photos program works with.
     * It though may (and must) be changed when add some photos from other dirs.
     */
    int rootpath;

    /**
     * @brief filters are list of strings imported from config, which represent image files.
     */
    QStringList filters;

    void init(QString rootpath);
    int tabCount();
    TTableViewModel* tab(QString name);
    TTableViewModel* tabByIndex(int index);
    TTableViewModel* newTable(QString name);
    void getFiles(QStringList* str);

private:
    QList<TTableViewModel*> *tabs;
};

#endif // TCONTEXT_H
