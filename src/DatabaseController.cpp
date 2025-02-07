#include "DatabaseController.h"
#include "QThread"

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
        qDebug() << "veritabani acma hatasi:" << db.lastError().text();
        return false;
    }

    qDebug() << "veritabani basariyla acildi.open_database: " << dbName;
    return true;
}

void DatabaseController::closeDatabase() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Veritabani kapatildi.";
    }
}

bool DatabaseController::executeQuery(const QString& queryStr) {
    if (!db.isOpen()) {
        qDebug() << "Hata: Veritabani acik degil!";
        return false;
    }

    QSqlQuery query(db);
    if (!query.exec(queryStr)) {
        qDebug() << "SQL Hatasi: " << query.lastError().text();
        return false;
    }

    qDebug() << "SQL sorgusu basariyla calistirildi: " << queryStr;
    return true;
}

bool DatabaseController::createTable() {
    QString sql = "CREATE TABLE IF NOT EXISTS Users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "age INTEGER)";
    return executeQuery(sql);
}

bool DatabaseController::insertData(const QString& name, int age, int id) {

    qInfo("inserdata_thread = %p", QThread::currentThreadId());
    QThread::sleep(15); // 2 saniye uyku
    QString sql = QString("INSERT INTO Users (name, age, id) VALUES ('%1', %2, %3)")
        .arg(name)
        .arg(age)
        .arg(id);
    return executeQuery(sql);
}

QVariantList DatabaseController::getAllUsers() {
    QVariantList users;
    if (!db.isOpen()) {
        qDebug() << "veritabani acik degil.get_all_user";
        return users;
    }

    QSqlQuery query("SELECT id, name, age FROM Users", db);
    while (query.next()) {
        QVariantMap user;
        user["id"] = query.value(0);
        user["name"] = query.value(1);
        user["age"] = query.value(2);
        users.append(user);
    }

    return users;
}

bool DatabaseController::updateUser(int id, const QString& name, int age) {
    qInfo("update_thread = %p", QThread::currentThreadId());
    QString sql = QString("UPDATE Users SET name='%1', age=%2 WHERE id=%3")
        .arg(name)
        .arg(age)
        .arg(id);
    return executeQuery(sql);
}

bool DatabaseController::deleteUser(int id) {
    QString sql = QString("DELETE FROM Users WHERE id=%1").arg(id);
    return executeQuery(sql);
}
