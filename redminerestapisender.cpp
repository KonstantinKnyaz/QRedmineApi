#include "redminerestapisender.h"

#include <QFile>
#include <QFileInfo>
#include <QPointer>

#define ISSUE           "issues"
#define UPLOADS         "uploads"
#define FILE_TOKEN      "token"
#define WIKI            "wiki"
#define TEN_SECOND      10000

RedmineRestApiSender::RedmineRestApiSender(const QString &serverRedmine, const QString &userName, const QString &password, QObject *parent)
    : QObject(parent),
    _serverRedmine(serverRedmine),
    _userName(userName),
    _password(password)
{
    manager = new QNetworkAccessManager(this);
    // Если вдруг обычная авторизация не сработает, то она пройдёт по сигналу
    QObject::connect(manager, &QNetworkAccessManager::authenticationRequired, this, &RedmineRestApiSender::authentication);
    QObject::connect(manager, &QNetworkAccessManager::finished, this, &RedmineRestApiSender::response);
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
}

void RedmineRestApiSender::sendIssue(const QJsonDocument &data, const QString &Autor, const int id)
{
    QString resource = ISSUE;

    int TypeRequest = POST;

    if(id != -1) {
        resource.append("/").append(QString::number(id));
        TypeRequest = PUT;
    }

    QString url = _serverRedmine + "/" + resource + ".json?";

    sendRequest(data.toJson(), url, TypeRequest, Autor);
}

void RedmineRestApiSender::createOrUpdateWiki(const QJsonDocument &data, const QString url)
{
    sendRequest(data.toJson(), url, PUT, Q_NULLPTR);
}

FileToken RedmineRestApiSender::sendFile(const QString &filePath)
{
    QFile *file = new QFile(filePath);
    QPointer<QFile> pFile(file);

    if(!pFile->open(QIODevice::ReadOnly)) {
        qCritical() << pFile->errorString();
        return FileToken();
    }

    QFileInfo info(*pFile);
    QString fileName = info.fileName();

    QByteArray data = pFile->readAll();
    pFile->close();

    return getToken(data, fileName);
}

void RedmineRestApiSender::authentication(QNetworkReply *rpl, QAuthenticator *auth)
{
    qDebug() << "Авторизация ПОЛЬЗОВАТЕЛЯ";
    Q_UNUSED(rpl);
    if(rpl->error() != QNetworkReply::NoError) {
        qCritical() << rpl->errorString();
        return;
    }
    auth->setUser(_userName);
    auth->setPassword(_password);
}

void RedmineRestApiSender::response(QNetworkReply *rpl)
{
    if(rpl->error() != QNetworkReply::NoError) {
        qCritical() << rpl->errorString() << rpl->error();
        if(loop.isRunning())
            loop.quit();
        return;
    }

    QString response = rpl->readAll();

    if(response.contains("token")) {
        QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject obj = doc.object();
        QJsonObject uploadObj = obj["upload"].toObject();

        newToken.id = uploadObj["id"].toInt();
        newToken.token = uploadObj["token"].toString();

        emit tokenCome(newToken);
        loop.quit();
    }

    rpl->deleteLater();
}

void RedmineRestApiSender::sendRequest(const QByteArray &data, const QString &_url, int type, const QString &Autor)
{ 
    if(manager == Q_NULLPTR) {
        qCritical() << "NetworkManager не определён";
        return;
    }

    if(data.isEmpty()) {
        qWarning() << "Данные пусты";
        return;
    }

    QUrl url = _url;

    if(!url.isValid()) {
        qCritical() << "Не валидный url";
        return;
    }

    QNetworkRequest request(url);
    QByteArray auth = QByteArray(QString("%1:%2").arg(_userName).arg(_password).toLatin1().toBase64());
    request.setRawHeader( "Content-Type",        "application/json" );
    request.setRawHeader( "Content-Length",      QByteArray::number(data.size()));
    if(!Autor.isNull())
        request.setRawHeader( "X-Redmine-Switch-User", Autor.toUtf8());
    request.setRawHeader( "Authorization", "Basic " + auth );

    switch(type) {
        case POST:
            manager->post(request, data);
        break;
        case PUT:
            manager->put(request, data);
        break;
        default:
        break;
    }
}

FileToken RedmineRestApiSender::getToken(const QByteArray &data, const QString &FileName)
{
    if(manager == Q_NULLPTR) {
        qCritical() << "NetworkManager не определён";
        return FileToken();
    }

    if(data.isEmpty()) {
        qWarning() << "Данные пусты";
        return FileToken();
    }

    size_t size = data.size();

    QString strUrl = _serverRedmine;
    if(strUrl.right(1) != "/")
        strUrl.append("/");

    strUrl.append(defaultFileUpdatePath);
    strUrl.append(FileName);

    QUrl url(strUrl);

    if(!url.isValid()) {
        qCritical() << "Не валидный url";
        return FileToken();
    }

    QNetworkRequest req(url);
    QByteArray auth = QByteArray(QString("%1:%2").arg(_userName).arg(_password).toLatin1().toBase64());
    req.setRawHeader("Content-Type",        "application/octet-stream");
    req.setRawHeader("Content-Length",      QByteArray::number(size));
    req.setRawHeader( "Authorization", "Basic " + auth );

    newToken.clear();

    manager->post(req, data);

    timer.setSingleShot(true);
    timer.start(TEN_SECOND);

    loop.exec();

    FileToken retToken;

    retToken.id = newToken.id;
    retToken.token = newToken.token;

    return retToken;
}
