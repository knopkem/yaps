/*
 * Copyright (C) 2014 Michael Knopke
 *
 * See the COPYRIGHT file for terms of use.
 */

#include "exifWrapper.h"
#include "exifData.h"

#include <QtGui>
#include <QApplication>

class ExifWrapperPrivate
{
public:
    QString   appPath;
};
//--------------------------------------------------------------------------------------

ExifWrapper::ExifWrapper( const QString& path, QObject* parent /*= NULL*/ ) :
    QObject(parent), d(new ExifWrapperPrivate)
{
    d->appPath = path;

}

//--------------------------------------------------------------------------------------

ExifWrapper::~ExifWrapper()
{
    delete d;
}

//--------------------------------------------------------------------------------------

QString ExifWrapper::findValue( const QStringList input, const QString& key )
{
    QString result;
    for(int i=0; i < input.count(); ++i) {
        QString current = input.at(i);
        if (current.contains(key)) {
            int index = current.indexOf(":");
            if (index > 0) {
                result = current.right(current.length() - (index+1)).trimmed();
            }
        }
    }
    return result;
}

//--------------------------------------------------------------------------------------

void ExifWrapper::parseFile( ExifData& data )
{

    // prepare data
    QFileInfo info (data.FilePath);

    // skip known extensions
    if (isUnsupportedFileType(info.completeSuffix())) {
        data.Extention = info.completeSuffix();
        data.FileName = info.completeBaseName();
        return;
    }

    // decide what tool to use, we want exiv2 only for jpg
    if ( isEqual(info.completeSuffix(), "jpg") ) {
        data = doParse(EXIV2, data.FilePath);
        if (data.CreateDate.isInvalid()) {
            qWarning() << "Parsing failed once, retrying.";
            data = doParse(EXIFTOOL, data.FilePath);
            if (data.CreateDate.isInvalid()) {
                qWarning() << "Parsing failed again, giving up on file" << data.FilePath;
            }
        }
    }
    else {
        data = doParse(EXIFTOOL, data.FilePath);
    }

}

//--------------------------------------------------------------------------------------

ExifData ExifWrapper::doParse( eLookup type, const QString& path )
{
    QFileInfo info (path);
    ExifData data;
    data.FilePath = path;
    data.Extention = info.completeSuffix();
    data.FileName = info.completeBaseName();

    BaseLookup* lookup = NULL;
    if (type == EXIFTOOL) {
        lookup = new ExifToolLookup;
    }
    else {
        lookup = new Exiv2Lookup;
    }
    QString processPath = this->osSpecificPath() + lookup->processName() + this->osSpecificExtension();

#if defined (Q_WS_WIN) || defined( Q_WS_MAC)
    if (!QFile::exists(processPath) ) {
        qCritical() << "process not found" << processPath;
    }
#endif

    // start process
    QStringList args;
    args << lookup->processParams() << path;
    QProcess* process = new QProcess;

    process->start(processPath, args);
    // and wait for it to finish
    process->waitForReadyRead();
    process->waitForFinished();

    // make sure we have all events processed
    //qApp->processEvents();

    // now read and parse the result
    QByteArray bytes = process->readAllStandardOutput();
    QString message(bytes.constData());
    QStringList result = message.split("\n");

    data.CreateDate = ExifDate(findValue(result, lookup->createDate()));
    data.CameraModel = findValue(result, lookup->cameraModel());
    data.CameraMake = findValue(result, lookup->cameraMake());
    data.MediaType = findValue(result, lookup->mimeType());
    
    delete process;
    delete lookup;
    // finally data should be up-to-date
    return data;
}

//--------------------------------------------------------------------------------------


QString ExifWrapper::osSpecificPath()
{
#if defined (Q_WS_WIN) || defined( Q_WS_MAC)
    return d->appPath + "/";
#endif
    return QString();
}

//--------------------------------------------------------------------------------------

QString ExifWrapper::osSpecificExtension()
{
#ifdef OS_WIN
    return ".exe";
#endif
    return QString();
}

//--------------------------------------------------------------------------------------

bool ExifWrapper::isUnsupportedFileType( const QString& extension )
{
    QStringList fileTypes;
    fileTypes << "txt";
    fileTypes << "png";
    fileTypes << "raw";

    foreach(const QString& type, fileTypes) {
        if (isEqual(extension, type)) {
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------

bool ExifWrapper::isEqual( const QString& a, const QString& b )
{
    return (a.compare(b, Qt::CaseInsensitive) == 0 );
}

//--------------------------------------------------------------------------------------


