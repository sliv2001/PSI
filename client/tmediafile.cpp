#include <QCryptographicHash>
#include "tmediafile.h"

int g_count_ofpixmaps=0;
int g_maxcount_ofpixmaps=0; /*Не использовать сохранение pixmap в памяти*/

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

void TMediaFile::getHashCode()
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(this->fullPath.toUtf8());
    this->pictureCode=hash.result();
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
    this->year=this->getYear_setTags();
    getLiveVideo(path);
    resolution=QSize(0, 0);
    resolution=getResolution();
    getHashCode();

    if (g_count_ofpixmaps<g_maxcount_ofpixmaps){
        this->picture = new QPixmap(this->fullPath);
        g_count_ofpixmaps++;
    }
    else {
        this->picture = NULL;
    }
}

QIcon TMediaFile::getIcon() const
{
    return QIcon();
}

QSize TMediaFile::getResolution() const
{
    QSize s(0, 0);
    if (resolution.width()==0||resolution.height()==0){
        int w, h;

        Exiv2::Image::UniquePtr img = Exiv2::ImageFactory::open((const char*)fullPath.toLocal8Bit());
        assert(img.get()!=0);
        img->readMetadata();

        w=img->pixelWidth();
        h=img->pixelHeight();
        if (w>0&&h>0){
            s.setHeight(h);
            s.setWidth(w);
        }
    }
    else{
        return this->resolution;
    }

    return s;
}

bool TMediaFile::move(QString path)
{
    QFile f(this->fullPath);
    if (!f.rename(path)){
        if (QFileInfo(path).exists()){
            if (!move(path+" (1)"))
                return false;
        }
        else {
            return false;
        }
    }
    this->fullPath=path;
    return true;
}

int TMediaFile::getYear_setTags()
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

        if (data["Exif.Image.XPKeywords"].size()>0){
            char s[4096];
            data["Exif.Image.XPKeywords"].copy((Exiv2::byte*)&s, Exiv2::ByteOrder::littleEndian);
            tags=QString::fromUtf16((const char16_t*)&s);
        }

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

enum heif_colorspace getHeifColorSpace(QImage::Format format){
    switch (format){
    case QImage::Format_Invalid:                    return heif_colorspace_undefined;
    case QImage::Format_Mono:                       return heif_colorspace_monochrome;
    case QImage::Format_MonoLSB:                    return heif_colorspace_monochrome;
    case QImage::Format_Indexed8:                   return heif_colorspace_undefined;
    case QImage::Format_RGB32:                      return heif_colorspace_RGB;
    case QImage::Format_ARGB32:                     return heif_colorspace_RGB;
    case QImage::Format_ARGB32_Premultiplied:       return heif_colorspace_RGB;
    case QImage::Format_RGB16:                      return heif_colorspace_RGB;
    case QImage::Format_ARGB8565_Premultiplied:     return heif_colorspace_RGB;
    case QImage::Format_RGB666:                     return heif_colorspace_RGB;
    case QImage::Format_ARGB6666_Premultiplied:     return heif_colorspace_RGB;
    case QImage::Format_RGB555:                     return heif_colorspace_RGB;
    case QImage::Format_ARGB8555_Premultiplied:     return heif_colorspace_RGB;
    case QImage::Format_RGB888:                     return heif_colorspace_RGB;
    case QImage::Format_RGB444:                     return heif_colorspace_RGB;
    case QImage::Format_ARGB4444_Premultiplied:     return heif_colorspace_RGB;
    case QImage::Format_RGBX8888:                   return heif_colorspace_RGB;
    case QImage::Format_RGBA8888:                   return heif_colorspace_RGB;
    case QImage::Format_RGBA8888_Premultiplied:     return heif_colorspace_RGB;
    case QImage::Format_BGR30:                      return heif_colorspace_undefined;
    case QImage::Format_A2BGR30_Premultiplied:      return heif_colorspace_undefined;
    case QImage::Format_RGB30:                      return heif_colorspace_RGB;
    case QImage::Format_A2RGB30_Premultiplied:      return heif_colorspace_undefined;
    case QImage::Format_Alpha8:                     return heif_colorspace_monochrome;
    case QImage::Format_Grayscale8:                 return heif_colorspace_monochrome;
    case QImage::Format_Grayscale16:                return heif_colorspace_monochrome;
    case QImage::Format_RGBX64:                     return heif_colorspace_RGB;
    case QImage::Format_RGBA64:                     return heif_colorspace_RGB;
    case QImage::Format_RGBA64_Premultiplied:       return heif_colorspace_RGB;
    case QImage::Format_BGR888:                     return heif_colorspace_undefined;
    case QImage::Format_RGBX16FPx4:                 return heif_colorspace_RGB;
    case QImage::Format_RGBA16FPx4:                 return heif_colorspace_RGB;
    case QImage::Format_RGBA16FPx4_Premultiplied:   return heif_colorspace_RGB;
    case QImage::Format_RGBX32FPx4:                 return heif_colorspace_RGB;
    case QImage::Format_RGBA32FPx4:                 return heif_colorspace_RGB;
    case QImage::Format_RGBA32FPx4_Premultiplied:   return heif_colorspace_RGB;
    default: return heif_colorspace_undefined;
    }
}

enum heif_chroma getHeifChroma(QImage::Format format){
    switch (format){
    case QImage::Format_Invalid:                    return heif_chroma_undefined;
    case QImage::Format_Mono:                       return heif_chroma_monochrome;
    case QImage::Format_MonoLSB:                    return heif_chroma_undefined;
    case QImage::Format_Indexed8:                   return heif_chroma_undefined;
    case QImage::Format_RGB32:                      return heif_chroma_interleaved_32bit;
    case QImage::Format_ARGB32:                     return heif_chroma_undefined;
    case QImage::Format_ARGB32_Premultiplied:       return heif_chroma_undefined;
    case QImage::Format_RGB16:                      return heif_chroma_undefined;
    case QImage::Format_ARGB8565_Premultiplied:     return heif_chroma_undefined;
    case QImage::Format_RGB666:                     return heif_chroma_undefined;
    case QImage::Format_ARGB6666_Premultiplied:     return heif_chroma_undefined;
    case QImage::Format_RGB555:                     return heif_chroma_undefined;
    case QImage::Format_ARGB8555_Premultiplied:     return heif_chroma_undefined;
    case QImage::Format_RGB888:                     return heif_chroma_interleaved_RGB;
    case QImage::Format_RGB444:                     return heif_chroma_444;
    case QImage::Format_ARGB4444_Premultiplied:     return heif_chroma_undefined;
    case QImage::Format_RGBX8888:                   return heif_chroma_interleaved_RRGGBBAA_BE;
    case QImage::Format_RGBA8888:                   return heif_chroma_interleaved_RRGGBBAA_BE;
    case QImage::Format_RGBA8888_Premultiplied:     return heif_chroma_interleaved_RRGGBBAA_BE;
    case QImage::Format_BGR30:                      return heif_chroma_undefined;
    case QImage::Format_A2BGR30_Premultiplied:      return heif_chroma_undefined;
    case QImage::Format_RGB30:                      return heif_chroma_undefined;
    case QImage::Format_A2RGB30_Premultiplied:      return heif_chroma_undefined;
    case QImage::Format_Alpha8:                     return heif_chroma_monochrome;
    case QImage::Format_Grayscale8:                 return heif_chroma_monochrome;
    case QImage::Format_Grayscale16:                return heif_chroma_monochrome;
    case QImage::Format_RGBX64:                     return heif_chroma_undefined;
    case QImage::Format_RGBA64:                     return heif_chroma_undefined;
    case QImage::Format_RGBA64_Premultiplied:       return heif_chroma_undefined;
    case QImage::Format_BGR888:                     return heif_chroma_undefined;
    case QImage::Format_RGBX16FPx4:                 return heif_chroma_undefined;
    case QImage::Format_RGBA16FPx4:                 return heif_chroma_undefined;
    case QImage::Format_RGBA16FPx4_Premultiplied:   return heif_chroma_undefined;
    case QImage::Format_RGBX32FPx4:                 return heif_chroma_undefined;
    case QImage::Format_RGBA32FPx4:                 return heif_chroma_undefined;
    case QImage::Format_RGBA32FPx4_Premultiplied:   return heif_chroma_undefined;
    default: return heif_chroma_undefined;
    }
}

bool isHeifInterleaved(enum heif_chroma chr){
    switch (chr){
    case heif_chroma_420:
    case heif_chroma_422:
    case heif_chroma_444:
    case heif_chroma_monochrome:
    case heif_chroma_undefined: return 0;
    default: return 1;
    }
}

QByteArray TMediaFile::encodeImage()
{
    QByteArray res;
    heif::Context ctx;
    heif::Encoder ecd(heif_compression_HEVC);
    heif::Image heif_img;
    QImage qt_img(this->fullPath);

    enum heif_chroma heif_chr;
    enum heif_colorspace heif_sps;
    if ((heif_chr=getHeifChroma(qt_img.format()))==heif_chroma_undefined||(heif_sps=getHeifColorSpace(qt_img.format()))){
        qt_img.save(res, "JPEG");
        this->way_of_coding=coded_as_JPEG;
        return res;
    }

    QByteArray red, green, blue, alpha;
    heif_img.create(qt_img.width(), qt_img.height(), heif_sps, heif_chr);
    if (isHeifInterleaved(heif_chr)){
        heif_img.add_plane(heif_channel_interleaved, qt_img.width(), qt_img.height(), qt_img.depth());
    }

    red.setRawData()
    for (int i=0; i<qt_img.height(); i++){

    }
}

QString TMediaFile::concatTags(QVector<QByteArray> array){
    QStringList list;
    QString res="";
    if (array.length()==1)
        return "'NOTAG'";
    int m=(array.length()-1)/3;
    for (int i=0; i<m; i++){
        list.append(array[3*i+2]);
    }
    list.removeDuplicates();
    res=list.join(';');
    res.prepend("\'");
    res.append("\'");
    return res;
}

void TMediaFile::updatePropertiesWithResponse(QVector<QByteArray> array)
{
    if (array.length()==1){
        this->rawDetectionBoxes.clear();
        this->rawDetectionClassEntities.clear();
        this->rawDetectionScores.clear();
        return;
    }
    int m=(array.length()-1)/3;
    for (int i=0; i<m; i++){
        this->rawDetectionBoxes.append(array[3*i+1]);
        this->rawDetectionClassEntities.append(array[3*i+2]);
        this->rawDetectionScores.append(array[3*i+3]);
    }
}
