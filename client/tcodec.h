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
    enum coded_as {
        coded_as_undefined,
        coded_as_RAW,
        coded_as_JPEG,
        coded_as_HEIC
    };

    enum supportedColorspaces {
        RGB888 = QImage::Format_RGB888,
        RGBA8888 = QImage::Format_RGBA8888,
        RGB32 = QImage::Format_RGB32
    };

    /**
     * @brief TCodec is a main class to encode images with
     * @param quality is parameter passed to encoder
     * @param parent
     */
    explicit TCodec(uint8_t quality=50, QObject *parent = nullptr);

    ~TCodec();

    /**
     * @brief setImage_RGB888_RGBA8888 adds new raw image to context without conversion to working colorspace
     * @param img is object of QImage, which must be of RGBA8888 or RGB888 format,
     * otherwise use setImage_convert() at your own risk
     * @return -1 if error encountered
     */
    int setImage_RGB888_RGBA8888(QImage img);

    /**
     * @brief setImage_convert adds new raw image to context, conversing colorspace
     * according to function converseColorspace()
     * @param img is object of QImage of any colorspace from enum supportedColorspaces
     * @return -1 if error encountered
     */
    int setImage_convert(QImage img);

    /**
     * @brief transcode attempts to encode prepared data into .heic
     * and in case of success saves its representation in memory into context
     * @return -1 if error
     */
    int transcode_asHeic();

    /**
     * @brief updateHeicWithMetadata writes raw metadata from .jpg file (Exif only)
     * to .heic raw data in memory
     * @param path is path to .jpg file
     * @return -1 if error
     */
    int updateHeicWithMetadata(QString path);

    /**
     * @brief updateHeicWithMetadata writes raw metadata from .jpg file (Exif only)
     * and also sets Exif tags
     * @param path is path to .jpg file
     * @param tags is string with objects received from AI
     * @return -1 if error
     */
    int updateHeicWithMetadata(QString path, QString tags);

    /**
     * @brief getTranscodeResult
     * @return gives result of transcoding
     */
    QByteArray getTranscodeResult();

    /**
     * @brief save_toByteArray writes raw .heic file into byte array,
     * whose pointer was written into object
     */
    void save_toByteArray();

signals:

private:
    heif::Context *ctx;
    heif::Encoder *enc;
    heif::Image img;
    heif_chroma chroma;
    heif::ImageHandle handle;
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

    coded_as way_of_coding=coded_as_undefined;

};

#endif // TCODEC_H
