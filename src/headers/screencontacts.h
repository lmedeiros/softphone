#ifndef SCREENCONTACTS_H
#define SCREENCONTACTS_H

#include <QObject>
#include "dbtablemobel.h"
#include <QFileSystemModel>

class Controller;

class ScreenContacts : public QObject
{
    Q_OBJECT
public:
    explicit ScreenContacts(Controller *db, QObject *parent = 0);
    Controller *controller;
    bool createTableNumbers();
    bool createTable();

    Q_INVOKABLE bool setFavourite(const int number_id, const QString &fav);
    Q_INVOKABLE void getFavourites();
    Q_INVOKABLE void getContact(const QString& number);
    Q_INVOKABLE void getNumbers(const QString& contact_id);
    Q_INVOKABLE void getNewNumbers(const QString& contact_id);
    Q_INVOKABLE qint64 generateContactId();
    Q_INVOKABLE bool addNumber(const QString& description, const QString& number, const QString& contact_id);
    Q_INVOKABLE bool delContact(const QString &contact_id);
    Q_INVOKABLE bool deleteNumber(const QString& number_id);
    Q_INVOKABLE void getContacts(const QString &order_by, bool order_asc, const QString &name);
    Q_INVOKABLE bool addContact(const QString &name, const QString &surname, const QString &pic_path, const QString &account_id);
    Q_INVOKABLE bool editContact(const QString &name, const QString &surname, const QString &pic_path, const QString &contact_id);


signals:

public slots:
    
};

#endif // SCREENCONTACTS_H
