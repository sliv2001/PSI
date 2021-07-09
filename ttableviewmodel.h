#ifndef TTABLEVIEWMODEL_H
#define TTABLEVIEWMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QList>
#include "tmediafile.h"

/**
 * @brief The TTableViewModel class represents values of a tab in ui.
 */
class TTableViewModel : public QAbstractListModel
{
public:
    /**
     * @brief TTableViewModel creates main class for program to work with.
     * @param parent is just a parent of an object.
     */
    TTableViewModel(QObject *parent = nullptr);

    ~TTableViewModel();

    /**
     * @brief rowCount shows total count of rows.
     * @return total count of rows.
     */
    int rowCount(const QModelIndex &) const;

    /**
     * @brief columnCount shows total count of columns.
     * @param parent is just an argument for inheritance.
     * @return total count of columns.
     */
    int columnCount(const QModelIndex &parent) const;

    /**
     * @brief data returns value of particular cell.
     * @param index is address of the cell.
     * @param role is type of data to get.
     * @return if role is Qt::DisplayRole, it returns QString, if Qt::UserRole, then QIcon, and "" in case of error.
     */
    QVariant data(const QModelIndex &index, int role) const;

    /**
     * @brief populate replaces data with new one.
     * @param newValues is pointer to new data.
     * @warning You had better use it only when initializing new model cause i don't know how exactly it works.
     */
    void populate(QList<TMediaFile> *newValues);

private:
    /**
     * @brief values are values of the table.
     */
    QList<TMediaFile> *values;
};

#endif // TTABLEVIEWMODEL_H
