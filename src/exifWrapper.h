#ifndef exifWrapper_h__
#define exifWrapper_h__

#include "exifData.h"
#include <QProcess>

class BaseLookup
{
public:
    virtual QString processName() = 0;
    virtual QStringList processParams() = 0;
    virtual QString fileName() = 0;
    virtual QString createDate() = 0;
    virtual QString cameraModel() = 0;
    virtual QString cameraMake() = 0;
    virtual QString mimeType() = 0;
};

class Exiv2Lookup : public BaseLookup
{
public:
    inline QString processName() {return "exiv2";}
    inline QStringList processParams() {return QStringList();}
    inline QString fileName() {return "File name";}
    inline QString createDate() {return "Image timestamp";}
    inline QString cameraModel() {return "Camera model";}
    inline QString cameraMake() {return "Camera make";}
    inline QString mimeType() {return "Thumbnail";}
};

class ExifToolLookup : public BaseLookup
{
public:
    inline QString processName() {return "exifTool";}
    inline QStringList processParams() {return QStringList() << "-CreateDate" << "-Model" << "-Make";}
    inline QString fileName() {return "File Name";}
    inline QString createDate() {return "Create Date";}
    inline QString cameraModel() {return "Camera Model Name";}
    inline QString cameraMake() {return "Make";}
    inline QString mimeType() {return "MIME type";}

};


class ExifWrapperPrivate;
class ExifWrapper : public QObject
{
    Q_OBJECT
public:
    ExifWrapper(const QString& path, QObject* parent = NULL);
    ~ExifWrapper();

    void parseFile(ExifData& data);

    enum eLookup {
        EXIFTOOL,
        EXIV2
    };

protected:

    ExifData doParse(eLookup type, const QString& path);

    QString findValue(const QStringList input, const QString& key);
    QString osSpecificPath();
    QString osSpecificExtension();

    bool isUnsupportedFileType(const QString& extension);

    bool isEqual(const QString& a, const QString& b);

private:
    ExifWrapperPrivate* d;

};
#endif // exifWrapper_h__

