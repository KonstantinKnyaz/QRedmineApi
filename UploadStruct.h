#ifndef UPLOADSTRUCT_H
#define UPLOADSTRUCT_H

#include <QString>
#include <QVector>

struct Upload
{
    QString token;
    QString filename;
    QString content_type;
};

using Uploads = QVector<Upload>;

#endif // UPLOADSTRUCT_H
