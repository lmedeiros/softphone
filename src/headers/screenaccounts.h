#ifndef SCREENACCOUNTS_H
#define SCREENACCOUNTS_H

#include <QObject>
#include "dbtablemobel.h"
class Controller;

class ScreenAccounts : public QObject
{
    Q_OBJECT
public:
    explicit ScreenAccounts(Controller *ctrl, QObject *parent = 0);
    Controller *controller;

    void setCodecs(const QString &first_codec, const QString &second_codec, const QString &third_codec);
    bool createTable();
    void createEmptyAccounts();
    Q_INVOKABLE bool deleteAccount(const QString &account_id);
    Q_INVOKABLE void loadAccount(const QString &account_id);
    Q_INVOKABLE void getAccountList();
    Q_INVOKABLE bool addAccount(const QString &description, const QString &username, const QString &secret, const QString &server, const QString &first_codec, const QString &second_codec, const QString &third_codec, const QString &srtp, const QString &tls, const QString &mailbox="121");
    Q_INVOKABLE bool editAccount(const QString &description, const QString &username, const QString &secret, const QString &server, const QString &account_id, const QString &first_codec, const QString &second_codec, const QString &third_codec, const QString &srtp, const QString &tls, const QString &mailbox="121");
signals:
    
public slots:
    
};

#endif // SCREENACCOUNTS_H
