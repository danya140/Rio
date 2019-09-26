#include "parser.h"

/**
 * @brief Parser::Parser
 * @param fileNames список файлов для парсинга
 * @param path путь до папки с файлами
 * @param parent родитель
 */
Parser::Parser(QStringList fileNames, QString path, QObject *parent): QObject(parent)
{
    qRegisterMetaType<QList<Book>>("QList<Book>");
    this->fileNames = fileNames;
    this->path = path;
    books = new QList<Book>();
}

void Parser::run(){
    parseFolder();
    emit(finished(*books, errors));
}

/**
 * @brief Parser::parseFolder
 *
 * Основа парсера.
 * Производит парсинг по корню
 */
void Parser::parseFolder(){
    for(QString filePath : fileNames){
        QFile* file = new QFile(path+"/"+filePath);

        if(!file->open(QIODevice::ReadOnly)){
            errors.append("Не получилось открыть файл:" + path + "/" + filePath);
        }

        QXmlStreamReader xml(file);
        while (!xml.atEnd() && !xml.hasError())
            {
                QXmlStreamReader::TokenType token = xml.readNext();
                if (token == QXmlStreamReader::StartDocument)
                    continue;
                if (token == QXmlStreamReader::StartElement && xml.name() == "Book")
                {
                    try {
                    books->append(parseBook(xml));
                    } catch (QException) {
                    errors.append("Ошибка чтения файла: " + path + "/" + filePath
                                  +" строка: "+QString::number(xml.lineNumber()-1));
                    }
                }
            }
        onProgress(fileNames.indexOf(filePath));
        file->close();
    }

}

/**
 * @brief Parser::parseBook
 * @param xml xml ридер
 * @return распарсеная книга при удачном
 *
 * Парсинг книги
 */
Book Parser::parseBook(QXmlStreamReader& xml){
    Book* book = new Book();
    xml.readNext();

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Book"))
    {
        if(xml.hasError()){
            throw QException();
        }
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

