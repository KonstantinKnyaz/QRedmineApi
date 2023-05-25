#include "qredmineapi.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

QRedmineApi::QRedmineApi(const QString &serverRedmine, const QString &userName, const QString &password, QObject *parent) : QObject(parent)
  ,_serverRedmine(serverRedmine)
{
    sender = new RedmineRestApiSender(serverRedmine, userName, password, this);
    if(_serverRedmine.right(1) == "/")
        _serverRedmine.chop(1);
    wikiUrl = _serverRedmine+wikiUrl;

    QObject::connect(sender, &RedmineRestApiSender::tokenCome, this, &QRedmineApi::getToken);
}

void QRedmineApi::sendIssue(Issue issue, const QString &Autor, int id)
{
    QJsonObject attr;

    if( issue.project.id != NULL_ID )
        attr["project_id"] = issue.project.id;

    if( issue.tracker.id != NULL_ID )
        attr["tracker_id"] = issue.tracker.id;

    if( issue.status.id != NULL_ID )
        attr["status_id"] = issue.status.id;

    if( issue.priority.id != NULL_ID )
        attr["priority_id"] = issue.priority.id;

    if( !issue.subject.isEmpty() )
        attr["subject"] = issue.subject;

    if( !issue.description.isEmpty() )
        attr["description"] = issue.description;

    if( issue.category.id != NULL_ID )
        attr["category_id"] = issue.category.id;

    if( issue.version.id != NULL_ID )
        attr["fixed_version_id"] = issue.version.id;

    if( issue.assignedTo.id != NULL_ID )
        attr["assigned_to_id"] = issue.assignedTo.id;

    if( issue.parentId != NULL_ID )
        attr["parent_issue_id"] = issue.parentId;

    if( issue.startDate.isValid() )
        attr["start_date"] = issue.startDate.toString( "yyyy-MM-dd" );

    if( issue.dueDate.isValid() )
        attr["due_date"] = issue.dueDate.toString( "yyyy-MM-dd" );

    if( issue.uploads.size() )
    {
        QJsonArray uploads;

        for( const auto& upload : issue.uploads )
        {
            QJsonObject up;
            up["token"] = upload.token;
            up["filename"] = upload.filename;
            up["content_type"] = upload.content_type;
            uploads.append( up );
        }

        attr["uploads"] = uploads;
    }

    if( issue.customFields.size() )
    {
        QJsonArray customFields;

        for( const auto& customField : issue.customFields )
        {
            QJsonObject cf;
            cf["id"] = customField.id;
            cf["value"] = customField.values.at( 0 );
            customFields.append( cf );
        }

        attr["custom_fields"] = customFields;
    }

    if( issue.estimatedHours )
        attr["estimated_hours"] = issue.estimatedHours;

    QJsonObject data;
    data["issue"] = attr;

    QJsonDocument json;
    json.setObject( data );

    sender->sendIssue(json, Autor, id);
}

// Title лучше указывать и как название файла. Чтобы обновить страничку, надо вписать поля автора, title и text
void QRedmineApi::createOrUpdateWiki(Wiki_st wiki, const QString &wikiName)
{
    QJsonObject attr;

    if( !wiki.title.isEmpty() )
        attr["title"] = wiki.title;

    if( !wiki.text.isEmpty() )
        attr["text"] = wiki.text;

    if (wiki.version != NULL_ID)
        attr["version"] = wiki.version;

    if( wiki.uploads.size() )
    {
        QJsonArray uploads;

        for( const auto& upload : wiki.uploads )
        {
            QJsonObject up;
            up["token"] = upload.token;
            up["filename"] = upload.filename;
            up["content_type"] = upload.content_type;
            uploads.append( up );
        }

        attr["uploads"] = uploads;
    }

    QJsonObject author;

    if(wiki.author.user.id != NULL_ID)
        author["id"] = wiki.author.user.id;
    if(!wiki.author.user.name.isEmpty())
        author["name"] = wiki.author.user.name;

    if(!author.isEmpty())
        attr["author"] = author;

    QJsonObject data;
    data["wiki_page"] = attr;

    QJsonDocument json;
    json.setObject(data);

    QString url = wikiUrl.append(wikiName);

    sender->createOrUpdateWiki(json, url);
}

void QRedmineApi::setNewWikiUrl(const QString &url)
{
    QString _url = url;
    if(_url.contains(_serverRedmine))
        wikiUrl = _url;
    else {
        if(_url.left(1) != "/")
            _url.prepend("/");
        wikiUrl = _serverRedmine+_url;
    }

    if(wikiUrl.right(1) != "/") {
        wikiUrl.append("/");
    }
}

FileToken QRedmineApi::uploadFile(const QString &filePath)
{
    return sender->sendFile(filePath);
}

void QRedmineApi::getToken(FileToken token)
{
    emit tokenCome(token);
}

