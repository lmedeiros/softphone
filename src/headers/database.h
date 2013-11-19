#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = 0);
    QSqlDatabase mydb;
    QString *databasePath;
    void setup();
    bool createFile();
signals:
    
public slots:
    
};

#endif // DATABASE_H
