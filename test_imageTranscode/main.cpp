#include <iostream>
#include <QCoreApplication>
#include <QFileDialog>
#include <QString>
#include "libheif/heif.h"
#include "libheif/heif_cxx.h"

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

bool isHeifInterleaved(enum heif_chroma chr){
    switch (chr){
    case heif_chroma_420:
    case heif_chroma_422:
    case heif_chroma_444:
    case heif_chroma_monochrome:
    case heif_chroma_undefined:
        return 0;
    default:
        return 1;
    }
}

bool hasHeifAlpha(enum heif_chroma chr){
    switch (chr){
    case heif_chroma_interleaved_RGBA:
    case heif_chroma_interleaved_RRGGBBAA_BE:
    case heif_chroma_interleaved_RRGGBBAA_LE:
        return 1;
    default:
        return 0;
    }
}

uchar* get_heifData(QString s){
    heif::Context ctx;

    // get the default encoder
    heif::Encoder enc(heif_compression_HEVC);

    // set the encoder parameters
    enc.set_lossy_quality(50);

    // encode the image
    heif::Image heif_img;
    QImage qt_img(s);
    qt_img=qt_img.convertToFormat(QImage::Format_RGBA8888);
    int stride;
    heif_img.create(qt_img.width(), qt_img.height(), heif_colorspace_RGB, heif_chroma_interleaved_RGBA);
    heif_img.add_plane(heif_channel_interleaved, qt_img.width(), qt_img.height(), 8);
    uchar* dataptr=heif_img.get_plane(heif_channel_interleaved, &stride);
    memset(dataptr, 0, stride*heif_img.get_height(heif_channel_interleaved));
    for (int i=0; i<qt_img.height(); i++){
        memcpy(dataptr+stride*i, qt_img.scanLine(i), qt_img.bytesPerLine());
    }

    ctx.encode_image(heif_img, enc);

    ctx.write_to_file("encode.heic");
    return dataptr;
}

uchar* get_decodedData(QString s){
    heif_context* ctx = heif_context_alloc();
    heif_context_read_from_file(ctx, s.toStdString().c_str(), nullptr);

    // get a handle to the primary image
    heif_image_handle* handle;
    heif_context_get_primary_image_handle(ctx, &handle);

    // decode the image and convert colorspace to RGB, saved as 24bit interleaved
    heif_image* img;
    heif_decode_image(handle, &img, heif_colorspace_RGB, heif_chroma_interleaved_RGBA, nullptr);

    int stride;
    const uint8_t* data = heif_image_get_plane_readonly(img, heif_channel_interleaved, &stride);
    QImage qt_img(data, heif_image_get_width(img, heif_channel_interleaved),
                  heif_image_get_height(img, heif_channel_interleaved),
                  QImage::Format_RGBA8888);
    qt_img.save("decode.jpg", "JPG");

    return (uchar*)data;
}

int main(int argc, char *argv[])
{
    QString sample="sample.jpg";
    get_heifData("sample.jpg");
    //get_decodedData("sample.heic");
    return 0;
}
