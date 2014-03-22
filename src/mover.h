#ifndef mover_h__
#define mover_h__

/*
 * Copyright (C) 2014 Michael Knopke
 *
 * See the COPYRIGHT file for terms of use.
 */

#include "exifData.h"
#include "patternFormat.h"

#include <QList>

class FileOptions
{
public:
    enum eFileOperation {
        COPY,
        MOVE
    };

    FileOptions() {
        traverseSubdirectories = true;
        fixDuplicates = true;
        fileOp = COPY;
    }

    bool traverseSubdirectories;
    bool fixDuplicates;
    eFileOperation fileOp;
};


class MoverPrivate;
class Mover : public QObject
{
    Q_OBJECT
public:
    Mover(QObject* parent= NULL);
    ~Mover();

    bool performOperations(const QString& source, const QString& target, const FileOptions& options, const PatternFormat& format);

    static bool makedir(const QString& path);

    static bool dirExists(const QString& path);
protected:

    QStringList findAllFilesInDirectory(const QString& path, bool traverseSubdir);

    QList<ExifData> parseFiles(const QStringList& files, const FileOptions& options);

    bool performFileOperation(const QList<ExifData>& files, const QString& target, const FileOptions& options, const PatternFormat& format);

    bool fileOperation(const ExifData& data, const QString& target, const FileOptions& options, const PatternFormat& format);

    QString proposeNewFilename(const QString& path);

    QString createFolderStructure(const QString& root, const ExifData& data, const PatternFormat& format);

    QString createFilename(const ExifData& data, const PatternFormat& format);

    QString tagToValue(PatternFormat::eTag tag, const ExifData& data);

    bool hasDuplicateHash(const QString& folder, const QString& filepath);

    QString md5sum(const QString& filepath);

    void initializeFolder(const QString& folder);

    bool copyOrMoveFile(const QString& source, const QString& target, const FileOptions& options);

    void reset();

private:
    MoverPrivate* d;
};
#endif // mover_h__
