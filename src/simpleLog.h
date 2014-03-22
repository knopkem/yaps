#ifndef simpleLog_h__
#define simpleLog_h__

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMutex>

class SimpleLog
{
public:
    enum eLogLevel {
        LOG_ERROR,
        LOG_WARNING,
        LOG_DEBUG
    };
    static void log(eLogLevel level, const QString& msg);

    static void startFileLogging(const QString& location);

    static void stopFileLogging();

protected:
    static QString levelToString(eLogLevel l);

private:
    static QString logFileLocation;

    static QFile* m_File;
    static QTextStream* m_OutputStream;

    static QMutex mutex;
};
#endif // simpleLog_h__

