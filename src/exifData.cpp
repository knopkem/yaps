/*
 * Copyright (C) 2014 Michael Knopke
 *
 * See the COPYRIGHT file for terms of use.
 */

#include "exifData.h"

#include <QDebug>

//--------------------------------------------------------------------------------------

ExifDate::ExifDate( const QString& input )
{
    int y = parseYear(input);
    int M = parseMonth(input);
    int d = parseDay(input);

    int h = parseHour(input);
    int m = parseMinute(input);
    int s = parseSecond(input);

    m_date =  QDateTime(QDate(y,M,d), QTime(h,m,s));
}

//--------------------------------------------------------------------------------------

QString ExifDate::year() const
{
    return m_date.date().toString("yyyy");
}

//--------------------------------------------------------------------------------------

QString ExifDate::month() const
{
    return m_date.date().toString("MM");
}

//--------------------------------------------------------------------------------------

QString ExifDate::monthS() const
{
    return m_date.date().toString("MMM");
}

//--------------------------------------------------------------------------------------

QString ExifDate::monthL() const
{
    return m_date.date().toString("MMMM");
}

//--------------------------------------------------------------------------------------

QString ExifDate::weekNumber() const
{
    return QString::number(m_date.date().weekNumber()).rightJustified(2, '0');
}

//--------------------------------------------------------------------------------------

QString ExifDate::day() const
{
    return m_date.date().toString("dd");
}

//--------------------------------------------------------------------------------------

QString ExifDate::dayS() const
{
    return m_date.date().toString("ddd");
}

//--------------------------------------------------------------------------------------

QString ExifDate::dayL() const
{
    return m_date.date().toString("dddd");
}

//--------------------------------------------------------------------------------------

QString ExifDate::hour() const
{
    return m_date.time().toString("hh");
}

//--------------------------------------------------------------------------------------

QString ExifDate::minute() const
{
    return m_date.time().toString("mm");
}

//--------------------------------------------------------------------------------------

QString ExifDate::second() const
{
    return m_date.time().toString("ss");
}

//--------------------------------------------------------------------------------------

bool ExifDate::isInvalid() const
{
    return !m_date.isValid();
}

//--------------------------------------------------------------------------------------

int ExifDate::parseYear(const QString& input)
{
    QString cp = input;
    return cp.left(4).toInt();
}

//--------------------------------------------------------------------------------------

int ExifDate::parseMonth(const QString& input)
{
    QString cp = input;
    return cp.remove(0,5).left(2).toInt();
}

//--------------------------------------------------------------------------------------

int ExifDate::parseDay(const QString& input)
{
    QString cp = input;
    return cp.remove(0,8).left(2).toInt();
}

//--------------------------------------------------------------------------------------

int ExifDate::parseHour(const QString& input)
{
    QString cp = input;
    return cp.remove(0,11).left(2).toInt();
}

//--------------------------------------------------------------------------------------

int ExifDate::parseMinute(const QString& input)
{
    QString cp = input;
    return cp.remove(0,14).left(2).toInt();
}

//--------------------------------------------------------------------------------------

int ExifDate::parseSecond(const QString& input)
{
    QString cp = input;
    return cp.remove(0,17).left(2).toInt();
}

//--------------------------------------------------------------------------------------

