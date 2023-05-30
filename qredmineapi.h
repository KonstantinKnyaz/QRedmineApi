#ifndef RESTAPI_H
#define RESTAPI_H

#include <QObject>

#include "IssueStruct.h"
#include "redminerestapisender.h"
#include "WikiStruct.h"

/*!
 * \brief The QRedmineApi class для работы с Redmine по средством REST API
 */
class QRedmineApi : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief QRedmineApi Конструктор, в который передаём ссылку на сервер Редмайна формата "http://www.RedmineServer.ru", логин админа и пароль админа
     * \param serverRedmine параметр ссылки на сервер
     * \param userName логин пользователя
     * \param password пароль пользователя
     * \param parent ссылка на родительский класс
     */
    QRedmineApi(const QString &serverRedmine, const QString &userName, const QString &password, QObject *parent = Q_NULLPTR);

    /*!
     * \brief sendIssue функция создания или обновления задачи
     * \param issue Структура задачи
     * \param Autor Автор задачи (Если указан этот параметр, то пользователь, создающий задачу, должен обладать правами администратора, иначе будет ошибка)
     * \param id id задачи (Если указан этот параметр, то обновится уже существующая задача с данным id)
     */
    void sendIssue(Issue issue, const QString &Autor = Q_NULLPTR, int id = NULL_ID);

    /*!
     * \brief createOrUpdateWiki функция создания или обновления wiki странички
     * \param wiki структура с данными вики
     * \param wikiName Название вики странички в url
     */
    void createOrUpdateWiki(Wiki_st wiki, const QString &wikiName);

    /*!
     * \brief setNewWikiUrl устанавливает новое дефолтное значение для ссылки где находятся странички wiki
     * \param url
     */
    void setNewWikiUrl(const QString& url);

    /*!
     * \brief uploadFile функция отправки файла на сервер Redmine
     * \param filePath полный путь до файла
     * \return возврщает структуру с токеном файла
     */
    FileToken uploadFile(const QString &filePath);

private slots:
    /*!
     * \brief getToken слот получает токен от RedmineRestApiSender
     * \param token структура с токеном
     */
    void getToken(FileToken token);

signals:
    /*!
     * \brief tokenCome сигнал издаётся, когда получаем токен от RedmineRestApiSender
     * \param token структура с токеном
     */
    void tokenCome(FileToken token);

private:
    RedmineRestApiSender *sender = Q_NULLPTR;

    QString _serverRedmine;

    QString wikiUrl = "/projects/wiki/"; // Дефолтное значение
};

#endif // RESTAPI_H
