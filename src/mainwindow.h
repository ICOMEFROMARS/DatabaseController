#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread> // QThread ekliyoruz
#include "DatabaseController.h"
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void addUser();
    void updateUser();
    void deleteUser();
    void refreshTable();

private:
    Ui::MainWindow* ui;
    DatabaseController* dbController; // Pointer olarak değiştirdik
    QThread* dbThread; // Veritabanı işlemleri için bir QThread ekledik
    QSqlTableModel* model;
};

#endif // MAINWINDOW_H
