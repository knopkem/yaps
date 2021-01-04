/*
 * Copyright (C) 2014 Michael Knopke
 *
 * See the COPYRIGHT file for terms of use.
 */

#include <patternFormat.h>

#include <QString>
#include <QDebug>
#include <QDateTime>

QString PatternFormat::tagToString( eTag tag )
{
    QString result;
    switch(tag) {
    case PatternFormat::CameraMake:
        result = "[CamMake]";
        break;
    case PatternFormat::CameraModel:
        result = "[CamModel]";
        break;
    case PatternFormat::MediaType:
        result = "[MediaType]";
        break;
    case PatternFormat::Year:
        result = "[year: yyyy]";
        break;
    case PatternFormat::Month:
        result = "[month: mm]";
        break;
    case PatternFormat::MonthS:
        result = "[month: short]";
        break;
    case PatternFormat::MonthL:
        result = "[month: long]";
        break;
    case PatternFormat::WeekNumber:
        result = "[WeekNumber]";
        break;
    case PatternFormat::Day:
        result = "[day: dd]";
        break;
    case PatternFormat::DayS:
        result = "[day: short]";
        break;
    case PatternFormat::DayL:
        result = "[day: long]";
        break;
    case PatternFormat::Hour:
        result = "[hour: hh]";
        break;
    case PatternFormat::Minute:
        result = "[minute: MM]";
        break;
    case PatternFormat::Second:
        result = "[second: ss]";
        break;
    case PatternFormat::Filename:
        result = "[fname]";
        break;
    case PatternFormat::NewSubDir:
        result = "[/]";
        break;
    case PatternFormat::DelimiterDash:
        result = "[-]";
        break;
    case PatternFormat::DelimiterDot:
        result = "[.]";
        break;
    case PatternFormat::DelimiterHash:
        result = "[#]";
        break;
    case PatternFormat::DelimiterUnderscore:
        result = "[_]";
        break;
    case PatternFormat::DelimiterTilde:
        result = "[~]";
        break;
    case PatternFormat::DelimiterWhiteSpace:
        result = "[ ]";
        break;
    default:
        qWarning() << "default not implemented";
        break;
    }
    return result;
}

//--------------------------------------------------------------------------------------

QString PatternFormat::tagToDescription( eTag tag )
{
    QString result;
    switch(tag) {
    case PatternFormat::CameraMake:
        result = "Camera brand name";
        break;
    case PatternFormat::CameraModel:
        result = "Camera model name";
        break;
    case PatternFormat::MediaType:
        result = "One of the following: image, video, other";
        break;
    case PatternFormat::Year:
        result = "creation date - year";
        break;
    case PatternFormat::Month:
        result = "creation date - month";
        break;
    case PatternFormat::MonthS:
        result = "creation date - month short name";
        break;
    case PatternFormat::MonthL:
        result = "creation date - month long name";
        break;
    case PatternFormat::WeekNumber:
        result = "creation date - week number";
        break;
    case PatternFormat::Day:
        result = "creation date - day";
        break;
    case PatternFormat::DayS:
        result = "creation date - day short name";
        break;
    case PatternFormat::DayL:
        result = "creation date - day long name";
        break;
    case PatternFormat::Hour:
        result = "creation date - hour";
        break;
    case PatternFormat::Minute:
        result = "creation date - minute";
        break;
    case PatternFormat::Second:
        result = "creation date - second";
        break;
    case PatternFormat::Filename:
        result = "original filename";
        break;
    case PatternFormat::NewSubDir:
        result = "create new subdirectory";
        break;
    case PatternFormat::DelimiterDash:
        result = "insert delimiter -";
        break;
    case PatternFormat::DelimiterDot:
        result = "insert delimiter .";
        break;
    case PatternFormat::DelimiterHash:
        result = "insert delimiter #";
        break;
    case PatternFormat::DelimiterUnderscore:
        result = "insert delimiter _";
        break;
    case PatternFormat::DelimiterTilde:
        result = "insert delimiter ~";
        break;
    case PatternFormat::DelimiterWhiteSpace:
        result = "insert delimiter [whitespace]";
        break;

    default:
        qWarning() << "default not implemented";
        break;
    }
    return result;
}

//--------------------------------------------------------------------------------------

QString PatternFormat::tagToExample( eTag tag )
{
    QString result;
    switch(tag) {
    case PatternFormat::CameraMake:
        result = "Nikon";
        break;
    case PatternFormat::CameraModel:
        result = "D50";
        break;
    case PatternFormat::MediaType:
        result = "image";
        break;
    case PatternFormat::Year:
        result = QDate::currentDate().toString("yyyy");
        break;
    case PatternFormat::Month:
        result = QDate::currentDate().toString("MM");
        break;
    case PatternFormat::MonthS:
        result = QDate::currentDate().toString("MMM");
        break;
    case PatternFormat::MonthL:
        result = QDate::currentDate().toString("MMMM");
        break;
    case PatternFormat::WeekNumber:
        result = QString::number(QDate::currentDate().weekNumber());
        break;
    case PatternFormat::Day:
        result = QDate::currentDate().toString("dd");
        break;
    case PatternFormat::DayS:
        result = QDate::currentDate().toString("ddd");
        break;
    case PatternFormat::DayL:
        result = QDate::currentDate().toString("dddd");
        break;
    case PatternFormat::Hour:
        result = QTime::currentTime().toString("hh");
        break;
    case PatternFormat::Minute:
        result = QTime::currentTime().toString("mm");
        break;
    case PatternFormat::Second:
        result = QTime::currentTime().toString("ss");
        break;
    case PatternFormat::Filename:
        result = "P0001";
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

