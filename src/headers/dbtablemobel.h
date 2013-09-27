#ifndef DBTABLEMOBEL_H
#define DBTABLEMOBEL_H

#include <QSqlQueryModel>

class DBTableMobel : public QSqlQueryModel
{
    Q_OBJECT
    void generateRoleNames();

public:
    explicit DBTableMobel(QObject *parent = 0);

    void setQuery(const QString &query, const QSqlDatabase &db = QSqlDatabase());
    void setQuery(const QSqlQuery &query);
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const{return roles;}

private:
    QHash<int, QByteArray> roles;

signals:

public slots:

};

#endif // DBTABLEMOBEL_H
