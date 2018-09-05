#include "mysql.h"

mysql::mysql()
{

}

QSqlDatabase mysql::connect_db(QString name, QString host, int port, QString db_name, QString username, QString password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", name);
    db.setHostName(host);
    db.setPort(port);
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
//        q.prepare(query);
        for (int i = 0; i < parameter.length(); i++) {
//            if (query.indexOf("':p"+QString::number(i+1)+"'") > 0) {
//                query.replace("':p"+QString::number(i+1)+"'", parameter.at(i));
//            }
            if (query.indexOf(":p"+QString::number(i+1)) > 0) {
                query.replace(":p"+QString::number(i+1), parameter.at(i));
            }
        }
        if (q.exec(query)) {
            while(q.next()) {
                result.append(q.value(0).toString());
            }
            if (result.isEmpty()) {
//                result.append("Query Error!::Hasil dari Query Kosong: "+query);
                result.append("-");
            }
        } else {
            result.append("Query Error!::Query Salah: "+query);
//            printf("Error: Cannot Execute query: %s\n", query.toUtf8().data());
        }
        db.close();
    }
    return result;
}

QStringList mysql::read_m(QSqlDatabase db, QString query, QStringList parameter)
{
    QStringList result;
    if (db.isValid()) {
        if (!db.isOpen()) {
            db.open();
        }
        QSqlQuery q(QSqlDatabase::database(db.connectionName()));;
//        qDebug() << query;
//        q.prepare(query);
        for (int i = 0; i < parameter.length(); i++) {
//            if (query.indexOf("':p"+QString::number(i+1)+"'") > 0) {
//                query.replace("':p"+QString::number(i+1)+"'", parameter.at(i));
//            }
            if (query.indexOf(":p"+QString::number(i+1)) > 0) {
                query.replace(":p"+QString::number(i+1), parameter.at(i));
            }
        }
        if (q.exec(query)) {
            while(q.next()) {
                for (int i = 0; i < q.record().count(); i++) {
                    result.append(q.value(i).toString());
                }
            }
            if (result.isEmpty()) {
//                result.append("Query Error!::Hasil dari Query Kosong: "+query);
                result.append("-");
            }
        } else {
            result.append("Query Error!::Query Salah: "+query);
//            printf("Error: Cannot Execute query: %s\n", query.toUtf8().data());
        }
        db.close();
    }
    return result;
}
