#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DatabaseController.h"
#include <QSqlQueryModel>
#include <QMainWindow>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QThread>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void open_database_file();
    void refresh_table();
    void add_user();
    void update_user();
    void delete_user();

private:
    Ui::MainWindow*      ui;
    QSqlQueryModel*      model;
    DatabaseController*  dbController;

    QString tableName;
    QMap<QString, QLineEdit*> field_inputs;

    void generate_input_fields();
};

#endif // MAINWINDOW_H
