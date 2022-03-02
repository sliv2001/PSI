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
class TMediaFile : public QObject
{
    Q_OBJECT
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
     * @brief concatTags collects data from raw array of server response and deletes duplicates as well as puts data into proper format.
     * @param array is raw set of data
     * @return string of image tags
     */
    static QString concatTags(QVector<QByteArray> array);

    /**
     * @brief updatePropertiesWithResponse fills raw data with response from array
     * @param array
     */
    void updatePropertiesWithResponse(QVector<QByteArray> array);

    QByteArray encodeImage();

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

    /**
     * @brief rawDetectionBoxes are boxes found by server in special format
     */
    QByteArray rawDetectionBoxes;

    /**
     * @brief rawDetectionClassEntities are entities, like in TMediaFile::tags, but with repetitions
     */
    QByteArray rawDetectionClassEntities;

    /**
     * @brief rawDetectionScores are probabilities for an object to be properly named
     */
    QByteArray rawDetectionScores;

    enum coded_as {
        coded_as_undefined,
        coded_as_RAW,
        coded_as_JPEG,
        coded_as_HEIC
    };

    TMediaFile::coded_as way_of_coding=coded_as_undefined;

    /**
     * @brief tagsHaveBeenSet tells whether recognition was done successfully
     * and this->tags are set appropriate
     */
    bool tagsHaveBeenSet=0;

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
