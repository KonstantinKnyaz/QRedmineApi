#ifndef WIKISTRUCT_H
#define WIKISTRUCT_H

#include <QString>

#include "IssueStruct.h"

struct Wiki_st
{
    QString title;

    QString parentTitle;

    QString text;
    int version;

    RedmineResource author;

    QString comments;

    Uploads uploads;
};

#endif // WIKISTRUCT_H
