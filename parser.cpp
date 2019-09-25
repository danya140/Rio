#include "parser.h"


Parser::Parser(QStringList fileNames, QString path, QObject *parent): QObject(parent)
{
    qRegisterMetaType<QList<Book>>("QList<Book>");
    this->fileNames = fileNames;
    this->path = path;
    books = new QList<Book>();
}

void Parser::run(){
    parseFolder();
    emit(finished(getBooksList()));
}

void Parser::parseFolder(){
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
        file->close();
    }

}

QList<Book> Parser::getBooksList(){
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

void Parser::setRunning(bool running){
    isRunning = running;
}

