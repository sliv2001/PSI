#include "ttableviewmodel.h"

TTableViewModel::TTableViewModel(QString Name, QObject *parent)
{
    Q_UNUSED(parent);
    values = new QList<TMediaFile>();
    this->name = Name;
}

TTableViewModel::~TTableViewModel()
{
    delete values;
}

int TTableViewModel::rowCount(const QModelIndex &) const
{
    return values->count();
}

int TTableViewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7;
}

QVariant TTableViewModel::data(const QModelIndex &index, int role) const
{
    QVariant value;

    switch (role){
        case Qt::DisplayRole:{
            switch (index.column()) {
            case 0:{
                value = this->values->at(index.row()).fullPath;
                break;
            }
            case 1:{
                value = QFileInfo(this->values->at(index.row()).fullPath).fileName();
                break;
            }
            case 2:{
                value = QString::number(this->values->at(index.row()).year); //year
                break;
            }
            case 3:{
                value = this->values->at(index.row()).tags; //tags
                if (value.toString()=="")
                    value = "Scan pending";
                break;
            }
            case 4:{
                value = QString::number(this->values->at(index.row()).unique); //unique
                break;
            }
            case 5:{
                value = QString::number(this->values->at(index.row()).quality); //quality
                break;
            }
            case 6:{
                value = this->values->at(index.row()).fullPath; //path
                break;
            }
            default:{
                break;
            }
            }
            break;
        }
        case Qt::UserRole:{
            value = this->values->at(index.row()).getIcon();
            break;
        }
//        case Qt::DecorationRole:{
//            if (index.column()==0){
//                QIcon pixmap(this->values->at(index.row()).fullPath);
//                if (!pixmap.isNull()){

//                    return pixmap.pixmap(400);
//                }
//                else{
//                    qWarning(
//                            "Couldnot read %s thumbnail",
//                            (const char*)(this->values->at(index.row()).fullPath.toLocal8Bit())
//                    );
//                    return value;
//                }
//            }
//        }
        default:
            break;
        }

    return value;
}

void TTableViewModel::populate(QList<TMediaFile> *newValues)
{
    int idx = this->values->count();
    this->beginInsertRows(QModelIndex(), 1, idx);
    this->values = newValues;
    endInsertRows();
}

void TTableViewModel::append(TMediaFile file)
{
    int newRow = this->values->count()+1;
    beginInsertRows(QModelIndex(), newRow, newRow);
    values->append(file);
    endInsertRows();
}

void TTableViewModel::update(int idx, TMediaFile file)
{
    (*this->values)[idx] = file;

    QModelIndex s = index(idx, 0);
    QModelIndex e = index(idx, columnCount(QModelIndex()));
    emit dataChanged(s, e);
}

void TTableViewModel::deleteRow(int idx)
{
    beginRemoveRows(QModelIndex(), idx, idx);
    values->removeAt(idx);
    endRemoveRows();
}

QVariant TTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role ==Qt::DisplayRole && orientation==Qt::Horizontal){
        switch (section){
        case 0:
            return QVariant();
        case 1:
            return QString("Имя файла");
        case 2:
            return QString("Год съёмки");
        case 3:
            return QString("Теги");
        case 4:
            return QString("Уникальность");
        case 5:
            return QString("Качество");
        case 6:
            return QString("Полный путь к файлу");
        }
    }

    if (role==Qt::DisplayRole)
        return QVariant(section+1);
    return QVariant();
}

TMediaFile TTableViewModel::value(int index)
{
    return values->at(index);
}
