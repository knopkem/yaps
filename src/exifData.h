#ifndef exifData_h__
#define exifData_h__

#include <QString>
#include <QObject>
#include <QDateTime>

class ExifDate 
{
public:
    ExifDate(const QString& input="");

    QString year() const;
    QString month() const;
    QString monthS() const;
    QString monthL() const;

    QString day() const;
    QString dayS() const;
    QString dayL() const;

    QString hour() const;
    QString minute() const;
    QString second() const;

    bool isInvalid() const;

protected:

    int parseYear(const QString& input);
    int parseMonth(const QString& input);
    int parseDay(const QString& input);
    int parseHour(const QString& input);
    int parseMinute(const QString& input);
    int parseSecond(const QString& input);

    QDateTime m_date;
};

struct ExifData
{
    QString FilePath;
    QString FileName;
    QString AbsolutePath;
    QString Extention;
    ExifDate CreateDate;
    QString CameraModel;
    QString CameraMake;
    QString MediaType;
};
#endif // exifData_h__

