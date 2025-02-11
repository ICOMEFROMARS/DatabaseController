#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlQuery>
#include <QPushButton>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), dbController(new DatabaseController()), model(new QSqlQueryModel(this)) {
    ui->setupUi(this);

    open_database_file();

    connect(ui->refresh_button, &QPushButton::clicked, this, &MainWindow::refresh_table);
}

void MainWindow::open_database_file() {
    QString filePath = QFileDialog::getOpenFileName(this, "veritabani sec", "", "SQLite veritabani (*.db)");
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "hata", "veritabani secilmedi.");
        return;
    }

    if (!dbController->openDatabase(filePath)) {
        QMessageBox::critical(this, "hata", "veritabani acilmadi.");
        return;
    }

    refresh_table();
}

void MainWindow::refresh_table() {
    QSqlQuery query("SELECT name FROM sqlite_master WHERE type='table'", dbController->getDatabase());
    if (query.next()) {
        tableName = query.value(0).toString();
        model->setQuery("SELECT * FROM " + tableName, dbController->getDatabase());
        ui->user_table->setModel(model);

        generate_input_fields();
    }
    else {
        QMessageBox::warning(this, "hata", "veritabaninda tablo yok.");
    }
}

void MainWindow::generate_input_fields() {
    if (ui->left_panel->layout()) {
        QLayoutItem* item;
        while ((item = ui->left_panel->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }
    else {
        ui->left_panel->setLayout(new QVBoxLayout());
    }

    QFormLayout* formLayout = new QFormLayout();
    formLayout->setSpacing(8);

    QSqlQuery query("PRAGMA table_info(" + tableName + ")", dbController->getDatabase());

    while (query.next()) {
        QString columnName = query.value(1).toString();

        QLabel* label = new QLabel(columnName, this);
        QLineEdit* lineEdit = new QLineEdit(this);
        field_inputs[columnName] = lineEdit;

        formLayout->addRow(label, lineEdit);
    }

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    QPushButton* addButton = new QPushButton("Add", this);
    QPushButton* updateButton = new QPushButton("Update", this);
    QPushButton* deleteButton = new QPushButton("Delete", this);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::add_user);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::update_user);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::delete_user);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);

    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(ui->left_panel->layout());
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(12);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);
}

void MainWindow::add_user() {
    if (tableName.isEmpty()) {
        QMessageBox::warning(this, "hata", "once veritabani acilmali.");
        return;
    }

    QMap<QString, QString> data;
    for (auto it = field_inputs.begin(); it != field_inputs.end(); ++it) {
        data[it.key()] = it.value()->text();
    }

    QThread* thread = QThread::create([this, data]() {
        dbController->insert_data(tableName, data);
        QMetaObject::invokeMethod(this, "refresh_table", Qt::QueuedConnection);
        });

    thread->start();
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
}

void MainWindow::update_user() {
    if (tableName.isEmpty()) {
        QMessageBox::warning(this, "hata", "once veritabani acilmali.");
        return;
    }

    if (!field_inputs.contains("id")) {
        QMessageBox::warning(this, "hata", "guncelleme icin id alani olmali.update_user");
        return;
    }

    int id = field_inputs["id"]->text().toInt();
    if (id == 0) {
        QMessageBox::warning(this, "hata", "id bos veya gecersiz");
        return;
    }

    QMap<QString, QString> data;
    for (auto it = field_inputs.begin(); it != field_inputs.end(); ++it) {
        if (it.key() != "id") {
            data[it.key()] = it.value()->text();
        }
    }

    QThread* thread = QThread::create([this, id, data]() {
        dbController->update_user(tableName, id, data);
        QMetaObject::invokeMethod(this, "refresh_table", Qt::QueuedConnection);
        });

    thread->start();
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
}

void MainWindow::delete_user() {
    if (tableName.isEmpty()) {
        QMessageBox::warning(this, "hata", "once veritabani acilmali.");
        return;
    }

    if (!field_inputs.contains("id")) {
        QMessageBox::warning(this, "hata", "silme islemi icin id alani olmali.delete_user");
        return;
    }

    int id = field_inputs["id"]->text().toInt();
    if (id == 0) {
        QMessageBox::warning(this, "Hata", "id bos veya gecersiz");
        return;
    }

    QThread* thread = QThread::create([this, id]() {
        dbController->delete_user(tableName, id);
        QMetaObject::invokeMethod(this, "refresh_table", Qt::QueuedConnection);
        });

    thread->start();
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
}

MainWindow::~MainWindow() {
    delete dbController;
    delete ui;
}
