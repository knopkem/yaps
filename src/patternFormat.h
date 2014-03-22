#ifndef patternFormat_h__
#define patternFormat_h__

#include <QList>

class PatternFormat
{
public:
    enum eTag {
        CameraMake,
        CameraModel,
        MediaType,
        Year,
        Month,
        MonthS,
        MonthL,
        Day,
        DayS,
        DayL,
        Hour,
        Minute,
        Second,
        Filename,
        NewSubDir,
        DelimiterDash,
        DelimiterDot,
        DelimiterHash,
        DelimiterUnderscore
    };

    QList<PatternFormat::eTag> FolderStructureContainer;

    QList<PatternFormat::eTag> FileStructureContainer;

    static QString tagToString(PatternFormat::eTag tag);

    static QString tagToDescription(PatternFormat::eTag tag);

    static QString tagToExample(PatternFormat::eTag tag);

};
#endif //patternFormat_h__
