#ifndef MYSQL_H
#define MYSQL_H

#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

class mysql
{
public:
    mysql();

    QString host;
    QString db_name;
    QString user_name;
    QString password;

    QSqlDatabase connect_db(QString name, QString host, QString db_name, QString username, QString password);
    void close(QSqlDatabase db);

    QStringList read(QSqlDatabase db, QString query, QStringList parameter);
};

#endif // MYSQL_H
