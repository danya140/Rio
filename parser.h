#ifndef PARSER_H
#define PARSER_H

#include <QList>
#include <QXmlStreamReader>
#include <QtSql>
#include "book.h"



class Parser
{
public:
    Parser();
    void parseFolder(QStringList& fileNames, QString& path);
    QList<Book> getBooksList();

private:
    QList<Book>* books;

    Book parseBook(QXmlStreamReader& xml);


};


#endif // PARSER_H
