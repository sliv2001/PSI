#include "ttabledelegate.h"

TTableDelegate::TTableDelegate()
{

}

QSize TTableDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QItemDelegate::sizeHint(option, index);
}

void TTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem myOption = option;

    if (index.column()==0){
        QString data = index.model()->data(index, Qt::DisplayRole).toString();

        myOption.displayAlignment = Qt::AlignCenter|Qt::AlignVCenter;

        QString StrIcon = index.model()->data(index, Qt::DisplayRole).toString();
        if (QFileInfo(StrIcon).exists()){
            QPixmap icon(StrIcon);
            painter->drawPixmap(myOption.rect.x(), myOption.rect.y(), 64, 64, icon);
        }
        else{
            qWarning("File %s doesnot exist.", (const char*)StrIcon.toLocal8Bit());
        }
    }
    else{
        drawDisplay(painter, option, option.rect, index.model()->data(index, Qt::DisplayRole).toString());
    }

    drawFocus(painter, myOption, myOption.rect);
}
