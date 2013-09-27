
#include "headers/screencontacts.h"
#include "headers/controller.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QObject>


ScreenContacts::ScreenContacts(Controller *ctrl, QObject *parent) :
    QObject(parent)
{
    controller = ctrl;
    createTable();
    createTableNumbers();
}

qint64 ScreenContacts::generateContactId()
{
    if (controller->db->mydb.open())
    {
        QString queryString("SELECT seq FROM SQLITE_SEQUENCE WHERE name='contacts'");

        QSqlQuery id = controller->db->mydb.exec(queryString);

        if(controller->db->mydb.lastError().number()>0)
        {
            qWarning() << "getLogs() error:" << controller->db->mydb.lastError();
            return 0;
        }
        else
        {
            while (id.next())
            {
                qint64 new_id = id.value(0).toLongLong() + 1;

                QString query = QString("DELETE FROM contact_number WHERE contact_id = '" + QString::number(new_id) + "'");
                controller->db->mydb.exec(query);

                qDebug() << "Contact ID: " << new_id;

                return new_id;
            }
        }
    }
    qDebug() << "First Contact ID: 1";
    return 1;
}

bool ScreenContacts::createTableNumbers()
{
    if (controller->db->mydb.open())
    {
        QString query("CREATE TABLE IF NOT EXISTS contact_number "
                 "(number_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "contact_id INTEGER, "
                 "number VARCHAR(80), "
                 "description VARCHAR(80),"
                 "favourite INT)");

        controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number()>0)
        {
            qDebug() << "createTableNumbers error: " << controller->db->mydb.lastError();
            return false;
        }
        return true;
    }
    return false;
}

bool ScreenContacts::createTable()
{
    if (controller->db->mydb.open())
    {
        QString query("CREATE TABLE IF NOT EXISTS contacts "
                 "(contact_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "name VARCHAR(80), "
                 "surname VARCHAR(80), "
                 "pic_path VARCHAR(255),"
                 "account_id INT)");

        controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number()>0)
        {
            qDebug() << "createTable error: " << controller->db->mydb.lastError();
            return false;
        }
        return true;
    }
    return false;
}

bool ScreenContacts::addNumber(const QString& description, const QString& number, const QString& contact_id)
{
    if (controller->db->mydb.open())
    {
        QString query("INSERT INTO contact_number(number, description, contact_id) VALUES('" + number + "', '" + description + "', '" + contact_id + "')");

        controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number()>0)
        {
            qDebug() << "addNumber error: " << controller->db->mydb.lastError();
            return false;
        }
        return true;
    }
    return false;

}

void ScreenContacts::getContact(const QString& number)
{
    if(controller->db->mydb.open())
    {
        QString queryString("SELECT name, surname, pic_path, number, description FROM contact_number NATURAL JOIN contacts WHERE number LIKE '" + number + "' LIMIT 1");

        QStringList activeCallContact;

        QSqlQuery query = controller->db->mydb.exec(queryString);

        if(controller->db->mydb.lastError().number()>0)
        {
            qDebug() << "getContact error: " << controller->db->mydb.lastError();
            return;
        }
        else
        {
            while(query.next())
            {
                activeCallContact.append(query.value(0).toString());
                activeCallContact.append(query.value(1).toString());
                activeCallContact.append(query.value(2).toString());
                activeCallContact.append(query.value(3).toString());
                activeCallContact.append(query.value(4).toString());

                if(controller->screenPhone->active_channel()==1)
                {
                    if(controller->screenPhone->c1_call_id() == -1)
                    {
                        controller->qmlviewer->rootContext()->setContextProperty("activeCall1Contact",  activeCallContact);
                    }
                    else
                    {
                        controller->qmlviewer->rootContext()->setContextProperty("activeCall2Contact",  activeCallContact);
                    }
                }
                else
                {
                    if(controller->screenPhone->c2_call_id() == -1)
                    {
                        controller->qmlviewer->rootContext()->setContextProperty("activeCall2Contact",  activeCallContact);
                    }
                    else
                    {
                        controller->qmlviewer->rootContext()->setContextProperty("activeCall1Contact",  activeCallContact);
                    }
                }

                return;
            }

            activeCallContact.append(number);
            activeCallContact.append("");
            activeCallContact.append("");
            activeCallContact.append(number);
            activeCallContact.append("Unknown");

            if(controller->screenPhone->active_channel()==1)
            {
                if(controller->screenPhone->c1_call_id() == -1)
                {
                    controller->qmlviewer->rootContext()->setContextProperty("activeCall1Contact",  activeCallContact);
                }
                else
                {
                    controller->qmlviewer->rootContext()->setContextProperty("activeCall2Contact",  activeCallContact);
                }
            }
            else
            {
                if(controller->screenPhone->c2_call_id() == -1)
                {
                    controller->qmlviewer->rootContext()->setContextProperty("activeCall2Contact",  activeCallContact);
                }
                else
                {
                    controller->qmlviewer->rootContext()->setContextProperty("activeCall1Contact",  activeCallContact);
                }
            }
        }
    }
}

void ScreenContacts::getContacts(const QString &order_by, bool order_asc, const QString &name)
{
    if (controller->db->mydb.open())
    {
        DBTableMobel *model = new DBTableMobel();

        QString queryString("SELECT * FROM contacts WHERE name LIKE '%" + name + "%' OR surname  LIKE '%" + name + "%'");

        if(!order_by.isNull())
        {
            queryString = queryString + " ORDER BY " + order_by + ((order_asc) ? " ASC " : " DESC ");
        }
        else
        {
            queryString = queryString + " ORDER BY name DESC ";
        }

        queryString = queryString + "LIMIT 500";

        model->setQuery(queryString, controller->db->mydb);

        if(controller->db->mydb.lastError().number()>0)
        {
            qWarning() << "getLogs() error:" << controller->db->mydb.lastError();
        }
        else
        {
            controller->qmlviewer->rootContext()->setContextProperty("contactModel",  model);
        }
    }
}

void ScreenContacts::getNumbers(const QString& contact_id)
{
    if (controller->db->mydb.open())
    {
        DBTableMobel *model = new DBTableMobel();

        QString queryString("SELECT * FROM contact_number WHERE contact_id = '" + contact_id + "' ORDER BY description");

        qDebug() << "query contact_number: " << queryString;

        model->setQuery(queryString, controller->db->mydb);

        if(controller->db->mydb.lastError().number()>0)
        {
            qWarning() << "getNumbers() error:" << controller->db->mydb.lastError();
        }
        else
        {
            controller->qmlviewer->rootContext()->setContextProperty("numbersModel",  model);
        }
    }
}

void ScreenContacts::getFavourites()
{
    if (controller->db->mydb.open())
    {
        DBTableMobel *model = new DBTableMobel();

        QString queryString("SELECT * FROM contact_number NATURAL JOIN contacts WHERE");

        queryString.append(" favourite=1 ORDER BY name, description");

        model->setQuery(queryString, controller->db->mydb);

        if(controller->db->mydb.lastError().number()>0)
        {
            qWarning() << "getFavourites() error:" << controller->db->mydb.lastError();
        }
        else
        {
            controller->qmlviewer->rootContext()->setContextProperty("favouritesModel",  model);
        }
    }
}

bool ScreenContacts::setFavourite(const int number_id, const QString& fav)
{
    if (controller->db->mydb.open())
    {
        QString query = QString("UPDATE contact_number SET favourite='"+fav+"' WHERE number_id='"+QString::number(number_id)+"'");

       controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number() < 0)
        {
            return true;
        }
        else
        {
            qDebug() << "setFavourite() error: " << controller->db->mydb.lastError();
            return false;
        }
    }

    return false;
}

void ScreenContacts::getNewNumbers(const QString& contact_id)
{
    if (controller->db->mydb.open())
    {
        DBTableMobel *model = new DBTableMobel();

        QString queryString("SELECT * FROM contact_number WHERE contact_id = '" + contact_id + "' ORDER BY description");

        model->setQuery(queryString, controller->db->mydb);

        if(controller->db->mydb.lastError().number()>0)
        {
            qWarning() << "getNewNumbers() error:" << controller->db->mydb.lastError();
        }
        else
        {
            controller->qmlviewer->rootContext()->setContextProperty("numbersNewModel",  model);
        }
    }
}

bool ScreenContacts::addContact(const QString &name, const QString &surname, const QString &pic_path, const QString &account_id)
{
    if (controller->db->mydb.open())
    {
        QString query = QString("INSERT INTO contacts(name, surname, pic_path, account_id) VALUES('" + name + "', '" + surname + "', '" + pic_path + "', '" + account_id + "')");
        controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number() < 0)
        {
//            QString query_update_contact = "UPDATE log_call SET name = '" + name + "', pic_path = '" + pic_path + "' WHERE to_num IN (SELECT number FROM contact_number WHERE contact_id=last_insert_rowid())";
//            controller->db->mydb.exec(query_update_contact);

//            if(controller->db->mydb.lastError().number() >= 0)
//            {
//                qDebug() << "ScreenContacts::addContact() --> No previous call_log from this contact found...";
//            }

            return true;
        }
        else
        {
            qDebug() << "addContact() error: " << controller->db->mydb.lastError().number();
            return false;
        }
    }

    return false;
}

bool ScreenContacts::editContact(const QString &name, const QString &surname, const QString &pic_path, const QString &contact_id)
{
    if (controller->db->mydb.open())
    {
        QString query = QString("UPDATE contacts SET name='"+name+"', surname='"+surname+"' , pic_path='"+pic_path+"' WHERE contact_id='"+contact_id+"'");
        controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number() < 0)
        {
            return true;
        }
        else
        {
            qDebug() << "editContact() error: " << controller->db->mydb.lastError();
            return false;
        }
    }

    return false;
}

bool ScreenContacts::delContact(const QString &contact_id)
{
    if (controller->db->mydb.open())
    {
        QString query("DELETE FROM contacts WHERE contact_id ='" + contact_id + "'");

        controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number() >= 0)
        {
            qDebug() << "delete contact error: " << controller->db->mydb.lastError();
            return false;
        }
        else
        {
            QString query_number("DELETE FROM contact_number WHERE contact_id =" + contact_id);

            controller->db->mydb.exec(query_number);
            return true;
        }
    }
    return false;
}

bool ScreenContacts::deleteNumber(const QString &number_id)
{
    if (controller->db->mydb.open())
    {
        QString query("DELETE FROM contact_number WHERE number_id =" + number_id);

        controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number() >= 0)
        {
            qDebug() << "deleteNumber error: " << controller->db->mydb.lastError();
            return false;
        }
        else
        {
            return true;
        }
    }
    return false;
}
