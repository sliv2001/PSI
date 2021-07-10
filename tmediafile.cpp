#include "tmediafile.h"

void TMediaFile::getLiveVideo(QString path)
{
    //TODO добавить конфиг файл с расширениями live
    if (QFileInfo(path).suffix()=="MOV"||QFileInfo(path).suffix()=="mov")
        this->live_path="";
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
    QString p = path;
    QFileInfo f(p);
    if (!f.isFile())
        qWarning("Path %s is not to a file!", path.toStdString().c_str());

    this->fullPath=p;
    this->quality=0;
    this->tags="";
    this->unique=1.0;
    this->year=this->getYear();
    getLiveVideo(path);
}

QIcon TMediaFile::getIcon() const
{
    return QIcon();
}

QSize TMediaFile::getSize() const
{
    int w, h;
    QSize s(0, 0);
    Exiv2::Image::UniquePtr img = Exiv2::ImageFactory::open((const char*)fullPath.toLocal8Bit());
    assert(img.get()!=0);
    img->readMetadata();

    w=img->pixelWidth();
    h=img->pixelHeight();
    if (w>0&&h>0){
        s.setHeight(h);
        s.setWidth(w);
    }


    return s;
}

int TMediaFile::getYear()
{
    if (!QFile(this->fullPath).exists()){
        qWarning("File %s does not exist", this->fullPath.toStdString().c_str());
        return -1;
    }
    QString date;
    try {
        Exiv2::Image::UniquePtr image;
        image = Exiv2::ImageFactory::open((const char*)this->fullPath.toLocal8Bit());
        assert(image.get() != 0);
        image->readMetadata();
        Exiv2::ExifData &data = image->exifData();

        date = QString::fromStdString(data["Exif.Photo.DateTimeOriginal"].toString());
        date = date.left(4);
    }  catch (Exiv2::AnyError &err) {
        qWarning("Couldnot read metadata \"Exif.Photo.DateTimeOriginal\" from file %s. Error: %s",
                 this->fullPath.toStdString().c_str(), err.what());
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
