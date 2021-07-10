#include "ttabledelegate.h"

TTableDelegate::TTableDelegate()
{

}

QSize TTableDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize s(100, 100);
    if (index.column()==0){
        QString str = index.model()->data(index, Qt::DisplayRole).toString();
        s = ((TTableViewModel*)index.model())->value(index.row()).getSize();
        if (s.width()>0&&s.height()>0){
            s.setHeight(ceil(s.height()/s.width()*100));
            s.setWidth(100);
        }
        return s;
    }

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
