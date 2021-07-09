#include "tmediafile.h"

void TMediaFile::getLiveVideo(QString path)
{
    //TODO добавить конфиг файл с расширениями live
    QFileInfo fi = QFileInfo(path);
    QFileInfo fo(fi.completeBaseName()+".MOV");
    if (fo.exists()){
        this->live_path=fo.absoluteFilePath();
        return;
    }

    fo = QFileInfo(fi.completeBaseName()+".mov");
    if (fo.exists())
        this->live_path=fo.absoluteFilePath();
    else
        this->live_path = "";
}

TMediaFile::TMediaFile(QString path)
{
    this->fullPath=path;
    this->quality=0;
    this->tags="";
    this->unique=1.0;
    this->year=this->getYear();
    getLiveVideo(path);
}

int TMediaFile::getYear()
{
    if (!QFile(this->fullPath).exists()){
        qWarning("File %s does not exist", this->fullPath.toStdString().c_str());
        return -1;
    }
    QString date;
    try {
        Exiv2::Image::AutoPtr image;
        std::string s = this->fullPath.toStdString();
        image = Exiv2::ImageFactory::open(s, false);
        assert(image.get() != 0);
        image->readMetadata();
        Exiv2::ExifData &data = image->exifData();

        date = QString::fromStdString(data["Exif.Photo.DateTimeOriginal"].toString());
        date = date.left(4);
    }  catch (Exiv2::AnyError &err) {
        qWarning("Couldnot read metadata \"Exif.Photo.DateTimeOriginal\" from file %s.",
                 this->fullPath.toStdString().c_str());
        return 0;
    }

    bool ok=false;
    int year = date.toInt(&ok);
    if (!ok){
#ifndef TESTING
        qWarning("Couldnot get year of photo %s", this->fullPath.toStdString().c_str());
#endif
        return 0;
    }
    if (year<1500||year>2500){
        qWarning("Year of photo %s is out of range", this->fullPath.toStdString().c_str());
        return 0;
    }
    return year;

}
