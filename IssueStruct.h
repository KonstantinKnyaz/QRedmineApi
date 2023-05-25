#ifndef ISSUESTRUCT_H
#define ISSUESTRUCT_H

#define NULL_ID -1

#include <QDateTime>
#include <QVector>

#include "UploadStruct.h"

struct Item {
    int     id = NULL_ID; ///< ID
    QString name;         ///< Name
};

using Items = QVector<Item>;

struct RedmineResource
{
    QDateTime createdOn; ///< Created on
    QDateTime updatedOn; ///< Updated on
    Item      user;      ///< Redmine user
};

struct Group : RedmineResource
{
    int     id = NULL_ID; ///< ID
    QString name;         ///< Group name
    Items   members;      ///< Group members
};

struct CustomField // no RedmineResource
{
    int                  id = NULL_ID; ///< ID
    QString              name;         ///< Name

    QVector<QString>     values;         ///< Value(s)
    QVector<QString>     possibleValues; ///< Possible
    QString              defaultValue;   ///< Default value

    QString type;   ///< Customised type
    QString format; ///< Field format
    QString regex;  ///< Regular expression
    int minLength;  ///< Minimum length
    int maxLength;  ///< Maximum length

    bool allProjects; ///< Custom field may be used by all projects
    bool isRequired;  ///< Custom field is required
    bool isFilter;    ///< Custom field may be used as filter
    bool searchable;  ///< Custom field is searchable
    bool multiple;    ///< Custom field may contain multiple values
    bool visible;     ///< Custom field is visible

    Items projects; ///< Custom field is allowed in these projects
    Items trackers; ///< Custom field is allowed in these trackers
};

using CustomFields = QVector<CustomField>;

struct Issue : RedmineResource
{
    int          id = NULL_ID;        ///< ID
    int          parentId = NULL_ID;  ///< Parent issue ID

    QString      description;    ///< Description
    double       doneRatio = 0;  ///< Done ratio
    QString      subject;        ///< Subject

    Item         assignedTo;     ///< Assigned to user
    Item         author;         ///< Author
    Item         category;       ///< Category
    Item         priority;       ///< Priority
    Item         project;        ///< Project
    Item         status;         ///< Status
    Item         tracker;        ///< Tracker
    Item         version;        ///< Version

    QDate        dueDate;        ///< Due date
    double       estimatedHours = 0; ///< Estimated hours
    QDate        startDate;      ///< Start date

    Uploads      uploads;        ///< Uploads files vector

    CustomFields customFields;   ///< Custom fields vector
};

#endif // ISSUESTRUCT_H
