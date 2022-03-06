#ifndef TDATABASEENTRY_H
#define TDATABASEENTRY_H

#include <QObject>
#include <exiv2/exiv2.hpp>

class TDataBaseEntry : public QObject
{
    Q_OBJECT
public:
    explicit TDataBaseEntry(QObject *parent = nullptr);

signals:

};

#endif // TDATABASEENTRY_H
