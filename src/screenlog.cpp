#include "headers/screenlog.h"
#include "headers/controller.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QObject>

ScreenLog::ScreenLog(Controller *ctrl, QObject *parent) :
    QObject(parent)
{
    controller = ctrl;
    createTable();
}

bool ScreenLog::createTable()
{
    if (controller->db->mydb.open())
    {
        QString query = "CREATE TABLE IF NOT EXISTS log_call "
                 "(log_call_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "from_num VARCHAR(80), "
                 "to_num VARCHAR(80), "
                 "name VARCHAR(190), "
                 "description VARCHAR(80), "
                 "pic_path VARCHAR(255), "
                 "log_type VARCHAR(3), "
                 "moment DATETIME, "
                 "duration INT, "
                 "rx VARCHAR(40), "
                 "tx VARCHAR(40), "
                 "cost VARCHAR(40), "
                 "local_loss VARCHAR(40), "
                 "remote_loss VARCHAR(40), "
                 "codec VARCHAR(40), "
                 "contact_id VARCHAR(40), "
                 "number_id VARCHAR(40), "
                 "account_id INT)";

        controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number() > 0)
        {
            qDebug() << "createTable() error: " << controller->db->mydb.lastError();
            return false;
        }
        return true;
    }
    return false;
}

bool ScreenLog::insertLog(const QString &from_num, const QString &to_num, const QString &log_type, const QString &datetime, const QString &duration,
                          const QString &account_id, const QString &name, const QString &description, const QString &pic_path, const QString &rx,
                          const QString &tx, const QString &cost, const QString &local_loss, const QString &remote_loss, const QString &codec,
                          const QString &contact_id, const QString &number_id)
{
    if (controller->db->mydb.open())
    {
        QString to_n = to_num;
        QString query = "INSERT INTO log_call(from_num, to_num, log_type, moment, duration, account_id, name, description, pic_path, rx, tx, cost, local_loss, remote_loss, codec, contact_id, number_id)"
                " VALUES('"+from_num+"','"+ to_n.replace("\"","").replace("sip:","") +"','"+log_type+"','"+datetime+"',"+duration+",'"+account_id+"','"+name+"','"+description+"','"+pic_path+"','"+rx+"','"+tx+"','"+cost+"','"+local_loss+"','"+remote_loss+"','"+codec+"','"+contact_id+"','"+number_id+"')";

        controller->db->mydb.exec(query);

        //qDebug() << "Registering call data: " << query << "!!!!!!!!!!";

        if(controller->db->mydb.lastError().number() > 0)
        {
            qDebug() << "insertLog() error: " << controller->db->mydb.lastError();
            return false;
        }
        return true;
    }
    return false;
}

bool ScreenLog::clearLogs(const QString &account_id)
{
    if (controller->db->mydb.open())
    {
         QString query("DELETE FROM log_call WHERE account_id =" + account_id);

        controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number() > 0)
        {
            qDebug() << "clear Log error: " << controller->db->mydb.lastError();
            return false;
        }
        return true;
    }
    return false;
}

void ScreenLog::getLogs(const QString &account_id)
{
    if (controller->db->mydb.open())
    {
        DBTableMobel *model = new DBTableMobel();

        QString query("SELECT *, time(duration, 'unixepoch') as duration FROM log_call WHERE account_id = '" + account_id + "' ");

        query += " ORDER BY moment DESC LIMIT 500";

        model->setQuery(query, controller->db->mydb);

        if(controller->db->mydb.lastError().number() > 0)
        {
            qDebug() << "getLogs() error:" << controller->db->mydb.lastError();
        }
        else
        {
            controller->qmlviewer->rootContext()->setContextProperty("logModel",  model);
        }
    }
}

bool ScreenLog::delLog(const QString &log_id)
{
    if (controller->db->mydb.open())
    {
        QString query("DELETE FROM log_call WHERE log_call_id =" + log_id);

        controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number() > 0)
        {
            qDebug() << "delete log error: " << controller->db->mydb.lastError();
            return false;
        }
        return true;

    }
    return false;
}

void ScreenLog::contactInfo(const QString &number)
{
    DBTableMobel *model = new DBTableMobel();

    QString queryString("SELECT * FROM contact_number NATURAL JOIN contacts WHERE number LIKE '" + number + "%' ");

    queryString = queryString + "LIMIT 30";

    //qDebug() << "query suggestions: " << queryString;

    model->setQuery(queryString, controller->db->mydb);

    if(controller->db->mydb.lastError().number()>0)
    {
        qWarning() << "contactInfo() error:" << controller->db->mydb.lastError();
    }
    else
    {
        controller->qmlviewer->rootContext()->setContextProperty("suggestionModel",  model);
    }
}
