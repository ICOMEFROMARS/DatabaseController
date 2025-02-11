#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QSqlDatabase>
#include <QVariantList>
#include <QVariantMap>
#include <QSqlQuery>
#include <QSqlError>
#include <QObject>
#include <QThread>
#include <QDebug>

class DatabaseController : public QObject {
    Q_OBJECT

public:
    explicit DatabaseController(QObject* parent = nullptr);
    ~DatabaseController();

    
    QSqlDatabase& getDatabase();

    Q_INVOKABLE bool openDatabase(const QString& dbName);
    //Q_INVOKABLE bool createTable();
    Q_INVOKABLE bool insert_data(const QString& tableName, const QMap<QString, QString>& data);
    Q_INVOKABLE bool update_user(const QString& tableName, int id, const QMap<QString, QString>& data);
    Q_INVOKABLE bool delete_user(const QString& tableName, int id);

    void closeDatabase();
    //QVariantList getAllUsers();
    bool executeQuery(const QString& queryStr);

private:
    QSqlDatabase db;
};

#endif // DATABASECONTROLLER_H
