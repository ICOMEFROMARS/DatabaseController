#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QObject>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariantList>
#include <QVariantMap>

class DatabaseController : public QObject {
    Q_OBJECT

public:
    explicit DatabaseController(QObject* parent = nullptr);
    ~DatabaseController();

    Q_INVOKABLE bool openDatabase(const QString& dbName);
    Q_INVOKABLE bool createTable();
    Q_INVOKABLE bool insertData(const QString& name, int age, int id);
    Q_INVOKABLE bool updateUser(int id, const QString& name, int age);
    Q_INVOKABLE bool deleteUser(int id);

    void closeDatabase();
    QVariantList getAllUsers();
    bool executeQuery(const QString& queryStr);

private:
    QSqlDatabase db;
};


#endif // DATABASECONTROLLER_H
