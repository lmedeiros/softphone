#include "headers/telephony.h"
#include "headers/controller.h"
#include <pjsua-lib/pjsua.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QDir>
#define THIS_FILE "APP"
#define APP_VERSION "3.0.0"


Telephony *Telephony::telephony;

Telephony::Telephony(Controller *ctrl, QObject *parent) : QObject(parent)
{
    controller = ctrl;
    telephony = this;

}

void Telephony::on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(call_id, &ci);

    telephony->controller->screenPhone->incomingCall(call_id, ci, telephony->activeAccountID);
}

int Telephony::recordCall(int call_id)
{
    pjsua_call_info ci;
    pjsua_call_get_info((pjsua_call_id)call_id, &ci);

    QString recFolder = QGuiApplication::applicationDirPath().append("/recs/");

    QString cinfo = ci.remote_info.ptr;
    cinfo = cinfo.mid(0, cinfo.indexOf("@"));
    cinfo = cinfo.mid(cinfo.indexOf(":")+1);

    if(!QDir(recFolder).exists()) QDir().mkdir(recFolder);

    QString date = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
    QString filename = recFolder.append("Recording_").append(cinfo).append("-").append(date).append(".wav").replace("*","").replace("#","");

    const char* file = filename.toUtf8().constData();

    pjsua_recorder_id rec_id;

    pj_str_t rec_file = pj_str((char*)file);
    pj_status_t status = PJ_ENOTFOUND;

    status = pjsua_recorder_create(&rec_file, 0, NULL, 0, 0, &rec_id);

    if (status != PJ_SUCCESS) {
    printf("GASP!!  record error!!\n");
    }

    pjsua_conf_port_id rec_port = pjsua_recorder_get_conf_port(rec_id);
    pjsua_conf_connect(ci.conf_slot, rec_port);

    return 1;

}

int Telephony::transferCall(int callId, const QString& url)
{
    pj_hostent he;
    pj_status_t status;
    pj_status_t has_internet;
    pj_str_t host = pj_str((char*)activeServer.toStdString().c_str());

    has_internet = pj_gethostbyname( &host, &he);
    if (has_internet != PJ_SUCCESS) {
        qDebug() << "No internet ";
        telephony->controller->screenPhone->setIs_registered(0);
        unregisterAccount();
    }

    if(telephony->controller->screenPhone->is_registered()==0)
    {
        qDebug() << "Error when calling, Not registered";
        return -1;
    }

    QString dialUri;
    dialUri = QString("sip:"+url+"@"+activeServer).trimmed();

    if (dialUri.size() > 149)
    {
        qDebug() << "Error when calling, number too long";
        return -1;
    }

    char ch_url[150];

    strcpy(ch_url, dialUri.toLocal8Bit().data());
    ch_url[dialUri.size()] = 0;

    pj_str_t uri = pj_str(ch_url);
    pjsua_call_id call_id = (pjsua_call_id)callId;

    qDebug() << "Transfering " << uri.ptr;
    status = pjsua_call_xfer(call_id, &uri, NULL);

    if (status != PJ_SUCCESS)
    {
        qDebug() << "Error when transfering " << status;
        return -1;
    }

    qDebug() << "OK on transfer " << status;

    hangUp(call_id);



    return (int)call_id;
}

void Telephony::stopRecordCall(int call_id)
{
    pjsua_call_info ci;
    pjsua_call_get_info((pjsua_call_id)call_id, &ci);
    pjsua_conf_disconnect(ci.conf_slot,0);
}


void Telephony::on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
    PJ_UNUSED_ARG(e);

    pjsua_call_info ci;
    pjsua_call_get_info(call_id, &ci);

    qDebug() << "on_call_state Rx_msg " << QString(ci.last_status_text.ptr);

    if(ci.state == PJSIP_INV_STATE_NULL) /**< Before INVITE is sent or received  */
    {
        qDebug() << "call state PJSIP_INV_STATE_NULL" << ci.state;
        return;
    }
    if(ci.state == PJSIP_INV_STATE_CALLING)	    /**< After INVITE is sent		    */
    {
        qDebug() << "call state PJSIP_INV_STATE_CALLING" << ci.state;
        return;
    }
    if(ci.state == PJSIP_INV_STATE_INCOMING)	    /**< After INVITE is received.	    */
    {
        qDebug() << "call state PJSIP_INV_STATE_INCOMING" << ci.state;
        return;
    }
    if(ci.state == PJSIP_INV_STATE_EARLY)	    /**< After response with To tag.	    */
    {
        qDebug() << "call state PJSIP_INV_STATE_EARLY" << ci.state;
        return;
    }
    if(ci.state == PJSIP_INV_STATE_CONNECTING)	    /**< After 2xx is sent/received.	    */
    {
        qDebug() << "call state PJSIP_INV_STATE_CONNECTING" << ci.state;
        return;
    }
    if(ci.state == PJSIP_INV_STATE_CONFIRMED)	    /**< After ACK is sent/received.	    */
    {
        qDebug() << "call state PJSIP_INV_STATE_CONFIRMED" << ci.state << " sip: " << ci.media_status;
        qDebug() << "Call connected";
        return;
    }
    if(ci.state == PJSIP_INV_STATE_DISCONNECTED)   /**< Session is terminated.		    */
    {
        telephony->controller->screenPhone->hangUp(call_id, telephony->activeAccountID);
        qDebug() << "call state PJSIP_INV_STATE_DISCONNECTED" << ci.state;
        return;
    }

    qDebug() << "call state PJSIP_OTHER____state" << ci.state;
}

void Telephony::on_reg_state2(pjsua_acc_id acc,pjsua_reg_info* info)
{
    PJ_UNUSED_ARG(acc);
    pjsua_acc_info acc_info;
    pjsua_acc_get_info(acc, &acc_info);

    qDebug() << "Register INFO: " << QString(info->cbparam->reason.ptr);

    if (acc_info.status < 300)
    {
        telephony->controller->screenPhone->setIs_registered(1);
        telephony->controller->screenPhone->setRegister_reason(QString::number(acc_info.status).append(" ").append(QString(acc_info.status_text.ptr)));
        qDebug() << "Registered";
    }
    else
    {
        telephony->controller->screenPhone->setIs_registered(0);
        telephony->controller->screenPhone->setRegister_reason(QString::number(acc_info.status).append(" ").append(QString(acc_info.status_text.ptr)));
        qDebug() << "Not registered";
    }
}

void Telephony::on_call_media_state(pjsua_call_id call_id)
{
    pjsua_call_info ci;
    pjsua_call_get_info(call_id, &ci);

    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE)
    {
        telephony->controller->screenPhone->connectedCall(call_id, ci);
        pjsua_conf_connect(ci.conf_slot, 0);
        pjsua_conf_connect(0, ci.conf_slot);
    }

    if(ci.media_status == PJSUA_CALL_MEDIA_ERROR)
    {
        telephony->hangUp(call_id);
    }
}

void Telephony::on_mwi_info(pjsua_acc_id acc_id, pjsua_mwi_info* mwi_info)
{
    qDebug() << "MWI INFO" << mwi_info->rdata->msg_info.msg->body->print_body;
    telephony->controller->screenPhone->setMwi_info(telephony->controller->screenPhone->mwi_info()+1);
}

void Telephony::error_exit(const char *title, pj_status_t status)
{
    pjsua_perror(THIS_FILE, title, status);
    pjsua_destroy();
}

void Telephony::hold(const int &call_id)
{
    pjsua_call_id cid = call_id;

    pjsua_call_set_hold(cid, 0);
    pjsua_conf_disconnect(0,  pjsua_call_get_conf_port(cid));
    pjsua_conf_adjust_rx_level(pjsua_call_get_conf_port(cid), 0.0f);

    telephony->controller->screenPhone->hold(cid);
}

void Telephony::unhold(const int &call_id)
{
    pjsua_call_id cid = call_id;

    pjsua_conf_connect(0, pjsua_call_get_conf_port(cid));
    pjsua_conf_adjust_rx_level(pjsua_call_get_conf_port(cid), 1.0f);
    pjsua_call_reinvite((pjsua_call_id)call_id,1,0);

    telephony->controller->screenPhone->unhold(cid);
}

void Telephony::answerCall(const int &call_id)
{
    qDebug()<< "Answer call: " << call_id;

    if(call_id<0) return;

    pjsua_call_info call_info;
    pjsua_call_get_info(call_id, &call_info);

    if (call_info.state == PJSIP_INV_STATE_INCOMING)
    {
        pjsua_call_answer((pjsua_call_id)call_id, 200, NULL, NULL);
    }
}

QVector<QString> Telephony::loadAccount(const QString &account_id)
{
    qDebug() <<  "Loading account...";
    QVector<QString> account;

    if(account_id.toInt()<=0)
    {
        return account;
    }

    if (controller->db->mydb.open())
    {
        QString queryString("SELECT * FROM sip_account WHERE sip_account_id='"+account_id+"' LIMIT 1");
        QSqlQuery query;
        query.exec(queryString);

        if(controller->db->mydb.lastError().number()<0)
        {
            if(query.first())
            {
                account.append(query.value(0).toString()); // Account ID
                account.append(query.value(1).toString()); // Description
                account.append(query.value(2).toString()); // Username
                account.append(query.value(3).toString()); // Secret
                account.append(query.value(4).toString()); // Server Address
                account.append(query.value(5).toString()); // First Codec
                account.append(query.value(6).toString()); // Second Codec
                account.append(query.value(7).toString()); // Third Codec
                account.append(query.value(8).toString()); // SRTP
                account.append(query.value(9).toString()); // Transport TLS-UDP-TCP
                account.append(query.value(10).toString()); // Mailbox

                controller->qmlviewer->rootContext()->setContextProperty("activeAccountMailbox", query.value(10).toString());
                controller->qmlviewer->rootContext()->setContextProperty("activeAccountDescription", query.value(1).toString());
                controller->qmlviewer->rootContext()->setContextProperty("activeAccountUser", query.value(2).toString());
                controller->qmlviewer->rootContext()->setContextProperty("activeAccount", query.value(0).toString());

            }
            else
            {
                qDebug() << "loadAccount() error: " << controller->db->mydb.lastError();
                return account;
            }
        }
        else
        {
            return account;
        }
    }

    if(account[4] == "" || account[2] == "" || account[5] == "" || account[6] == "" || account[7] == "")
    {
        qDebug() << "stop account load";
        return account;
    }

    if(account[4] == "South Africa")
    {
        account[4] = "voip.othos.co.za";
    }
    else if(account[4] == "Brazil")
    {
        account[4] = "voip.othos.com.br";
    }
    else if(account[4] == "Costa Rica")
    {
        account[4] = "voip.othos.co.cr";
    }
    else if(account[4] == "Asia")
    {
        account[4] = "voip.othos.co.za";
    }
    else if((account[4].startsWith("550") || account[4].startsWith("560")) && account[4].length() == 7)
    {
        account[4] = account[4].append(".ipabx.othos.net");
    }
    else if(account[4].startsWith("2787820") && account[4].length() == 11)
    {
        account[4] = account[4].append(".ipabx.othos.net");
    }

    qDebug() << "Server is: " << account[4];

    activeServer = account[4];
    activeAccountID = account[0];
    activeUser = account[2];
    QString first_codec = account[5];
    QString second_codec = account[6];
    QString third_codec = account[7];

    if(account[9]=="TLS") m_transport = "tls";
    if(account[9]=="UDP") m_transport = "udp";
    if(account[9]=="TCP") m_transport = "tcp";

    pj_status_t status = 0;
    pjsua_codec_info codecs[64];
    pj_str_t codec_id;
    unsigned count;
    count = 64;

    pjsua_enum_codecs(codecs, &count);

#ifndef _WIN32

    for(unsigned i=0 ; count > i ; i++)
    {
        codec_id = pj_str(codecs[i].codec_id.ptr);
        status = pjsua_codec_set_priority(&codec_id, PJMEDIA_CODEC_PRIO_DISABLED);
    }

#endif

    if(first_codec == second_codec)
    {
        codec_id = pj_str((char*)first_codec.toStdString().c_str());
        status = pjsua_codec_set_priority(&codec_id, PJMEDIA_CODEC_PRIO_NORMAL+12);

        codec_id = pj_str((char*)third_codec.toStdString().c_str());
        status = pjsua_codec_set_priority(&codec_id,  PJMEDIA_CODEC_PRIO_NORMAL+11);
    }
    else if(first_codec == third_codec || second_codec == third_codec)
    {
        codec_id = pj_str((char*)first_codec.toStdString().c_str());
        status = pjsua_codec_set_priority(&codec_id, PJMEDIA_CODEC_PRIO_NORMAL+12);

        codec_id = pj_str((char*)second_codec.toStdString().c_str());
        status = pjsua_codec_set_priority(&codec_id,  PJMEDIA_CODEC_PRIO_NORMAL+11);
    }
    else if(second_codec == third_codec && first_codec == third_codec)
    {
        codec_id = pj_str((char*)first_codec.toStdString().c_str());
        status = pjsua_codec_set_priority(&codec_id, PJMEDIA_CODEC_PRIO_NORMAL+12);
    }
    else
    {
        codec_id = pj_str((char*)first_codec.toStdString().c_str());
        status = pjsua_codec_set_priority(&codec_id, PJMEDIA_CODEC_PRIO_NORMAL+12);

        codec_id = pj_str((char*)second_codec.toStdString().c_str());
        status = pjsua_codec_set_priority(&codec_id,  PJMEDIA_CODEC_PRIO_NORMAL+11);

        codec_id = pj_str((char*)third_codec.toStdString().c_str());
        status = pjsua_codec_set_priority(&codec_id,  PJMEDIA_CODEC_PRIO_NORMAL+10);
    }

    return account;
}

bool Telephony::unregisterAccount()
{
   if(!pjsua_acc_is_valid(acc_id)) return false;

   if(telephony->controller->screenPhone->is_registered() == 0) return false;

   telephony->controller->screenPhone->setMwi_info(0);

   pj_status_t ret =  pjsua_acc_del(acc_id);

   if(ret == PJ_SUCCESS)
   {
       telephony->controller->screenPhone->setIs_registered(0);
       telephony->controller->screenPhone->setRegister_reason("");
       return true;
   }
   else
   {
       return false;
   }
}

bool Telephony::registerAccount(const QString &account)
{
    pj_status_t status;
    QVector<QString> acc(loadAccount(account));
    telephony->controller->screenPhone->setIs_registered(0);

    if(acc.count()==0)
    {
        qDebug() << "account not found ";
        return false;
    }

    if(acc[4]=="" || acc[2] == "" || acc[5] == "" || acc[6] == "" || acc[7] == "")
    {
        qDebug() << "account not set, not loading then ";
        controller->screenPhone->setRegister_reason("Account not loaded");
        return false;
    }

    qDebug() << "account found " << acc[1];

    char p_uri[190];
    char p_id[190];
    char p_user[190];
    char p_password[190];
    char p_server[190];

    QString id = "sip:"+acc[2]+"@"+acc[4];
    QString uri = "sip:"+acc[4]+";transport="+m_transport;
    QString password = acc[3];
    QString user = acc[2];
    QString server = acc[4];

    //qDebug() << "Account Loaded: " << acc[0] << ", " << acc[1] << ", " << acc[2] << ", " << acc[3] << ", " << acc[4] << ", " << acc[5] << ", "<< acc[6] << ", " << acc[7] << ", "<< acc[8]<< ", "<< acc[9]<< ", "<< acc[10];

    pjmedia_srtp_use srtp = PJMEDIA_SRTP_DISABLED;

    if(acc[8]!="")
    {
        if(acc[8]=="No") srtp = PJMEDIA_SRTP_DISABLED;
        if(acc[8]=="Optional") srtp = PJMEDIA_SRTP_OPTIONAL;
        if(acc[8]=="Required") srtp = PJMEDIA_SRTP_MANDATORY;
    }

    strcpy(p_uri, uri.toLocal8Bit().data());
    p_uri[uri.size()] = 0;

    strcpy(p_id, id.toLocal8Bit().data());
    p_id[id.size()] = 0;

    strcpy(p_user, user.toLocal8Bit().data());
    p_user[user.size()] = 0;

    strcpy(p_password, password.toLocal8Bit().data());
    p_password[password.size()] = 0;

    strcpy(p_server, server.toLocal8Bit().data());
    p_server[server.size()] = 0;

    pjsua_acc_config cfg;

    pjsua_acc_config_default(&cfg);

    cfg.allow_via_rewrite=PJ_FALSE;
    cfg.allow_contact_rewrite=PJ_FALSE;

    cfg.id = pj_str(p_id);
    qDebug() << "Setting SIP REG URI: " << p_uri;
    cfg.reg_uri = pj_str(p_uri);
    cfg.cred_count = 1;
    cfg.cred_info[0].realm = pj_str((char*)"*");
    cfg.cred_info[0].scheme = pj_str((char*)"Digest");
    cfg.cred_info[0].username = pj_str(p_user);
    cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    cfg.cred_info[0].data = pj_str(p_password);

    cfg.use_srtp = srtp;

    //TLS SUPPORT
    cfg.transport_id = controller->telApi->t_udp;
    cfg.srtp_secure_signaling = 0;

    if(acc[9]!="")
    {
        if(acc[9]=="UDP")
        {
             cfg.transport_id = controller->telApi->t_udp;
        }
        if(acc[9]=="TCP")
        {
            cfg.transport_id = controller->telApi->t_tcp;
        }
        if(acc[9]=="TLS")
        {
            cfg.transport_id = controller->telApi->t_tls;
            cfg.srtp_secure_signaling = 1;
        }
    }

    telephony->controller->screenPhone->setMwi_info(0);

    telephony->controller->screenPhone->setRegister_reason("Trying...");
    telephony->controller->screenPhone->setIs_registered(2);

    status = pjsua_acc_add(&cfg, PJ_FALSE, &acc_id);
    qDebug() << "Registering...";
    if (status != PJ_SUCCESS)
    {
        telephony->controller->screenPhone->setIs_registered(0);
        error_exit("Error adding account", status);
        return false;
    }
    else
    {
        return true;
    }
}

int Telephony::callTo(const QString &url)
{

#ifndef _WIN32

    pj_hostent he;
    pj_status_t has_internet;
    pj_str_t host = pj_str((char*)activeServer.toStdString().c_str());
    has_internet = pj_gethostbyname( &host, &he);

    if (has_internet != PJ_SUCCESS) {
        qDebug() << "No internet ";
        telephony->controller->screenPhone->setIs_registered(0);
        unregisterAccount();
    }

#endif
    if(telephony->controller->screenPhone->is_registered()==0)
    {
        qDebug() << "Error when calling, Not registered";
        return -1;
    }

    QString dialUri;
    dialUri = QString("sip:"+url+"@"+activeServer.append(";transport=").append(m_transport));

    if (dialUri.size() > 149)
    {
        qDebug() << "Error when calling, number too long";
        return -1;
    }

    char ch_url[150];

    strcpy(ch_url, dialUri.toLocal8Bit().data());
    ch_url[dialUri.size()] = 0;

    pj_str_t uri = pj_str(ch_url);
    pjsua_call_id call_id;

    telephony->controller->qmlviewer->rootContext()->setContextProperty("lastDial", url);

    qDebug() << "Dialling " << uri.ptr;
    pj_status_t status = pjsua_call_make_call((pjsua_acc_is_valid(acc_id) ? acc_id : NULL), &uri, 0, NULL, NULL, &call_id);

    if (status != PJ_SUCCESS)
    {
        qDebug() << "Error when calling " << status;
        return -1;
    }

    qDebug() << "OK on dialing " << status;

    pjsua_call_info ci;
    pjsua_call_get_info(call_id, &ci);

    telephony->controller->screenPhone->outgoingCall(call_id, ci);

    return (int)call_id;
}

void Telephony::hangUp(const int &call_id)
{
    pjsua_call_info ci;
    pjsua_call_id cid = call_id;

    if(cid>=0)
    {
        pjsua_call_get_info(cid,&ci);
        pjsua_call_hangup(cid,0,0,0);
    }
}

int Telephony::init()
{
  pj_status_t status;

  status = pjsua_create();
  {
      pjsua_config cfg;
      pjsua_logging_config log_cfg;

      pjsua_config_default(&cfg);
      cfg.cb.on_incoming_call = &on_incoming_call;
      cfg.cb.on_call_media_state = &on_call_media_state;
      cfg.cb.on_call_state = &on_call_state;
      cfg.cb.on_reg_state2 = &on_reg_state2;
      cfg.cb.on_mwi_info = &on_mwi_info;

      pjsua_logging_config_default(&log_cfg);
      log_cfg.console_level = 0;

      // user Agent
      char useragent[80];
      pj_ansi_snprintf(useragent, sizeof(useragent), "Othos O2C %s", APP_VERSION);
      cfg.user_agent = pj_str(useragent);

      status = pjsua_init(&cfg, &log_cfg, NULL);
      if (status != PJ_SUCCESS) error_exit("Error in pjsua_init()", status);
  }

  /* Add UDP transport. */
  {
      pjsua_transport_config cfg;
      pjsua_transport_config_default(&cfg);
      cfg.port = controller->qmlviewer->rootContext()->contextProperty("sipPort").toInt();
      status = pjsua_transport_create(PJSIP_TRANSPORT_TLS, &cfg, &t_tls);
      if (status != PJ_SUCCESS) error_exit("Error creating transport", status);
      status = pjsua_transport_create(PJSIP_TRANSPORT_TCP, &cfg, &t_tcp);
      if (status != PJ_SUCCESS) error_exit("Error creating transport", status);
      status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, &t_udp);
      if (status != PJ_SUCCESS) error_exit("Error creating transport", status);
  }

  /* Initialization is done, now start pjsua */
  status = pjsua_start();
  if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);

  telephony->controller->screenPhone->setVolume(float(0.5), "mic");
  telephony->controller->screenPhone->setVolume(float(0.5), "spk");

  return 0;
}
