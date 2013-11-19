#include "headers/screenaccounts.h"
#include "headers/controller.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QObject>
#include <QVector>

#include <pjsua-lib/pjsua.h>

ScreenAccounts::ScreenAccounts(Controller *ctrl, QObject *parent) :
    QObject(parent)
{
    controller = ctrl;
    createTable();
    createEmptyAccounts();
}

void ScreenAccounts::createEmptyAccounts()
{
    if (controller->db->mydb.open())
    {
       QSqlQuery query = controller->db->mydb.exec("SELECT COUNT(*) AS accs FROM sip_account");

        if(controller->db->mydb.lastError().number() > 0)
        {
            qDebug() << "createTable() error: " << controller->db->mydb.lastError();
            return;
        }
        else
        {
            query.next();
            int cnt = query.value(0).toInt();

            if(cnt != 3)
            {
                controller->db->mydb.exec("DELETE FROM sip_account");
                controller->db->mydb.exec("DELETE FROM SQLITE_SEQUENCE WHERE name='sip_account'");
                addAccount(QString("Othos VoIP Account"),"","","","","","","NO","UDP");
                addAccount(QString("Othos PBX Account"),"","","","","","","NO","UDP");
                addAccount(QString("Custom SIP Account"),"","","","","","","NO","UDP");
            }
        }
    }
}

bool ScreenAccounts::createTable()
{
    if (controller->db->mydb.open())
    {
       controller->db->mydb.exec("create table if not exists sip_account "
                 "(sip_account_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "description varchar(90), "
                 "user varchar(50), "
                 "secret varchar(50),"
                 "server_address varchar(40), "
                 "first_codec varchar(40),"
                 "second_codec varchar(40),"
                 "third_codec varchar(40), "
                 "srtp varchar(40), "
                 "transport varchar(40), "
                 "mailbox varchar(40), "
                 "is_enabled int)");

        if(controller->db->mydb.lastError().number() > 0)
        {
            qDebug() << "createTable() error: " << controller->db->mydb.lastError();
            return false;
        }
        return true;
    }
    return false;
}

void ScreenAccounts::loadAccount(const QString &account_id)
{
    if(account_id.toInt()<=0)
    {
        return;
    }

    qDebug() << "Loading account " << account_id;

    if (controller->db->mydb.open())
    {
        QString queryString("SELECT * FROM sip_account WHERE sip_account_id='"+account_id+"' LIMIT 1");

        int totalFields = 10;

        QSqlQuery query(queryString);

        if(query.lastError().number()>0)
        {
            qDebug() << "loadAccount() error" << controller->db->mydb.lastError();
            return;
        }

        QStringList lst;


        if(query.next())
        {
            for(int i=0 ; totalFields >= i ; i++)
            {
                lst.append(query.value(i).toString());
            }
            switch(account_id.toInt())
            {
                case 1:
                    controller->qmlviewer->rootContext()->setContextProperty("accountEditModel1", lst);
                    break;
                case 2:
                    controller->qmlviewer->rootContext()->setContextProperty("accountEditModel2", lst);
                    break;
                case 3:
                    controller->qmlviewer->rootContext()->setContextProperty("accountEditModel3", lst);
                    break;
            }
        }
    }
}

bool ScreenAccounts::addAccount(const QString &description, const QString &username, const QString &secret, const QString &server, const QString &first_codec, const QString &second_codec, const QString &third_codec,  const QString &srtp, const QString &tls, const QString &mailbox)
{
    if (controller->db->mydb.open())
    {
        QString query = QString("INSERT INTO sip_account(description, user, secret, server_address, first_codec, second_codec, third_codec, srtp, transport, mailbox, is_enabled) VALUES('" + description + "', '" + username + "', '" + secret + "', '" + server + "', '" + first_codec + "', '" + second_codec + "', '" + third_codec + "', '" + srtp + "', '" + tls + "','" + mailbox + "', 1)");
        controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number() > 0)
        {
            qDebug() << "addAccount() error: " << controller->db->mydb.lastError().number();
            return false;
        }

        if(first_codec=="" || second_codec=="" || third_codec=="") return true;
        setCodecs(first_codec, second_codec, third_codec);
        return true;
    }

    return false;
}

void ScreenAccounts::setCodecs(const QString &first_codec, const QString &second_codec, const QString &third_codec)
{
    pj_status_t status;
    pjsua_codec_info codecs[128];
    pj_str_t codec_id;
    unsigned count;
    count = 128;

    pjsua_enum_codecs(codecs, &count);

    for(unsigned i=0 ; count > i ; i++)
    {
        codec_id = pj_str(codecs[i].codec_id.ptr);
        status = pjsua_codec_set_priority(&codec_id, PJMEDIA_CODEC_PRIO_DISABLED);
    }

    if(first_codec == second_codec)
    {
        codec_id = pj_str((char*)first_codec.toStdString().c_str());
        status = pjsua_codec_set_priority(&codec_id, PJMEDIA_CODEC_PRIO_NORMAL+12);

        codec_id = pj_str((char*)third_codec.toStdString().c_str());
        status = pjsua_codec_set_priority(&codec_id,  PJMEDIA_CODEC_PRIO_NORMAL+11);

        return;
    }

    if(first_codec == third_codec || second_codec == third_codec)
    {
        codec_id = pj_str((char*)first_codec.toStdString().c_str());
        status = pjsua_codec_set_priority(&codec_id, PJMEDIA_CODEC_PRIO_NORMAL+12);

        codec_id = pj_str((char*)second_codec.toStdString().c_str());
        status = pjsua_codec_set_priority(&codec_id,  PJMEDIA_CODEC_PRIO_NORMAL+11);

        return;
    }

    if(second_codec == third_codec && first_codec == third_codec)
    {
        codec_id = pj_str((char*)first_codec.toStdString().c_str());
        status = pjsua_codec_set_priority(&codec_id, PJMEDIA_CODEC_PRIO_NORMAL+12);

        return;
    }

    codec_id = pj_str((char*)first_codec.toStdString().c_str());
    status = pjsua_codec_set_priority(&codec_id, PJMEDIA_CODEC_PRIO_NORMAL+12);

    codec_id = pj_str((char*)second_codec.toStdString().c_str());
    status = pjsua_codec_set_priority(&codec_id,  PJMEDIA_CODEC_PRIO_NORMAL+11);

    codec_id = pj_str((char*)third_codec.toStdString().c_str());
    status = pjsua_codec_set_priority(&codec_id,  PJMEDIA_CODEC_PRIO_NORMAL+10);
}

bool ScreenAccounts::editAccount(const QString &description, const QString &username, const QString &secret, const QString &server, const QString &account_id, const QString &first_codec, const QString &second_codec, const QString &third_codec, const QString &srtp, const QString &tls, const QString &mailbox)
{
    if (controller->db->mydb.open())
    {
        QString query = QString("UPDATE sip_account SET description='" + description + "', user='" + username + "', secret='" + secret + "', server_address='" + server + "', first_codec='" + first_codec + "', second_codec='" + second_codec + "', third_codec='" + third_codec + "' , srtp='" + srtp + "', transport='" + tls + "', mailbox='" + mailbox + "' WHERE sip_account_id ="+account_id);
        controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number() > 0)
        {
            qDebug() << "editAccount() error: " << controller->db->mydb.lastError() << " qry " << query;
            return false;
        }

        setCodecs(first_codec, second_codec, third_codec);
        return true;
    }
    return false;
}

bool ScreenAccounts::deleteAccount(const QString &account_id)
{
    if (controller->db->mydb.open())
    {
        QString query("DELETE FROM sip_account WHERE sip_account_id =" + account_id);
        controller->db->mydb.exec(query);

        if(controller->db->mydb.lastError().number() > 0)
        {
            qDebug() << "deleteAccount() error: " << controller->db->mydb.lastError() << query;
            return false;
        }

        controller->screenLog->clearLogs(account_id);
        return true;
    }
    return false;
}

void ScreenAccounts::getAccountList()
{
    if (controller->db->mydb.open())
    {
        DBTableMobel *model = new DBTableMobel();
        QString query("SELECT sip_account_id as id, description as data FROM sip_account");
        model->setQuery(query, controller->db->mydb);

        if(controller->db->mydb.lastError().number() > 0)
        {
            qDebug() << "res: " << model << "getAccounts() error" << controller->db->mydb.lastError();
            return;
        }

        controller->qmlviewer->rootContext()->setContextProperty("accountListModel", model);
    }
}
