#include "ttabledelegate.h"

TTableDelegate::TTableDelegate()
{

}

QSize TTableDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize s(200, 200);
    int w, h;
    if (index.column()==0){
        QString str = index.model()->data(index, Qt::DisplayRole).toString();
        Exiv2::Image::UniquePtr img = Exiv2::ImageFactory::open((const char*)str.toLocal8Bit());
        assert(img.get()!=0);
        img->readMetadata();

        Exiv2::ExifData &exif = img->exifData();
        if (!exif.empty()){
            w=img->pixelWidth();
            h=img->pixelHeight();
            if (w>0&&h>0){
                s.setHeight(ceil(h*200/w));
                return s;
            }

        }
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
