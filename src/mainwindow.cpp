#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QThread>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(new QSqlTableModel(this)) {
    ui->setupUi(this);

    // Veritabanını aç ve tabloyu oluştur
    dbController = new DatabaseController();
    dbController->openDatabase("test.db");
    dbController->createTable();

    // UI'yi güncelle
    refreshTable();

    // Buton sinyallerini bağla
    connect(ui->add_button, &QPushButton::clicked, this, &MainWindow::addUser);
    connect(ui->update_button, &QPushButton::clicked, this, &MainWindow::updateUser);
    connect(ui->delete_button, &QPushButton::clicked, this, &MainWindow::deleteUser);
    connect(ui->refresh_button, &QPushButton::clicked, this, &MainWindow::refreshTable);
}

// 🔹 Kullanıcı ekleme fonksiyonu (Her seferinde yeni thread açar)
void MainWindow::addUser() {
    QString name = ui->name_edit->text();
    int age = ui->age_edit->text().toInt();
    int id = ui->id_edit->text().toInt();

    QThread* thread = QThread::create([this, name, age, id]() {
        dbController->insertData(name, age, id);
        QMetaObject::invokeMethod(this, "refreshTable", Qt::QueuedConnection);
        });

    thread->start();
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
}

// 🔹 Kullanıcı güncelleme fonksiyonu (Her seferinde yeni thread açar)
void MainWindow::updateUser() {
    int id = ui->id_edit->text().toInt();
    QString name = ui->name_edit->text();
    int age = ui->age_edit->text().toInt();

    QThread* thread = QThread::create([this, id, name, age]() {
        dbController->updateUser(id, name, age);
        QMetaObject::invokeMethod(this, "refreshTable", Qt::QueuedConnection);
        });

    thread->start();
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
}

// 🔹 Kullanıcı silme fonksiyonu (Her seferinde yeni thread açar)
void MainWindow::deleteUser() {
    int id = ui->id_edit->text().toInt();

    QThread* thread = QThread::create([this, id]() {
        dbController->deleteUser(id);
        QMetaObject::invokeMethod(this, "refreshTable", Qt::QueuedConnection);
        });

    thread->start();
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
}

// 🔹 Tabloyu güncelleme fonksiyonu (UI thread içinde kalır)
void MainWindow::refreshTable() {
    if (model) {
        delete model;
    }
    model = new QSqlTableModel(this);
    model->setTable("Users");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();

    ui->user_table->setModel(model);
    ui->user_table->resizeColumnsToContents();
}

MainWindow::~MainWindow() {
    delete dbController;
    delete ui;
    delete model;
}
