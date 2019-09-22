#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

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

private:
    Ui::MainWindow *ui;
    QSqlDatabase m_db;
    QSqlTableModel* model;
    QSqlQuery* query;

};
#endif // MAINWINDOW_H
