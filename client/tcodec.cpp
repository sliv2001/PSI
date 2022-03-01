#include "libheif/heif.h"
#include "tcodec.h"

TCodec::TCodec(uint8_t quality, QObject *parent)
    : QObject{parent}
{
    ctx = new heif::Context();
    enc=new heif::Encoder(heif_compression_HEVC);
    enc->set_lossy_quality(quality);
}

TCodec::~TCodec()
{
    delete ctx;
    delete enc;
}

heif_chroma TCodec::getHeifChroma(QImage::Format format){
    switch (format){
    case QImage::Format_Invalid:                    return heif_chroma_undefined;
    case QImage::Format_Mono:                       return heif_chroma_monochrome;
    case QImage::Format_MonoLSB:                    return heif_chroma_undefined;
    case QImage::Format_Indexed8:                   return heif_chroma_undefined;
    case QImage::Format_RGB32:                      return heif_chroma_interleaved_RRGGBB_BE;
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

int TCodec::getBitDepth(int qt_img_depth)
{
    switch (qt_img_depth){
    case 16: return 4;
    case 24: return 8;
    case 32: return 8;
    default: return -1;
    }
}

bool TCodec::hasHeifAlpha(heif_chroma chr)
{
    switch (chr){
    case heif_chroma_interleaved_RGBA:
    case heif_chroma_interleaved_RRGGBBAA_BE:
    case heif_chroma_interleaved_RRGGBBAA_LE:
        return 1;
    default:
        return 0;
    }
}

int TCodec::setImage(QImage qt_img)
{
    int stride;

    if (qt_img.format()!=QImage::Format_RGB888&&qt_img.format()!=QImage::Format_RGBA8888){
        return -1;
    }
    chroma = getHeifChroma(qt_img.format());
    if (chroma==heif_chroma_undefined){
        return -1;
    }
    try{
        img.create(qt_img.width(), qt_img.height(), heif_colorspace_RGB, heif_chroma_interleaved_RGB);
    }
    catch (heif::Error e){
        return -1;
    }

    if ((bitDepth=getBitDepth(qt_img.depth()))<0){
        return -1;
    }
    try{
        img.add_plane(heif_channel_interleaved, qt_img.width(), qt_img.height(), bitDepth);
    }
    catch (heif::Error e){
        return -1;
    }

    uchar* dataptr=img.get_plane(heif_channel_interleaved, &stride);
    if (stride<=0||dataptr==nullptr){
        return -1;
    }
    memset(dataptr, 0, stride*img.get_height(heif_channel_interleaved));
    for (int i=0; i<qt_img.height(); i++){
        memcpy(dataptr+stride*i, qt_img.scanLine(i), qt_img.bytesPerLine());
    }
    /*@TODO check errors*/
    return 0;
}

int TCodec::transcode()
{
    heif::ImageHandle handle;
    try{
        handle=ctx->encode_image(img, *enc);
    }
    catch (heif::Error e){
        return -1;
    }

    QByteArray res="";

    writer w(&res);
    ctx->write(w);
    /*@FIXME possible error with freeing of memory used by res*/
    transcodeResult=res;
    return 0;
}

heif_error TCodec::writer::write(const void *data, size_t size)
{
    if (r!=nullptr)
        r->setRawData((const char*)data, size);
    heif_error e;
    e.code=heif_error_Ok;
    return e;
}
