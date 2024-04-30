#include "report.h"

Report::Report()
{}

Report::~Report()
{

}

QStringList Report::getParamsNameList()
{
    return this->paramsNameList;
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

Report::Report(QString text,QStringList guide,  QString sql, QStringList params):reportGuide(guide), reportText(text), reportSql(sql), paramsNameList(params)
{

}
