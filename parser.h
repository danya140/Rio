#ifndef PARSER_H
#define PARSER_H

#include <QList>
#include <QXmlStreamReader>
#include <QtSql>
#include "book.h"



class Parser
{
public:
    Parser(QSqlQuery& query);
    void parseFolder(QStringList& fileNames, QString& path);
    QList<Book> getXmlConf();

private:
    QList<Book>* books;
    QSqlQuery query;

    Book parseBook(QXmlStreamReader& xml);


};


#endif // PARSER_H
