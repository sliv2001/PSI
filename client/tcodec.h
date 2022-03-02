#ifndef TCODEC_H
#define TCODEC_H

#include <QObject>
#include <QImage>
#include "libheif/heif_cxx.h"
#include "exiv2/exiv2.hpp"

class TCodec : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief TCodec is a main class to encode images with
     * @param quality is parameter passed to encoder
     * @param parent
     */
    explicit TCodec(uint8_t quality=50, QObject *parent = nullptr);

    ~TCodec();

    /**
     * @brief setImage adds new raw image to context without conversion to working colorspace
     * @param img is object of QImage, which must be of RGBA8888 or RGB888 format,
     * otherwise use setImage_convert() for your own risk
     * @return -1 if error encountered
     */
    int setImage(QImage img);

    /**
     * @brief transcode attempts to encode prepared data into .heic
     * and in case of success saves its representation in memory into context
     * @return -1 if error
     */
    int transcode();

    /**
     * @brief copyMetadata copies Exif, Xmp and Iptc data into instance of file in memory.
     * @param file is source file to get copy from
     * @return -1 if error
     */
    int copyMetadata(QString file);

    /**
     * @brief setTags erases old tags from image and sets new ones
     * @param tags are tags in format "tag1;tag2;"
     * @return -1 if error
     */
    int setTags(QString tags);

    /**
     * @brief getTranscodeResult
     * @return gives result of transcoding
     */
    QByteArray getTranscodeResult();

signals:

private:
    heif::Context *ctx;
    heif::Encoder *enc;
    heif::Image img;
    heif_chroma chroma;
    int bitDepth;
    QByteArray transcodeResult="";

    /**
     * @brief hasHeifAlpha checks whether this heif chroma has an alpha channel
     * @param chr
     * @return
     */
    bool hasHeifAlpha(enum heif_chroma chr);

    /**
     * @brief getHeifChroma transforms Qt-style format to heif-format, if possible
     * @param format
     * @return
     */
    heif_chroma getHeifChroma(QImage::Format format);

    int getBitDepth(int qt_img_depth);

    class writer: public heif::Context::Writer{
    public:
        QByteArray* r;
        writer(QByteArray* result)  { r = result;}
        heif_error write(const void* data, size_t size) override;
    };

};

#endif // TCODEC_H
