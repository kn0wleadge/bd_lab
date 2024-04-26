#include "report.h"

Report::Report()
{}

Report::~Report()
{

}

QStringList Report::getGuide()
{
    return this->reportGuide;
}

QString Report::getText()
{
    return this->reportText;
}

QString Report::getSql()
{
    return this->reportSql;
}

Report::Report(QString text,QStringList guide,  QString sql):reportGuide(guide), reportText(text), reportSql(sql)
{

}
