#include "book.h"

Book::Book(){

}

Book::Book(QString title, QString author, int isbn)
{
    this->title = title;
    this->author = author;
    this->isbn = isbn;
}


QString Book::getTitle(){
    return title;
}

QString Book::getAuthor(){
    return author;
}

int Book::getIsbn(){
    return isbn;
}


void Book::setTitle(QString title){
    this->title = title;
}

void Book::setAuthor(QString author){
    this->author = author;
}

void Book::setIsbn(int isbn){
    this->isbn = isbn;
}
