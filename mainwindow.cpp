#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTableView* tableView = ui->tableView;
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableView,SIGNAL(customContextMenuRequested(QPoint)), SLOT(slotCustomMenuRequested(QPoint)));

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("myDB");
    if(!m_db.open()){
        throw "Can't open database";
    }

    query = new QSqlQuery(m_db);
    createTable();

    model = new QSqlTableModel(this,m_db);
    model->setTable("Book");
    model->select();
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    tableView->setModel(model);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete query;
    delete model;
}

void MainWindow::createTable(){
    query->clear();
    query->exec("CREATE TABLE IF NOT EXISTS "
                "Book(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                " Title TEXT, Author TEXT,"
                " ISBN INTEGER);");
}

void MainWindow::dropTable(){
    query->clear();
    query->exec("DROP TABLE IF EXISTS Book;");
}

void MainWindow::insert(QString title = "", QString author = "", int isbn= 0){
    query->clear();
    QString qry = QString("INSERT INTO Book(Title, Author, ISBN) "
                          "VALUES('%1', '%2', %3);")
                          .arg(title, author, QString::number(isbn));

    qDebug("Execute query:");
    qDebug()<<qry;
    query->exec(qry);
}

void MainWindow::insertBooks(QList<Book> books, QStringList errors){
    delete parser;

    if(errors.size()>0){
        QMessageBox box;
        QString text = "Ошибки:";

        for(QString error : errors){
            text.append("\n");
            text.append(error);
        }
        box.setText(text);
        box.exec();

    }

    for(Book book : books)
    {
        insert(book.getTitle(), book.getAuthor(), book.getIsbn());
    }
    progress->close();
    model->select();
}



void MainWindow::on_importFile_clicked()
{
    QDir dir = QFileDialog::getExistingDirectory(this, "Выбор файла", "C://");
    QStringList files = dir.entryList(QStringList()<<"*.xml"<<"*.XML", QDir::Files);
    QString path = dir.path();

    parser = new Parser(files, path);

    connect(&parserThread, &QThread::started, parser, &Parser::run);
    connect(parser, &Parser::finished, &parserThread, &QThread::terminate);
    connect(parser, SIGNAL(finished(QList<Book>, QStringList)), this, SLOT(insertBooks(QList<Book>, QStringList)));
    connect(parser, SIGNAL(onProgress(int)), this, SLOT(onPorgress(int)));

    parser->moveToThread(&parserThread);

    parser->setRunning(true);
    parserThread.start();
    progress = new QProgressDialog("Импорт файлов", 0, 0, files.size());
    progress->exec();
}


void MainWindow::on_clearTable_clicked()
{
    dropTable();
    createTable();
    model->select();
}

void MainWindow::slotCustomMenuRequested(QPoint pos){
    QMenu* menu = new QMenu(this);

    QAction* addRowAction = new QAction("Добавить",this);
    QAction* deleteRowAction = new QAction("Удалить", this);
    QAction* exportToXmlAction = new QAction("Экспорт в xml", this);

    connect(addRowAction,SIGNAL(triggered()),this,SLOT(addRow()));
    connect(deleteRowAction,SIGNAL(triggered()),this,SLOT(removeRow()));
    connect(exportToXmlAction,SIGNAL(triggered()),this,SLOT(exportToXml()));

    menu->addAction(addRowAction);
    menu->addAction(deleteRowAction);
    menu->addAction(exportToXmlAction);
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void MainWindow::addRow(){
    insert();
    model->select();
}

void MainWindow::removeRow(){
    QItemSelectionModel* select = ui->tableView->selectionModel();
    QString selectedRowsId;
    for(QModelIndex index : select->selectedRows()){
        selectedRowsId.append(QString::number(index.data().toInt()));
        if(index != select->selectedRows().last()){
            selectedRowsId.append(",");
        }
    }
    query->clear();
    QString qry = QString("DELETE FROM Book WHERE id in (%1);")
            .arg(selectedRowsId);
    query->exec(qry);
    model->select();
}

void MainWindow::exportToXml(){
    QFileDialog saveDialog(this,"Выбор файла");
    saveDialog.setDefaultSuffix(".xml");
    saveDialog.exec();
    QDir dir(saveDialog.selectedFiles().first());

    QItemSelectionModel* select = ui->tableView->selectionModel();
    QList<Book> books;

    for(QModelIndex index : select->selectedRows()){
        Book* book = new Book();

        for(int i=1; i<model->rowCount(); i++){

            QVariant cell = model->index(index.row(),i).data();
            QString column = model->record().fieldName(i);

            if (column == "Title"){
                book->setTitle(cell.toString());
            } else if (column == "Author"){
                book->setAuthor(cell.toString());
            } else if (column == "ISBN"){
                book->setIsbn(cell.toInt());

            }

        }
        books.append(*book);
    }
    Exporter exporter;
    exporter.exportToXml(books, dir.path());
}

void MainWindow::onPorgress(int value){
    progress->setValue(value);
}
