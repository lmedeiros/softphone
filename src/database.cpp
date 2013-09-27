#include "headers/database.h"
#include "QSqlDatabase"
#include "QFileInfo"
#include <QtGui/QGuiApplication>

Database::Database(QObject *parent) :
    QObject(parent)
{
    setup();
}

bool Database::createFile()
{
    QFile file(*databasePath);
    file.open(QIODevice::ReadWrite);
    file.close();

    QFileInfo checkfile(*databasePath);
    if(!checkfile.isFile())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Database::setup()
{
    databasePath = new QString(QGuiApplication::applicationDirPath()+"/db.sqlite");

    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName(*databasePath);

    QFileInfo checkfile(*databasePath);

    if(!checkfile.isFile()) { createFile(); }

}
