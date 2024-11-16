/*
 * Copyright (C) 2013 Michael Knopke
 *
 * See the COPYRIGHT file for terms of use.
 */

#include "mainWindow.h"
#include "simpleLog.h"
#include <version_config.h>

#include <QtGui>
#include <QtWidgets>
#include <iostream>

void redirectMessageOutput(QtMsgType type, const QMessageLogContext& ctx, const QString &msg)
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
    QCoreApplication::setApplicationName("yaps");
    QCoreApplication::setApplicationVersion(VERSION_STRING);
    Q_INIT_RESOURCE(app);

    qInstallMessageHandler(redirectMessageOutput);

    QApplication app(argc, argv);
    MainWindow window;

    window.show();
    return app.exec();
}
