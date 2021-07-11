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
    TTableViewModel(QString Name, QObject *parent = nullptr);

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
    QVariant data(const QModelIndex &index, int role) const override;

    /**
     * @brief populate replaces data with new one.
     * @param newValues is pointer to new data.
     * @warning You had better use it only when initializing new model cause i don't know how exactly it works.
     */
    void populate(QList<TMediaFile> *newValues);

    /**
     * @brief append adds new file to values.
     * @param file is new file to be added.
     */
    void append(TMediaFile file);

    /**
     * @brief update changes particular row.
     * @param idx is index of row changed.
     * @param file is new value of row.
     */
    void update(int idx, TMediaFile file);

    /**
     * @brief deleteRow does what it was made for.
     * @param idx is index to be deleted.
     */
    void deleteRow(int idx);

    /**
     * @brief insertAt adds a row with shift downwards.
     * @param idx is an index for data to be placed at.
     * @param file is value to be placed.
     */
    void insertAt(int idx, TMediaFile file);

    /**
     * @brief headerData serves for printing headers. It is overriding the same function in QAbstractModel (see F1 there).
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    /**
     * @brief name is corresponding year or tab name.
     */
    QString name;

    TMediaFile value(int index);
private:
    /**
     * @brief values are values of the table.
     */
    QList<TMediaFile> *values;
};

#endif // TTABLEVIEWMODEL_H
