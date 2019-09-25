#ifndef PARSER_H
#define PARSER_H

#include <QList>
#include <QXmlStreamReader>
#include <QtSql>
#include "book.h"



class Parser : public QObject
{
    Q_OBJECT


signals:
    void finished(QList<Book> books);

public:
    Parser(QStringList fileNames, QString path, QObject *parent=0);
    void parseFolder();
    QList<Book> getBooksList();
    bool running();

public slots:
    void run();
    void setRunning(bool running);


private:
    QList<Book>* books;
    bool isRunning;
    QStringList fileNames;
    QString path;

    Book parseBook(QXmlStreamReader& xml);


};


#endif // PARSER_H
