/*
 * Copyright (C) 2014 Michael Knopke
 *
 * See the COPYRIGHT file for terms of use.
 */

#include "mover.h"
#include "exifWrapper.h"
#include "simpleLog.h"

#include "reportDlg.h"

#include <QtGui>

struct AddWrapper {
    ExifWrapper *instance;
    AddWrapper(ExifWrapper *w): instance(w) {}
    void operator()(ExifData& data) {
        instance->parseFile(data);
    }
};

namespace {
    QString DUPLICATES_FOLDER = "[Duplicates]";
    QString NOEXIFDATA_FOLDER = "[NoExifData]";
}

//--------------------------------------------------------------------------------------

class MoverPrivate
{
public:
    QString appPath;
    QString currentTarget;
    QString sourcePath;
    QHash<QString, QSet<QString> > md5HashSet;
    QHash<QString,QString> fileHash;
    ExifWrapper* wrapper;

    ReportDetail report;

    AddWrapper *wrap;
};

//--------------------------------------------------------------------------------------

Mover::Mover( QObject* parent/*= NULL*/ ) : QObject(parent), d(new MoverPrivate)
{
    d->appPath = QApplication::applicationDirPath();
#ifdef Q_WS_MAC
    d->appPath = QCoreApplication::applicationDirPath() + "/../../..";
#endif
    d->wrapper = new ExifWrapper(d->appPath, this);
    d->wrap = new AddWrapper(d->wrapper);
}

//--------------------------------------------------------------------------------------

Mover::~Mover()
{
    delete d;
    d = NULL;
}

//--------------------------------------------------------------------------------------

bool Mover::performOperations(const QString &source, const QString &target, const FileOptions& options, const PatternFormat& format)
{
    this->reset();
    d->sourcePath = source;

    // make sure the target folder exists
    if (!makedir(target) ) {
        return false;
    }

    d->currentTarget = target;
    QString logs = QDesktopServices::storageLocation(QDesktopServices::TempLocation) + "/yaps_logs";
    makedir(logs);
    QString logFilepath = logs + "/" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".txt";
    SimpleLog::startFileLogging(logFilepath);

    QElapsedTimer elapsed;
    elapsed.start();

    // query all files in the directory
    QStringList files = findAllFilesInDirectory(source, options.traverseSubdirectories);

    // parse meta tags
    QList<ExifData> parsedData = parseFiles(files, options);

    // now copy or move
    bool result = performFileOperation(parsedData, target, options, format);

    SimpleLog::stopFileLogging();

    // show report
    ReportDlg myDialog;
    myDialog.setModal(true);

    ReportDetail detail;
    detail = d->report;
    detail.ElapsedTime = QDateTime::fromTime_t(elapsed.elapsed()/1000).toUTC().toString("hh:mm:ss");
    detail.LogfilePath = logFilepath;

    myDialog.setReportDetail(detail);

    qWarning() << "Operation took" << detail.ElapsedTime;
    myDialog.exec();

    return result;
}

//--------------------------------------------------------------------------------------

QStringList Mover::findAllFilesInDirectory( const QString& path, bool traverseSubdir )
{
    QStringList result;
    qDebug() << "traversal started";

    QDir dir(path);
    dir.setFilter(QDir::Files);

    QProgressDialog progress(tr("Looking for files..."), tr("Cancel"), 0, 0);
    progress.setModal(true);

    if ( dir.exists() ) {
        bool stop = false;
        QDirIterator directory_walker(path, QDir::Files, QDirIterator::Subdirectories);
        int i=0;
        while( directory_walker.hasNext() && !stop) {
            if (progress.wasCanceled()) {
                break;
            }
            QString file = directory_walker.next();
            result << file;

            if (!traverseSubdir) {
                stop = true;
            }

            progress.setValue(i++);
            d->report.FilesTotal++;
        }
    }

    qDebug() << "traversal finished";
    progress.hide();

    return result;
}

//--------------------------------------------------------------------------------------

QList<ExifData> Mover::parseFiles( const QStringList& files, const FileOptions& options )
{
    qDebug() << "parsing started";

    int numFiles = files.count();
    QProgressDialog progress("", tr("Cancel"), 0, numFiles);
    progress.setLabelText(QString(tr("Parsing meta information using %1 thread(s)...")).arg(QThread::idealThreadCount()));
    progress.setModal(true);

    QList<ExifData> parsedData;
    foreach(const QString& path, files) {
        ExifData data;
        data.FilePath = path;
        QFileInfo info(path);
        data.AbsolutePath = info.absolutePath();
        parsedData << data;
    }
    
    // Create a QFutureWatcher and connect signals and slots.
    // Monitor progress changes of the future
    QFutureWatcher<void> futureWatcher;
    QObject::connect(&futureWatcher, SIGNAL(finished()), &progress, SLOT(reset()));
    QObject::connect(&progress, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
    QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &progress, SLOT(setRange(int,int)));
    QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &progress, SLOT(setValue(int)));

    // Start the computation.
    futureWatcher.setFuture(QtConcurrent::map(parsedData, *d->wrap));

    // Display the dialog and start the event loop.
    progress.exec();

    futureWatcher.waitForFinished();

    qDebug() << "parsing finished";

    return parsedData;

}

//--------------------------------------------------------------------------------------

bool Mover::performFileOperation( const QList<ExifData>& parsedData, const QString& target, const FileOptions& options, const PatternFormat& format )
{
    qDebug() << "copying started";

    int numFiles = parsedData.count();
    QProgressDialog progress("Copying files...", "Cancel", 0, numFiles);
    progress.setModal(true);

    for(int i=0; i<numFiles; i++) {
        ExifData data = parsedData.at(i);
        if (progress.wasCanceled()) {
            break;
        }
        if (!fileOperation(data, target, options, format)) {
            d->report.FilesFailed++;
        }
        progress.setValue(i);
    }

    qDebug() << "copying finished";
    progress.hide();

    return true;
}

//--------------------------------------------------------------------------------------

bool Mover::fileOperation(const ExifData &data, const QString &target, const FileOptions& options, const PatternFormat& format)
{
    if (data.FilePath.isEmpty()) {
        qWarning() << "file error, no path given";
        return false;
    }

    if (data.CreateDate.isInvalid()) {
        qWarning() << "invalid creation date, or no exif meta data present" << data.FilePath;
        QString relativePath = data.AbsolutePath;
        relativePath.remove(d->sourcePath);
        QString folder = target + "/" + NOEXIFDATA_FOLDER + "/" + relativePath;
        makedir(folder);
        return copyOrMoveFile(data.FilePath, folder + "/" + data.FileName + "." + data.Extention, options);
    }

    d->report.FilesWithExif++;

    // create folders from pattern
    QString finalTarget = createFolderStructure(target, data, format);

    // check for duplicates
    if (options.fixDuplicates) {
        initializeFolder(finalTarget);
        if (hasDuplicateHash(finalTarget, data.FilePath)) {
            d->report.Duplicates++;
            qWarning() << data.FileName << ", file already exists in target location" << finalTarget;
            if (options.copyDuplicates) {
                QString relativePath = data.AbsolutePath;
                relativePath.remove(d->sourcePath);
                QString folder = target + "/" + DUPLICATES_FOLDER + "/" + relativePath;
                qWarning() << "copy " << folder;
                makedir(folder);
                return copyOrMoveFile(data.FilePath, folder + "/" + data.FileName + "." + data.Extention, options);
            }
            return true;
        }
    }

    // create filename from pattern
    finalTarget += "/"+ createFilename(data, format);

    bool result =  copyOrMoveFile(data.FilePath, finalTarget, options);
    if (result) {
        d->report.FilesCopied++;
    }
    return result;
}

//--------------------------------------------------------------------------------------

bool Mover::copyOrMoveFile( const QString& source, const QString& target, const FileOptions& options )
{
    QString finalTarget = target;
    // now check for name clashes and create a new filename
    if (QFile::exists(target)) {
        finalTarget = proposeNewFilename(finalTarget);
        qWarning() << "file already exists (" << finalTarget << " )  creating new name:" << finalTarget;
    }


    // depending on the input either copy or move the file
    QFile original(source);

    bool ok = false;
    if (options.fileOp == FileOptions::COPY) {
        ok = original.copy(finalTarget);
    }
    else {
        ok = original.rename(finalTarget);
    }

    if (!ok) {
        qWarning() << QString("Failed to copy file %1 to target %2").arg(source, target);
    }

    return ok;
}

//--------------------------------------------------------------------------------------

bool Mover::dirExists( const QString& path )
{
    QDir dir(path);
    return dir.exists(path);
}

//--------------------------------------------------------------------------------------

bool Mover::makedir( const QString& path )
{
    QDir dir(path);

    if (dir.exists(path)){
        return true;
    }

    if (!dir.mkpath(path)) {
        qWarning() << "Could not create directory: " << path;
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------

QString Mover::proposeNewFilename(const QString &path)
{
    QString result = path;

    QString proposed;
    int i = 1;
    do {
        QFileInfo info(result);
        QString suffix =  info.suffix();
        if (!suffix.isEmpty()) {
            suffix.prepend(".");
        }
        proposed = info.path() + "/" + info.completeBaseName() + "(" + QString::number(i) + ")" + suffix;
        i++;
    } while (QFile::exists(proposed));
    result = proposed;
    return result;
}

//--------------------------------------------------------------------------------------

QString Mover::createFolderStructure( const QString& root, const ExifData& data, const PatternFormat& format )
{
    QString result;
    foreach( PatternFormat::eTag tag, format.FolderStructureContainer) {
        result += tagToValue(tag, data);
    }

    QStringList subdirs = result.split("/");

    QString part = root;
    for(int i=0; i < subdirs.count(); ++i) {
        QString next = subdirs.at(i);
        part += "/"+ next;
        qDebug() << "creating directory" << part;
        makedir(part);
    }
    result.prepend(root + "/");
    return result;
}

//--------------------------------------------------------------------------------------

QString Mover::createFilename( const ExifData& data, const PatternFormat& format )
{
    QString result;
    foreach( PatternFormat::eTag tag, format.FileStructureContainer) {
        result += tagToValue(tag, data);
    }
    return result + "." + data.Extention;
}

//--------------------------------------------------------------------------------------

QString Mover::tagToValue(PatternFormat::eTag tag, const ExifData& data)
{
    QString result;
    switch(tag)
    {
    case PatternFormat::CameraMake:
        result = data.CameraMake;
        break;
    case PatternFormat::CameraModel:
        result = data.CameraModel;
        break;
    case PatternFormat::MediaType:
        if (data.MimeType.contains("image"))
            result = "image";
		else if (data.MimeType.contains("video"))
			result = "video";
        else
            result = "other";
        break;
    case PatternFormat::Year:
        result = data.CreateDate.year();
        break;
    case PatternFormat::Month:
        result = data.CreateDate.month();
        break;
    case PatternFormat::MonthS:
        result = data.CreateDate.monthS();
        break;
    case PatternFormat::MonthL:
        result = data.CreateDate.monthL();
        break;
    case PatternFormat::Day:
        result = data.CreateDate.day();
        break;
    case PatternFormat::DayS:
        result = data.CreateDate.dayS();
        break;
    case PatternFormat::DayL:
        result = data.CreateDate.dayL();
        break;
    case PatternFormat::Hour:
        result = data.CreateDate.hour();
        break;
    case PatternFormat::Minute:
        result = data.CreateDate.minute();
        break;
    case PatternFormat::Second:
        result = data.CreateDate.second();
        break;
    case PatternFormat::Filename:
        result = data.FileName;
        break;
    case PatternFormat::NewSubDir:
        result = "/";
        break;
    case PatternFormat::DelimiterDash:
        result = "-";
        break;
    case PatternFormat::DelimiterDot:
        result = ".";
        break;
    case PatternFormat::DelimiterHash:
        result = "#";
        break;
    case PatternFormat::DelimiterUnderscore:
        result = "_";
        break;
    case PatternFormat::DelimiterTilde:
        result = "~";
        break;
    case PatternFormat::DelimiterWhiteSpace:
        result = " ";
        break;

    default:
        qWarning() << "default not implemented";
        break;
    }
    return result;
}

//--------------------------------------------------------------------------------------

QString Mover::md5sum( const QString& filepath )
{
    QFile file(filepath);

    if (!file.exists()) {
        qWarning() << "file doesn't exist at location" << filepath;
    }

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();

        QByteArray hashData = QCryptographicHash::hash(fileData,QCryptographicHash::Md5);
        return hashData.toHex();
    }
    qWarning() << "[md5sum] unable to read file" << filepath;
    return QString();
}

//--------------------------------------------------------------------------------------

bool Mover::hasDuplicateHash( const QString& folder, const QString& filepath )
{
    if (!d->md5HashSet.contains(folder)) {
        return false;
    }

    QSet<QString> set = d->md5HashSet.value(folder);

    QString md5 = md5sum(filepath);

    bool result = set.contains(md5);
    if (!result) {
        set.insert(md5);
        d->md5HashSet[folder] = set;
    }
    return result;
}

//--------------------------------------------------------------------------------------

void Mover::initializeFolder( const QString& folder )
{
    if (d->md5HashSet.contains(folder)) {
        return;
    }

    // find all files
    QDir dir(folder);
    QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    if (!files.empty()) {
        qDebug() << "initializing folder with" << files.count() << "files";
    }

    // hash them
    QSet<QString> set;
    foreach(const QString& file, files) {
        set.insert(md5sum(folder + "/" + file));
    }

    d->md5HashSet[folder] = set;
}

//--------------------------------------------------------------------------------------

void Mover::reset()
{
    d->md5HashSet.clear();
    d->fileHash.clear();

    d->report.Duplicates = 0;
    d->report.FilesCopied = 0;
    d->report.FilesFailed = 0;
    d->report.FilesWithExif = 0;
    d->report.FilesTotal = 0;
}

//--------------------------------------------------------------------------------------

