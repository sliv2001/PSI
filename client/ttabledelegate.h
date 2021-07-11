#ifndef TTABLEDELEGATE_H
#define TTABLEDELEGATE_H

#include <QItemDelegate>
#include <QFileInfo>
#include <QPainter>
#include <QPicture>
#include <math.h>
#include "exiv2/exiv2.hpp"
#include "ttableviewmodel.h"
class TTableDelegate : public QItemDelegate
{
public:
    TTableDelegate();
    /**
     * @brief sizeHint inherits from QAbstractDelegate
     * @param option
     * @param index
     * @return
     */
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // TTABLEDELEGATE_H
