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
    void finished(QList<Book> books, QStringList errors);
    void onProgress(int value);

public:
    Parser(QStringList fileNames, QString path, QObject *parent=0);

    void parseFolder();

public slots:
    void run();
    void setRunning(bool running);

private:
    QList<Book>* books;
    QStringList fileNames;
    QStringList errors;
    QString path;
    bool isRunning;

    Book parseBook(QXmlStreamReader& xml);
};


#endif // PARSER_H
