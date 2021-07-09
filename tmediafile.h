#ifndef TMEDIAFILE_H
#define TMEDIAFILE_H

#include <QString>
#include <QFileInfo>
#include <exiv2/exiv2.hpp>

class TMediaFile
{
public:
    TMediaFile();
    TMediaFile(QString path);

private:
    int getYear();

    QString fullPath;
    int year;
    int quality;
    float unique;
    QString tags;
    QString live_path;
    void getLiveVideo(QString path);
};

#endif // TMEDIAFILE_H
