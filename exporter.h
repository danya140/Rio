#ifndef EXPORTER_H
#define EXPORTER_H

#include <QList>
#include <QXmlStreamWriter>
#include "book.h"

class Exporter
{
public:
    Exporter();
    void exportToXml(QList<Book> books, QString path);

private:

};

#endif // EXPORTER_H
