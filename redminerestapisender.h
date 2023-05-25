#ifndef REDMINERESTAPISENDER_H
#define REDMINERESTAPISENDER_H

#include <QObject>

#include <QEventLoop>
#include <QTimer>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

/*!
 * \brief The FileToken struct структура, которая хранти в себе данные *токена
 */
struct FileToken {
    void operator= (const FileToken &anotherToken) {
        this->id = anotherToken.id;
        this->token = anotherToken.token;
    };

    void clear()
    {
        id = 0;
        token = Q_NULLPTR;
    };

    bool isNull()
    {
        if(id == 0 && token.isNull())
            return true;
        return false;
    };

    int id = 0;
    QString token = Q_NULLPTR;
};
/*!
 * \brief The RedmineRestApiSender class отвечающий за непосредственную отправку данных в Редмайн
 */
class RedmineRestApiSender : public QObject
{
    /*!
     * \brief The TYPE_REQUEST enum типа отправки данных
     */
    enum TYPE_REQUEST {
        PUT = 0,
        POST = 1
    };

    Q_OBJECT
public:
    RedmineRestApiSender(const QString &serverRedmine, const QString &userName, const QString &password, QObject *parent);

    void sendIssue(const QJsonDocument& data, const QString &Autor, const int id = -1);

    void createOrUpdateWiki(const QJsonDocument& data, const QString url);

    FileToken sendFile(const QString &filePath);

private slots:
    void authentication(QNetworkReply *rpl, QAuthenticator *auth);
    void response(QNetworkReply *rpl);

signals:
    /*!
     * \brief tokenCome сигнал испускается, когда Редмайн вернул *токен
     * \param token
     */
    void tokenCome(FileToken token);

private:
    void sendRequest(const QByteArray &data, const QString &_url, int type, const QString &Autor);

    FileToken getToken(const QByteArray &data, const QString &FileName);

    QString _serverRedmine,
    _userName,
    _password;

    QNetworkAccessManager *manager = Q_NULLPTR;

    /*!
     * \brief defaultFileUpdatePath дефолтное значение конца url для загрузки файла
     */
    QString defaultFileUpdatePath = "uploads.json?filename=";
    /*!
     * \brief newToken вернувийся токен (Перезаписывается каждый раз, когда возвращается токен и обнуляется перед отправкой нового файла)
     */
    FileToken newToken;

    QEventLoop loop;

    QTimer timer;
};

#endif // REDMINERESTAPISENDER_H

/// Токен - идентификатор файла загруженного на сервер Redmine. Имеет в себе id файла и номер токена.
