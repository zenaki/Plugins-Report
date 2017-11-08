#include "mysql.h"

mysql::mysql()
{

}

QSqlDatabase mysql::connect_db(QString name, QString host, QString db_name, QString username, QString password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", name);
    db.setHostName(host);
    db.setDatabaseName(db_name);
    db.setUserName(username);
    db.setPassword(password);

    return (QSqlDatabase) db;
}

void mysql::close(QSqlDatabase db)
{
    QString connection;
    connection = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(connection);
}

QStringList mysql::read(QSqlDatabase db, QString query, QStringList parameter)
{
    QStringList result;
    if (db.isValid()) {
        if (!db.isOpen()) {
            db.open();
        }
        QSqlQuery q(QSqlDatabase::database(db.connectionName()));;
//        qDebug() << query;
        q.prepare(query);
        for (int i = 0; i < parameter.length(); i++) {
            if (query.indexOf(":p"+QString::number(i)) > 0) {
                q.bindValue(":p"+QString::number(i), parameter.at(i));
            }
        }
        if (q.exec()) {
            while(q.next()) {
                result.append(q.value(0).toString());
            }
        }
        db.close();
    }
    return result;
}
