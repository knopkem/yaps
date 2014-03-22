/*
 * Copyright (C) 2013 Michael Knopke
 *
 * See the COPYRIGHT file for terms of use.
 */

#include <QtGui>

#include "mainWindow.h"
#include "simpleLog.h"

#include <version_config.h>


void redirectMessageOutput(QtMsgType type, const char *msg)
{
    switch (type) {
    case QtDebugMsg:
        SimpleLog::log(SimpleLog::LOG_DEBUG, msg);
        break;
    case QtWarningMsg:
        SimpleLog::log(SimpleLog::LOG_WARNING, msg);
        break;
    case QtCriticalMsg:
        SimpleLog::log(SimpleLog::LOG_ERROR, msg);
        break;
    case QtFatalMsg:
        SimpleLog::log(SimpleLog::LOG_ERROR, msg);
        break;
    default:
        break;
    }
}

/**
 * Main application entry point
 */
int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("nonprofit");
    QCoreApplication::setOrganizationDomain("nonprofit");
    QCoreApplication::setApplicationName("PhotoMover");
    QCoreApplication::setApplicationVersion(VERSION_STRING);
    Q_INIT_RESOURCE(app);

    qInstallMsgHandler(redirectMessageOutput);

    QApplication app(argc, argv);
    MainWindow window;

    window.show();
    return app.exec();
}
