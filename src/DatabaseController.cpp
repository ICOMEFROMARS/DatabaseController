#include "DatabaseController.h"

DatabaseController::DatabaseController(QObject* parent) : QObject(parent) {
    qDebug() << "DatabaseController olustu.";
}

DatabaseController::~DatabaseController() {
    closeDatabase();
    qDebug() << "DatabaseController yok edildi.";
}

bool DatabaseController::openDatabase(const QString& dbName) {
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbName);
    }

    if (!db.open()) {
        qDebug() << "Veritabani acma hatasi:" << db.lastError().text();
        return false;
    }

    qDebug() << "veritabani basariyla acildi.open_database: " << dbName;
    return true;
}

void DatabaseController::closeDatabase() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "veritabani kapatildi.";
    }
}

bool DatabaseController::executeQuery(const QString& queryStr) {
    if (!db.isOpen()) {
        qDebug() << "Hata: Veritabani acik degil.";
        return false;
    }

    QSqlQuery query(db);
    if (!query.exec(queryStr)) {
        qDebug() << "SQL hatasi: " << query.lastError().text();
        return false;
    }

    qDebug() << "SQL sorgusu basariyla calistirildi.executeQuery: " << queryStr;
    return true;
}

bool DatabaseController::insert_data(const QString& tableName, const QMap<QString, QString>& data) {
    if (!db.isOpen()) return false;

    QThread::sleep(10);                                                                                                         // 10 saniye uyku
    QStringList columns = data.keys();
    QStringList values;
    for (const QString& key : columns) {
        values.append("'" + data.value(key) + "'");
    }

    QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
        .arg(tableName)
        .arg(columns.join(", "))
        .arg(values.join(", "));

    return executeQuery(sql);
}

bool DatabaseController::update_user(const QString& tableName, int id, const QMap<QString, QString>& data) {
    if (!db.isOpen()) return false;

    QStringList updateStatements;
    for (auto it = data.begin(); it != data.end(); ++it) {
        updateStatements.append(QString("%1='%2'").arg(it.key()).arg(it.value()));
    }

    QString sql = QString("UPDATE %1 SET %2 WHERE id=%3")
        .arg(tableName)
        .arg(updateStatements.join(", "))
        .arg(id);

    return executeQuery(sql);
}

bool DatabaseController::delete_user(const QString& tableName, int id) {
    if (!db.isOpen()) return false;

    QString sql = QString("DELETE FROM %1 WHERE id=%2")
        .arg(tableName)
        .arg(id);

    return executeQuery(sql);
}


QSqlDatabase& DatabaseController::getDatabase() {
    return db;
}