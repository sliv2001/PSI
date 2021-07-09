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
    return 0;
}

QVariant TTableViewModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    switch (role){
        case Qt::DisplayRole:{
            switch (index.column()) {
            case 1:{
                value = QString::number(this->values->at(index.row()).year); //year
                break;
            }
            case 2:{
                value = this->values->at(index.row()).tags; //tags
                break;
            }
            case 3:{
                value = QString::number(this->values->at(index.row()).unique); //unique
                break;
            }
            case 4:{
                value = QString::number(this->values->at(index.row()).quality); //quality
                break;
            }
            case 5:{
                value = this->values->at(index.row()).fullPath; //path
                break;
            }
            default:{
                value="";
            }
            }
        }
        break;
        case Qt::UserRole:{
            value = this->values->at(index.row()).getIcon();
        }
        break;

        default:
            value = "";
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
    this->values->append(file);
}
