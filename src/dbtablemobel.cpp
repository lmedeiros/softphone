#include "headers/dbtablemobel.h"
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>


DBTableMobel::DBTableMobel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void DBTableMobel::setQuery(const QString &query, const QSqlDatabase &db)
{
    QSqlQueryModel::setQuery(query,db);
    generateRoleNames();
}

void DBTableMobel::setQuery(const QSqlQuery & query)
{
    QSqlQueryModel::setQuery(query);
    generateRoleNames();
}

void DBTableMobel::generateRoleNames()
{
    //QHash<int, QByteArray> roles;
//    for( int i = 0; i < record().count(); i++) {
//        roles[Qt::UserRole + i + 1] = record().fieldName(i).toLatin1();
//        setHeaderData(i,Qt::Horizontal, record().fieldName(i).toLatin1(),i);
//    }

    int nbCols = this->columnCount();

    for (int i = 0; i < nbCols; i++)
    {
        roles[Qt::UserRole + i + 1] = QVariant(this->headerData(i, Qt::Horizontal).toString()).toByteArray();
    }

}

QVariant DBTableMobel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if(role < Qt::UserRole)
    {
        value = QSqlQueryModel::data(index, role);
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }
    return value;
}
