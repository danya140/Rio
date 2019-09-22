#ifndef BOOK_H
#define BOOK_H

#include <QString>

class Book
{
public:
    Book();
    Book(QString title, QString author, int isbn);

    int getIsbn();
    QString getAuthor();
    QString getTitle();

    void setIsbn(int isbn);
    void setAuthor(QString author);
    void setTitle(QString title);

private:

    QString title;
    QString author;
    int isbn;
};

#endif // BOOK_H
