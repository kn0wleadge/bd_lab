#include "report.h"

Report::Report()
{}

QString Report::getGuide()
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

Report::Report(QString guide, QString text, QString sql):reportGuide(guide), reportText(text), reportSql(sql)
{

}
