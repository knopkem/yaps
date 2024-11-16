/*
 * Copyright (C) 2014 Michael Knopke
 *
 * See the COPYRIGHT file for terms of use.
 */

#include "simpleLog.h"

#include <iostream>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>

QString SimpleLog::logFileLocation = "";
QFile* SimpleLog::m_File = NULL;
QTextStream* SimpleLog::m_OutputStream = NULL;
QMutex SimpleLog::mutex;

//--------------------------------------------------------------------------------------

void SimpleLog::log( eLogLevel level, const QString& msg )
{
    QMutexLocker locker(&mutex);

    std::cout << msg.toStdString() << std::endl;

    QString outMsg = levelToString(level) + " (" + QDateTime::currentDateTime().toString("yyyy/MM/dd-hh:mm:ss") + ") " + msg;

    if (logFileLocation.isEmpty()) {
        return;
    }
    *m_OutputStream << outMsg

#ifdef Q_OS_WIN
        << '\r'  // Use standard windows line endings
#endif
        << Qt::endl;  // This is '\n' << flush;

    m_OutputStream->flush();
}

//--------------------------------------------------------------------------------------

QString SimpleLog::levelToString( eLogLevel l )
{
    QString result ="[INVALID_LOG_LEVEL]";
    switch(l)
    {
    case LOG_DEBUG:
        result = "[DEB]";
        break;
    case LOG_WARNING:
        result = "[WAR]";
        break;
    case LOG_ERROR:
        result = "[ERR]";
        break;
    default:
        break;
    }
    return result;
}

//--------------------------------------------------------------------------------------

void SimpleLog::startFileLogging( const QString& location )
{
    if (m_File) {
        stopFileLogging();
    }

    m_File = new QFile;
    m_OutputStream = new QTextStream;
    logFileLocation = location;
    m_File->setFileName(logFileLocation);
    m_OutputStream->setDevice(m_File);
    m_File->open(QFile::WriteOnly);
}

//--------------------------------------------------------------------------------------

void SimpleLog::stopFileLogging()
{
    if (m_File) {
        logFileLocation = "";
        if (m_File->isOpen()) {
            m_OutputStream->flush();
            m_OutputStream->setDevice(NULL);
            m_File->close();
        }
        delete m_File;
        m_File = NULL;
        delete m_OutputStream;
        m_OutputStream = NULL;
    }
}

//--------------------------------------------------------------------------------------

