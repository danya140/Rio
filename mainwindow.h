#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QFileDialog>
#include "parser.h"
#include "book.h"
#include "exporter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_importFile_clicked();
    void on_clearTable_clicked();
    void slotCustomMenuRequested(QPoint pos);
    void addRow();
    void removeRow();
    void exportToXml();

private:
    Ui::MainWindow *ui;
    QSqlDatabase m_db;
    QSqlTableModel* model;
    QSqlQuery* query;

    void afterImportUpdate(QList<Book> books);
    void createTable();
    void dropTable();
    void insert(QString title, QString author, int isbn);
};
#endif // MAINWINDOW_H
