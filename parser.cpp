#include "parser.h"


Parser::Parser(QSqlQuery& query)
{
    this->query = query;

    books = new QList<Book>();
}

void Parser::parseFolder(QStringList& fileNames, QString& path){
    for(QString filePath : fileNames){
        QFile* file = new QFile(path+"/"+filePath);

        if(!file->open(QIODevice::ReadOnly)){
            qDebug("Can't open file");
        }

        QXmlStreamReader xml(file);

        while (!xml.atEnd() && !xml.hasError())
            {
                QXmlStreamReader::TokenType token = xml.readNext();
                if (token == QXmlStreamReader::StartDocument)
                    continue;
                if (token == QXmlStreamReader::StartElement)
                {
                    if (xml.name() == "Catalog")
                        continue;
                    if (xml.name() == "Book")
                        books->append(parseBook(xml));
                }
            }
    }

};

QList<Book> Parser::getXmlConf(){
    return *books;
}

Book Parser::parseBook(QXmlStreamReader& xml){
    Book* book = new Book();
    xml.readNext();

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Book"))
    {

        if (xml.tokenType() == QXmlStreamReader::StartElement)
            {
                if (xml.name() == "Title"){
                    xml.readNext();
                    book->setTitle(xml.text().toString());
                } else if (xml.name() == "Author"){
                    xml.readNext();
                    book->setAuthor(xml.text().toString());
                } else if (xml.name() == "ISBN"){
                    xml.readNext();
                    book->setIsbn(xml.text().toString().toInt());
                }
            }

            xml.readNext();
    }
        return  *book;
};

