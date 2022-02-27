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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::string fullPath;
    //std::cin>>fullPath;
    QByteArray res;
    heif::Context ctx;
    heif::Encoder ecd(heif_compression_HEVC);
    heif::Image heif_img;
    QString p;
    p.fromStdString(fullPath);
    p="D:\\Desktop\\образец\\2017\\IMG_5126.JPG";
    QImage qt_img(p);

    enum heif_chroma heif_chr;
    enum heif_colorspace heif_sps;
    if ((heif_chr=getHeifChroma(qt_img.format()))==heif_chroma_undefined||(heif_sps=getHeifColorSpace(qt_img.format()))==heif_colorspace_undefined){
        printf("wrong img");
        return -1;
    }

    QByteArray red, green, blue, alpha, inter;
    int bytesPerLine=0;
    heif_img.create(qt_img.width(), qt_img.height(), heif_sps, heif_chr);
    if (isHeifInterleaved(heif_chr)){
        heif_img.add_plane(heif_channel_interleaved, qt_img.width(), qt_img.height(), qt_img.depth());
        char* dataptr=(char*)heif_img.get_plane(heif_channel_interleaved, &bytesPerLine);
        if (dataptr==NULL||bytesPerLine<1){
            printf("wrong dataptr");
            return -1;
        }
        inter.setRawData(dataptr, bytesPerLine*qt_img.height());
        for (int i=0; i<qt_img.height(); i++){
            inter.append((const char*)qt_img.scanLine(i), qt_img.bytesPerLine());
        }
    }
    else {
        heif_img.add_plane(heif_channel_R, qt_img.width(), qt_img.height(), qt_img.depth());
        char* dataptr=(char*)heif_img.get_plane(heif_channel_R, &bytesPerLine);
        if (dataptr==NULL||bytesPerLine<1){
            printf("wrong dataptr");
            return -1;
        }
        red.setRawData(dataptr, bytesPerLine*qt_img.height());
        heif_img.add_plane(heif_channel_G, qt_img.width(), qt_img.height(), qt_img.depth());
        dataptr=(char*)heif_img.get_plane(heif_channel_G, &bytesPerLine);
        if (dataptr==NULL||bytesPerLine<1){
            printf("wrong dataptr");
            return -1;
        }
        green.setRawData(dataptr, bytesPerLine*qt_img.height());
        heif_img.add_plane(heif_channel_B, qt_img.width(), qt_img.height(), qt_img.depth());
        dataptr=(char*)heif_img.get_plane(heif_channel_B, &bytesPerLine);
        if (dataptr==NULL||bytesPerLine<1){
            printf("wrong dataptr");
            return -1;
        }
        blue.setRawData(dataptr, bytesPerLine*qt_img.height());
        if (hasHeifAlpha(heif_chr)){
            heif_img.add_plane(heif_channel_Alpha, qt_img.width(), qt_img.height(), qt_img.depth());
            dataptr=(char*)heif_img.get_plane(heif_channel_Alpha, &bytesPerLine);
            if (dataptr==NULL||bytesPerLine<1){
                printf("wrong dataptr");
                return -1;
            }
            alpha.setRawData(dataptr, bytesPerLine*qt_img.height());
        }
    }




    return 0;
}
