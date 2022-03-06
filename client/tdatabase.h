#ifndef TDATABASE_H
#define TDATABASE_H

#include <QObject>
#include <tdatabaseentry.h>

class TDatabase : public QObject
{
    Q_OBJECT
public:
    explicit TDatabase(QObject *parent = nullptr);

signals:

};

#endif // TDATABASE_H
