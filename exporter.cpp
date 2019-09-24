#include "exporter.h"

Exporter::Exporter()
{

}

void Exporter::exportToXml(QList<Book> books, QString path){
    QFile file(path);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);

    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Catalog");

    for(Book book : books){
        xmlWriter.writeStartElement("Book");

        xmlWriter.writeStartElement("Title");
        xmlWriter.writeCharacters(book.getTitle());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("Author");
        xmlWriter.writeCharacters(book.getAuthor());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("ISBN");
        xmlWriter.writeCharacters(QString::number(book.getIsbn()));
        xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    file.close();
}
