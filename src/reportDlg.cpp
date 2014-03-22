/*
 * Copyright (C) 2014 Michael Knopke
 *
 * See the COPYRIGHT file for terms of use.
 */

#include "reportDlg.h"

#include <QtGui>

class ReportDlgPrivate
{
public:
    QLabel* statusL;
    QPushButton* openButton;
    QString logpath;
};

ReportDlg::ReportDlg( QWidget *parent /*= 0*/ ) : QDialog(parent), d(new ReportDlgPrivate)
{
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
    QLabel* captionL = new QLabel(QString(tr("Report from: ")) + QDateTime::currentDateTime().toString());
    d->statusL = new QLabel;
    d->openButton = new QPushButton(tr("show log"));
    QVBoxLayout* hbox = new QVBoxLayout;
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(d->openButton, SIGNAL(clicked()), this, SLOT(openLogFile()));

    hbox->addWidget(captionL);
    hbox->addWidget(d->statusL);
    hbox->addWidget(d->openButton);
    hbox->addWidget(buttonBox);
    setLayout(hbox);
}

ReportDlg::~ReportDlg()
{
    delete d;
    d = NULL;
}

void ReportDlg::setReportDetail( const ReportDetail& detail )
{
    d->statusL->setText( QString("Time taken: %1\n"
        "Total number of files found: %2\n"
        "Number of Exif files: %3\n"
        "Number of duplicate files: %4\n"
        "Number of copied files: %5\n"
        "Number of failed files: %6\n"
        ).arg(detail.ElapsedTime)
            .arg(detail.FilesTotal)
            .arg(detail.FilesWithExif)
            .arg(detail.Duplicates)
            .arg(detail.FilesCopied)
                         .arg(detail.FilesFailed));
    d->logpath = detail.LogfilePath;
}

void ReportDlg::openLogFile()
{
    if (!QDesktopServices::openUrl(QUrl("file:///" + d->logpath, QUrl::TolerantMode))) {
        qWarning() << "failed to open logfile file" << d->logpath;
    }
}

