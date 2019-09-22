#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include "parser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_db = QSqlDatabase::addDatabase("QSQLITE");

    m_db.setDatabaseName("myDB");
    if(!m_db.open()){
        throw "Can't open database";
    }

    query = new QSqlQuery(m_db);

    if(!m_db.tables().contains("Book")){
        query->clear();
        query->exec("Create TABLE IF NOT EXISTS Book(id integer primary key, Title text, Author text, Isbn integer);");
    }

    model = new QSqlTableModel(this,m_db);
    model->setTable("Book");
    model->select();
    model->setEditStrategy(QSqlTableModel::OnFieldChange);

    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete query;
    delete model;
}


void MainWindow::on_importFile_clicked()
{
    QDir dir = QFileDialog::getExistingDirectory(this, "Выбор файла", "C://");
    QStringList files = dir.entryList(QStringList()<<"*.xml"<<"*.XML", QDir::Files);
    Parser* parser = new Parser(*query);
    QString path = dir.path();
    parser->parseFolder(files, path);
    qDebug()<<"";

}
