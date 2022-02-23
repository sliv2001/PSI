#ifndef TMEDIAFILE_H
#define TMEDIAFILE_H

#include <QString>
#include <QFileInfo>
#include <QIcon>
#include <QDir>
#include <exiv2/exiv2.hpp>
#include <math.h>

/**
 * @brief The TMediaFile class is primitive class which contains methods to analyse and edit files.
 */
class TMediaFile
{
public:
    /**
     * @brief TMediaFile constructor creates new empty instance of data saving primitive.
     */
    TMediaFile();

    /**
     * @brief TMediaFile constructor creates new empty instance of data saving primitive.
     * @param path tells which file must be chosen.
     */
    TMediaFile(QString path);

    QIcon getIcon() const;

    QSize getResolution() const;

    bool move(QString path);

    /**
     * @brief picture is data of a photo itself
     */
    QPixmap* picture;

    /**
     * @brief fullPath is obviously absolute path to current file.
     */
    QString fullPath;

    /**
     * @brief year is obviously year found in metadata by getYear().
     */
    int year;

    /**
     * @brief quality is rating of this file. Yet in development.
     */
    int quality;

    /**
     * @brief unique is rating which shows uniqueness of this file. Yet in development.
     */
    float unique;

    /**
     * @brief tags is QString which contains metatags this way: "tag1;tag2;tag3;" or
     * "NOTAG" if nothing was found, or "" if didn't search yet.
     */
    QString tags="";

    /**
     * @brief live_path contains path to live-video in iPhone-style data.
     */
    QString live_path;

    /**
     * @brief pictureCode is hash identifier from full path to a file.
     */
    QByteArray pictureCode;

private:
    /**
     * @brief getYear uses Exiv2 to read DateTimeOriginal metadatum from current file.
     * @return year if it's written, 0 if not, -1 if error.
     */
    int getYear_setTags();

    /**
     * @brief getLiveVideo searches for live-video in the same directory but a different suffix.
     * @param path is string representing abs or rel path to file
     */
    void getLiveVideo(QString path);
    QSize resolution;

    /**
     * @brief getHashCode creates a hash identifier using full path to a file.
     */
    void getHashCode();
};

#endif // TMEDIAFILE_H
