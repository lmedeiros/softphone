#include "headers/screenphone.h"
#include "headers/controller.h"
#include <QDebug>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileSystemModel>
#include <QDirModel>
#include <QDesktopServices>
#include <QUrl>

ScreenPhone::ScreenPhone(Controller *ctrl, QObject *parent) :
    QObject(parent)
{
    controller = ctrl;

    this->m_active_channel = 1;
    this->m_c1_call_id = -1;
    this->m_c2_call_id = -1;
    this->m_c1_state = "available";
    this->m_c2_state = "available";
    this->m_is_registered = 0;
    this->m_c1_sip_actiontext = "";
    this->m_c2_sip_actiontext = "";
    this->m_c1_timer = -1;
    this->m_c2_timer = -1;
    this->m_miss_notify = 0;
    this->m_mwi_info = 0;
}

struct my_call_data *tone_gen;

struct my_call_data *call_init_tonegen(pjsua_call_id call_id)
{
    pj_pool_t *pool;
    struct my_call_data *cd;
    pjsua_call_info ci;

    if (call_id !=-1 ) {
        pjsua_call_get_info(call_id, &ci);
    }

    pool = pjsua_pool_create("mycall", 512, 512);
    cd = PJ_POOL_ZALLOC_T(pool, struct my_call_data);
    cd->pool = pool;

    pjmedia_tonegen_create(cd->pool, 8000, 1, 160, 16, 0, &cd->tonegen);
    pjsua_conf_add_port(cd->pool, cd->tonegen, &cd->toneslot);

    if (call_id !=-1 ) {
        pjsua_conf_connect(cd->toneslot, ci.conf_slot);
    }

    pjsua_conf_connect(cd->toneslot, 0);

    if(call_id !=-1)
    {
        pjsua_call_set_user_data(call_id, (void*) cd);
    }

    return cd;
}

void ScreenPhone::playDTMF(pjsua_call_id call_id, const char *digits)
{
    pjmedia_tone_digit d[16];
    unsigned i, count = strlen(digits);
    struct my_call_data *cd;

    if (call_id !=-1 ) {
        cd = (struct my_call_data*) pjsua_call_get_user_data(call_id);
    } else {
        cd = tone_gen;
    }

    if (!cd)
        cd = call_init_tonegen(call_id);
    if (call_id == -1 ) {
        tone_gen = cd;
    }

    if (count > PJ_ARRAY_SIZE(d))
        count = PJ_ARRAY_SIZE(d);

    pj_bzero(d, sizeof(d));
    for (i=0; i<count; ++i) {
        d[i].digit = digits[i];
        if (call_id !=-1 ) {
            d[i].on_msec = 260;
            d[i].off_msec = 200;
        } else {
            d[i].on_msec = 120;
            d[i].off_msec = 50;
        }
        d[i].volume = 0;
        //qDebug() << "LOOP Playing DTMF " << digits;
    }

    pjmedia_tonegen_play_digits(cd->tonegen, count, d, 0);
}



void ScreenPhone::playbackAudio(const QString& path)
{
//    QSoundEffect rec;
//    rec.setSource(QUrl(path));
//    rec.play();

    QDesktopServices::openUrl(QUrl(path));

    //QSound::play(path);
}

void ScreenPhone::sendDTMF(pjsua_call_id call_id, const char* digits)
{
    const char* p_digits;

    p_digits = digits;

    pj_str_t pj_digits = pj_str((char*)p_digits);
    pjsua_call_info call_info;

    //qDebug() << "Trying to play dtmf " << digits;

    if(call_id>=0)
    {
        pjsua_call_get_info(call_id, &call_info);
        if (call_info.media_status == PJSUA_CALL_MEDIA_ACTIVE)
        {
            playDTMF(call_id, (char*)p_digits);
            pjsua_call_dial_dtmf(call_id, &pj_digits);
            return;
        }
    }
    else
    {
        playDTMF(call_id, (char*)p_digits);
    }
}

void ScreenPhone::setVolume(const float volume, const QString &device)
{
    if(device == "mic")
    {
        pjsua_conf_adjust_rx_level(0, volume);
        controller->qmlviewer->rootContext()->setContextProperty("micVolume", volume);
    }
    else if(device == "spk")
    {
        pjsua_conf_adjust_tx_level(0, volume);
        controller->qmlviewer->rootContext()->setContextProperty("spkVolume", volume);
    }
}

void ScreenPhone::incomingCall(pjsua_call_id &call_id, pjsua_call_info &call_info, const QString &account_id)
{
    PJ_UNUSED_ARG(account_id);
    if(this->c1_call_id()>=0 && this->c2_call_id()>=0)
    {
        qDebug() << "C1 busy and C2 busy";
        controller->telApi->hangUp(call_id);
        return;
    }

    QString cinfo = QString(call_info.remote_info.ptr);
    cinfo = cinfo.mid(0, cinfo.indexOf("@"));
    cinfo = cinfo.mid(cinfo.indexOf(":")+1);

    QString callerId = QString(call_info.remote_info.ptr).mid(0, QString(call_info.remote_info.ptr).indexOf("<")).replace("\"","");

    controller->screenContacts->getContact(cinfo);

    if(this->c1_call_id() == -1)
    {
        qDebug() << "C1 free";
        this->setC1_call_id(call_id);
        this->setC1_state("incoming");
        this->setC1_sip_actiontext("Incoming Call\n" + callerId + " " + cinfo);
    }
    else if(this->c2_call_id() == -1)
    {
        qDebug() << "C1 busy and C2 free";
        this->setC2_call_id(call_id);
        this->setC2_state("incoming");
        this->setC2_sip_actiontext("Incoming Call\n" + callerId + " " + cinfo);
    }

    this->ringsound.setSource(QUrl("qrc:/interface/assets/ring.wav"));
    this->ringsound.setLoopCount(999999999);

    if(this->ringsound.isPlaying())
    {
        this->ringsound.setMuted(false);
    }
    else
    {
        this->ringsound.play();
    }

    controller->setQml("screens/phoneactive.qml", "phoneactive");
}

void ScreenPhone::hangUp(pjsua_call_id &call_id, const QString &account_id)
{
    if(call_id<0) { return; }

    qDebug() << "Hanging up call: " << QString::number(call_id);

    register_call(call_id, account_id);

    if(m_c1_call_id == call_id)
    {
        this->setC1_timer(-1);
        this->setC1_call_id(-1);
        this->setC1_state("available");
        this->setC1_sip_actiontext("");
        this->setC1_downrate("");
        this->setC1_uprate("");
        this->setC1_loss("");
        this->setC1_stream_info("");

        if(c2_call_id() >= 0)
        {
            this->setActive_channel(2);
            if(c2_state() == "hold") this->unhold(this->m_c2_call_id);
        }
    }

    if(m_c2_call_id == call_id)
    {
        this->setC2_timer(-1);
        this->setC2_call_id(-1);
        this->setC2_state("available");
        this->setC2_sip_actiontext("");
        this->setC2_downrate("");
        this->setC2_uprate("");
        this->setC2_loss("");
        this->setC2_stream_info("");

        if(c1_call_id() >= 0)
        {
            this->setActive_channel(1);
            if (c1_state() == "hold") this->unhold(this->m_c1_call_id);
        }
    }

    if(this->m_c1_state!="incoming" && this->m_c2_state!="incoming")
    {
        this->ringsound.setMuted(true);
    }

    if(m_c1_call_id == -1 && m_c2_call_id == -1 )
    {
        controller->setQml("screens/phone.qml", "phone");
    }
}

void ScreenPhone::register_call(pjsua_call_id call_id, const QString &account_id)
{
    pjsua_call_info call_info;
    pjsua_call_get_info(call_id, &call_info);

    QString tx = "0";
    QString rx = "0";
    QString local_loss = "0";
    QString remote_loss = "0";
    QString codec = "N/A";

    QString callDuration = QString::number(call_info.connect_duration.sec);

    if(callDuration.toInt() > 0)
    {
        pjsua_stream_stat stream_stat;
        pjsua_call_get_stream_stat(call_id, call_info.media[0].index, &stream_stat);

        pjsua_stream_info stream_info;
        pjsua_call_get_stream_info(call_id, call_info.media[0].index, &stream_info);

        codec = (stream_info.info.aud.fmt.encoding_name.ptr);
        codec = codec.mid(0,codec.indexOf("strm")).remove(QRegExp("[^a-zA-Z\\d\\s]"));
        tx = QString::number(stream_stat.rtcp.rx.bytes);
        rx = QString::number(stream_stat.rtcp.tx.bytes);
        local_loss = QString::number(stream_stat.rtcp.rx.loss);
        remote_loss = QString::number(stream_stat.rtcp.tx.loss);
    }

    QString linfo = QString(call_info.local_info.ptr);

    QString cinfo = QString(call_info.remote_info.ptr);
    cinfo = cinfo.mid(0, cinfo.indexOf("@"));
    cinfo = cinfo.mid(cinfo.indexOf(":")+1);

    QString callerId = QString(call_info.remote_info.ptr).mid(0, QString(call_info.remote_info.ptr).indexOf("<")).replace("\"","");
    callerId = callerId.mid(0, callerId.indexOf("@"));
    callerId = callerId.mid(callerId.indexOf(":")+1);

    QString callType;

    QStringList callContactInfo = this->getContact(cinfo);

    if(call_info.role==0)
    {
        callType = "OUT";
        if(call_info.connect_duration.sec==0)
        {
            callType = "FAIL";
        }
    }
    else if(call_info.role==1)
    {
        callType = "IN";
        if(call_info.connect_duration.sec==0)
        {
            this->setMiss_notify(true);
            callType = "MISS";
        }
    }

    if(!controller->screenLog->insertLog(linfo, cinfo, callType, QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"), callDuration, account_id, ((callContactInfo[5]==0) ? callerId : callContactInfo[0]) , callContactInfo[4], callContactInfo[2], rx, tx, "N/A", local_loss, remote_loss, codec, callContactInfo[5], callContactInfo[6])) qDebug() << "Error when register the call";
}

QStringList ScreenPhone::getContact(const QString& number)
{
    QStringList activeCallContact;

    if(controller->db->mydb.open())
    {
        QString queryString("SELECT name, surname, pic_path, number, description, number_id, contact_id FROM contact_number NATURAL JOIN contacts WHERE number LIKE '" + number + "' LIMIT 1");

        //qDebug() << "getContact query: " << queryString;
        QSqlQuery query = controller->db->mydb.exec(queryString);

        if(controller->db->mydb.lastError().number()>0)
        {
            qDebug() << "getContact error: " << controller->db->mydb.lastError();
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
                activeCallContact.append(query.value(5).toString());
                activeCallContact.append(query.value(6).toString());

                return activeCallContact;
            }
        }
    }

    activeCallContact.append(number);
    activeCallContact.append("");
    activeCallContact.append("");
    activeCallContact.append(number);
    activeCallContact.append("Unknown");
    activeCallContact.append(0);
    activeCallContact.append(0);

    return activeCallContact;
}

void ScreenPhone::outgoingCall(pjsua_call_id &call_id, pjsua_call_info &call_info)
{
    QString cinfo = QString(call_info.remote_info.ptr);
    cinfo = cinfo.mid(0, cinfo.indexOf("@"));
    cinfo = cinfo.mid(cinfo.indexOf(":")+1);

    controller->screenContacts->getContact(cinfo);

    QStringList contact = getContact(cinfo);

    if(this->active_channel()==1)
    {
        this->setC1_call_id(call_id);
        this->setC1_state("dialing");
        this->setC1_sip_actiontext("Dialing to\n" + ((contact[0]==cinfo) ? "" : contact[0] + " ") + contact[4] + " " + cinfo );
    }

    if(this->active_channel()==2)
    {
        this->setC2_call_id(call_id);
        this->setC2_state("dialing");
        this->setC2_sip_actiontext("Dialing to\n" + ((contact[0]==cinfo) ? "" : contact[0] + " ") + contact[4] + " " + cinfo );
    }

    controller->setQml("screens/phoneactive.qml", "phoneactive");
}

void ScreenPhone::setNetworkRates()
{
    if(this->active_channel() == 1 && this->m_c1_call_id >= 0 && this->m_c1_state != "available")
    {
        static char c1_buffer[1024*1];
        pjsua_call_dump(this->m_c1_call_id, PJ_TRUE, c1_buffer, sizeof(c1_buffer), "  ");

        pjsua_call_info cinfo;
        pjsua_call_get_info(this->m_c1_call_id, &cinfo);

        if(!controller->telApi->has_srtp)
        {
            pjsua_stream_info stat;
            pjsua_call_get_stream_info(this->m_c1_call_id, cinfo.media[0].index, &stat);

            QString codec(stat.info.aud.fmt.encoding_name.ptr);
            codec = codec.mid(0,codec.indexOf("strm")).remove(QRegExp("[^a-zA-Z\\d\\s]"));

            pjsua_stream_stat stream_stat;
            pjsua_call_get_stream_stat(this->m_c1_call_id, cinfo.media[0].index, &stream_stat);

            this->m_c1_latency = QString::number(stream_stat.rtcp.rtt.last/1000);
            this->setC1_stream_info(codec.append("(").append(this->m_c1_latency).append(" ms) ").append(controller->telApi->m_transport.toUpper()));
        }
        else
        {
            this->setC1_stream_info("SRTP Encrypted Call");
        }

        QStringList qbuff = QString(c1_buffer).split("\n");

        //qDebug() << "RTT ::::: " << QString::number(stream_stat.rtcp.rtt.last);
        qDebug() << "RTT ::::: " << QString(c1_buffer);

        this->setC1_downrate(qbuff[5].mid(qbuff[5].indexOf("@avg=")+5).split('/')[1]);
        this->setC1_uprate(qbuff[11].mid(qbuff[11].indexOf("@avg=")+5).split('/')[1]);

        this->setC1_loss(qbuff[6].mid(qbuff[6].indexOf("loss=")+5).split(',')[0]);

        memset(&c1_buffer[0], 0, sizeof(c1_buffer));

        return;
    }

    if(this->active_channel() == 2 && this->m_c2_call_id >= 0 && this->m_c2_state != "available")
    {
        static char c2_buffer[1024*1];
        pjsua_call_dump(this->m_c2_call_id, PJ_TRUE, c2_buffer, sizeof(c2_buffer), "  ");

        pjsua_call_info cinfo;
        pjsua_call_get_info(this->m_c2_call_id, &cinfo);

        if(!controller->telApi->has_srtp)
        {
            pjsua_stream_info stat;
            pjsua_call_get_stream_info(this->m_c2_call_id, cinfo.media[0].index, &stat);

            QString codec(stat.info.aud.fmt.encoding_name.ptr);
            codec = codec.mid(0,codec.indexOf("strm")).remove(QRegExp("[^a-zA-Z\\d\\s]"));

            pjsua_stream_stat stream_stat;
            pjsua_call_get_stream_stat(this->m_c2_call_id, cinfo.media[0].index, &stream_stat);

            this->m_c2_latency = QString::number(stream_stat.rtcp.rtt.last/1000);
            this->setC2_stream_info(codec.append("(").append(this->m_c2_latency).append(" ms) ").append(controller->telApi->m_transport.toUpper()));
        }
        else
        {
            this->setC1_stream_info("SRTP Encrypted Call");
        }

        QStringList qbuff = QString(c2_buffer).split("\n");
        this->setC2_downrate(qbuff[5].mid(qbuff[5].indexOf("@avg=")+5).split('/')[1]);
        this->setC2_uprate(qbuff[11].mid(qbuff[11].indexOf("@avg=")+5).split('/')[1]);

        this->setC2_loss(qbuff[6].mid(qbuff[6].indexOf("loss=")+5).split(',')[0]);

        memset(&c2_buffer[0], 0, sizeof(c2_buffer));

        return;
    }

    this->m_c1_latency = "0";
    this->m_c2_latency = "0";

    this->setC1_downrate("0");
    this->setC1_uprate("0");
    this->setC2_downrate("0");
    this->setC2_uprate("0");
}

void ScreenPhone::showVideoWindow(pjsua_call_id call_id)
{
    pjsua_call_info cinfo;
    unsigned i;

    if (call_id == -1)
    return;

    pjsua_call_get_info(call_id, &cinfo);
    qDebug() << "Opening video window.......";
    for (i = 0; i < cinfo.media_cnt; ++i) {
        if ((cinfo.media[i].type == PJMEDIA_TYPE_VIDEO) && (cinfo.media[i].dir & PJMEDIA_DIR_DECODING))
        {
            pjsua_vid_win_info wi;
            pjsua_vid_win_get_info(cinfo.media[i].stream.vid.win_in, &wi);

            const pjmedia_rect_size size = {320, 240};
            const pjmedia_coord pos = {(controller->qmlviewer->x()+controller->qmlviewer->width()),controller->qmlviewer->y()+210};
            pjsua_vid_win_set_size(cinfo.media[i].stream.vid.win_in, &size);
            pjsua_vid_win_set_pos(cinfo.media[i].stream.vid.win_in, &pos);

            this->controller->screenSettings->showPreviewWindow(controller->settings->m_videoDevice.m_name.append("::::").append(controller->settings->m_videoDevice.m_id));

            this->m_video_viewer = new VidWin(&wi.hwnd);

            break;
        }
    }
}

void ScreenPhone::connectedCall(pjsua_call_id &call_id, pjsua_call_info &call_info)
{
    QString cinfo = QString(call_info.remote_info.ptr);

    cinfo = cinfo.mid(0, cinfo.indexOf("@"));
    cinfo = cinfo.mid(cinfo.indexOf(":")+1);

    if (call_info.media[0].type == PJMEDIA_TYPE_VIDEO)
    {
        qDebug() << "Video Call";
        this->showVideoWindow(call_id);
    }
    this->ringsound.setMuted(true);

    if(this->c1_call_id()==call_id)
    {
        this->m_c1_latency = "0";
        qDebug() << "connect call c1state" << m_c1_state;
        if(m_c1_state != "hold")
        {
            this->setActive_channel(1);
            this->setC1_timer(0);
        }

        this->setC1_state("busy");
        this->setC1_sip_actiontext("Connected to\n" + cinfo);
    }

    if(this->c2_call_id()==call_id)
    {
        this->m_c2_latency = "0";
        qDebug() << "connect call c2state" << m_c2_state;
        if(m_c2_state != "hold")
        {
            this->setActive_channel(2);
            this->setC2_timer(0);
        }

        this->setC2_state("busy");
        this->setC2_sip_actiontext("Connected to\n" + cinfo);
    }
}

void ScreenPhone::keypadEvent(const QString &digit)
{
    QVariant actualKeypadText = controller->qmlviewer->rootContext()->contextProperty("keypadText");

    if(digit=="c1")
    {
        setActive_channel(1);
        changeChannel();
        return;
    }

    if(digit=="c2")
    {
        setActive_channel(2);
        changeChannel();
        return;
    }

    if(digit=="backspace")
    {
        QString temp(actualKeypadText.toString());
        controller->qmlviewer->rootContext()->setContextProperty("keypadText", temp.left(temp.length()-1));
        return;
    }

    if(digit=="clear")
    {
        controller->qmlviewer->rootContext()->setContextProperty("keypadText", "");
        return;
    }

    if(active_channel()==1)
    {
        sendDTMF((pjsua_call_id)this->m_c1_call_id , (char*)digit.toStdString().c_str());
    }
    else if(active_channel()==2)
    {
        sendDTMF((pjsua_call_id)this->m_c1_call_id, (char*)digit.toStdString().c_str());
    }

    controller->qmlviewer->rootContext()->setContextProperty("keypadText",actualKeypadText.toString()+digit);
}

void ScreenPhone::setDialText(const QString &to_num)
{
    QStringList to_num_no_at;
    QStringList to_num_no_cid;
    QString to_num_ok = to_num;

    if(to_num.indexOf("@")>0)
    {
        to_num_no_at = to_num.split("@");
        to_num_ok = to_num_no_at[0];
    }

    if(to_num.indexOf("<")>0)
    {
        to_num_no_cid = to_num_no_at[0].split("<");
        to_num_ok = to_num_no_cid[1];
    }

    controller->qmlviewer->rootContext()->setContextProperty("keypadText", to_num_ok);
}

void ScreenPhone::unhold(pjsua_call_id &call_id)
{
    pjsua_call_info call_info;
    pjsua_call_get_info(call_id, &call_info);

    QString cinfo = QString(call_info.remote_info.ptr);

    cinfo = cinfo.mid(0, cinfo.indexOf("@"));
    cinfo = cinfo.mid(cinfo.indexOf(":")+1);

    if(this->c1_call_id()==call_id)
    {
        this->setC1_sip_actiontext("Connected to \n " + cinfo);
    }

    if(this->c2_call_id()==call_id)
    {
        this->setC2_sip_actiontext("Connected to \n" + cinfo);
    }
}

void ScreenPhone::hold(pjsua_call_id &call_id)
{
    pjsua_call_info call_info;
    pjsua_call_get_info(call_id, &call_info);

    QString cinfo = QString(call_info.remote_info.ptr);

    cinfo = cinfo.mid(0, cinfo.indexOf("@"));
    cinfo = cinfo.mid(cinfo.indexOf(":")+1);

    if(this->c1_call_id()==call_id)
    {
        this->setC1_state("hold");
        this->setC1_sip_actiontext("Call on hold\n" + cinfo);
    }

    if(this->c2_call_id()==call_id)
    {
        this->setC2_state("hold");
        this->setC2_sip_actiontext("Call on hold\n" + cinfo);
    }
}

void ScreenPhone::loadCodecList()
{
    QStringList codecList;
    pjsua_codec_info codec_info[64];
    unsigned count;
    count = 64;
    pjsua_enum_codecs(codec_info, &count);

    for(unsigned i=0 ; count > i ; i++)
    {
        codecList.append(QString(codec_info[i].codec_id.ptr));
    }

    setCodec_list(codecList);
}

void ScreenPhone::setFileModel()
{
    QDir dir;

    dir.setPath(QGuiApplication::applicationDirPath().append("/recs"));

    QStringList list = dir.entryList(QDir::Files);
    setRec_list(list);
}


void ScreenPhone::changeChannel()
{
    int call_id = -1;
    pjsua_call_info call_info;

    if(m_active_channel==2 && m_c2_call_id >= 0)
    {
        call_id = m_c2_call_id;
    }
    else if(m_active_channel==2 && m_c2_call_id >= 0)
    {
        call_id = m_c1_call_id;
    }

    if(call_id<0) return;

    pjsua_call_get_info(call_id, &call_info);

    QString cinfo = QString(call_info.remote_info.ptr);

    cinfo = cinfo.mid(0, cinfo.indexOf("@"));
    cinfo = cinfo.mid(cinfo.indexOf(":")+1);


    switch (call_info.state)
    {
        case 1:
        {
            if(m_active_channel==1)
            {
                setC1_sip_actiontext("Dialing to " + QString(cinfo));
            }
            else
            {
                setC2_sip_actiontext("Dialing to " + QString(cinfo));
            }
            break;
        }
        case 2:
        {
            if(m_active_channel==1)
            {
                setC1_sip_actiontext("Incoming " + QString(cinfo));
            }
            else
            {
                setC2_sip_actiontext("Incoming " + QString(cinfo));
            }
            break;
        }
        case 3:
        {
            if(m_active_channel==1)
            {
                setC1_sip_actiontext("Dialing to " + QString(cinfo));
            }
            else
            {
                setC2_sip_actiontext("Dialing to " + QString(cinfo));
            }
            break;
        }
        case 4:
        {
            if(m_active_channel==1)
            {
                setC1_sip_actiontext("Dialing to " + QString(cinfo));
            }
            else
            {
                setC2_sip_actiontext("Dialing to " + QString(cinfo));
            }

            break;
        }
        case 5:
        {
            if(m_active_channel==1)
            {
                setC1_sip_actiontext("Connected to " + QString(cinfo));
            }
            else
            {
                setC2_sip_actiontext("Connected to " + QString(cinfo));
            }

            break;
        }
        case 6:
        {
            if(m_active_channel==1)
            {
                setC1_sip_actiontext("Call ended");
            }
            else
            {
                setC2_sip_actiontext("Call ended");
            }

            break;
        }
        default:
        {
            if(m_active_channel==1)
            {
                setC1_sip_actiontext("");
            }
            else
            {
                setC2_sip_actiontext("");
            }
            break;
        }
    }
}

void ScreenPhone::setC1_timer(const int sec)
{
    m_c1_timer = sec;
    emit c1_timerChanged(sec);
}

void ScreenPhone::setC2_timer(const int sec)
{
    m_c2_timer = sec;
    emit c2_timerChanged(sec);
}

void ScreenPhone::setC1_call_id(const int cid)
{
    m_c1_call_id = cid;
    emit c1_call_idChanged(cid);
}

void ScreenPhone::setC2_call_id(const int cid)
{
    m_c2_call_id = cid;
    emit c2_call_idChanged(cid);
}

void ScreenPhone::setC1_state(const QString& state)
{
    m_c1_state = state;
    emit c1_stateChanged(state);
}

void ScreenPhone::setC2_state(const QString& state)
{
    m_c2_state = state;
    emit c2_stateChanged(state);
}

void ScreenPhone::setIs_registered(const int is)
{
    m_is_registered = is;
    emit is_registeredChanged(is);
}

void ScreenPhone::setMiss_notify(const bool show)
{
    m_miss_notify = show;
    emit miss_notifyChanged(show);
}

void ScreenPhone::setActive_channel(const int channel)
{
    m_active_channel = channel;
    changeChannel();
    emit active_channelChanged(channel);
}

void ScreenPhone::setC1_sip_actiontext(const QString& text)
{
    m_c1_sip_actiontext = text;
    emit c1_sip_actiontextChanged(text);
}

void ScreenPhone::setC2_sip_actiontext(const QString& text)
{
    m_c2_sip_actiontext = text;
    emit c2_sip_actiontextChanged(text);
}

void ScreenPhone::setMwi_info(const int cid)
{
    m_mwi_info = cid;
    emit mwi_infoChanged(cid);
}

void ScreenPhone::setRegister_reason(const QString& reason)
{
    m_register_reason = reason;
    emit register_reasonChanged(reason);
}

void ScreenPhone::setCodec_list(const QStringList& list)
{
    m_codec_list = list;
    emit codec_listChanged(list);
}

void ScreenPhone::setRec_list(const QStringList& list)
{
    m_rec_list = list;
    emit rec_listChanged(list);
}

void ScreenPhone::setC1_stream_info(const QString& info)
{
    m_c1_stream_info = info;
    emit c1_stream_infoChanged(info);
}

void ScreenPhone::setC2_stream_info(const QString& info)
{
    m_c2_stream_info = info;
    emit c2_stream_infoChanged(info);
}

void ScreenPhone::setC1_downrate(const QString& text)
{
    m_c1_downrate = text;
    emit c1_downrateChanged(text);
}

void ScreenPhone::setC1_uprate(const QString& text)
{
    m_c1_uprate = text;
    emit c1_uprateChanged(text);
}

void ScreenPhone::setC2_downrate(const QString& text)
{
    m_c2_downrate = text;
    emit c2_downrateChanged(text);
}

void ScreenPhone::setC2_uprate(const QString& text)
{
    m_c2_uprate = text;
    emit c2_uprateChanged(text);
}

void ScreenPhone::setC1_loss(const QString& text)
{
    m_c1_loss = text;
    emit c1_lossChanged(text);
}

void ScreenPhone::setC2_loss(const QString& text)
{
    m_c2_loss = text;
    emit c2_lossChanged(text);
}

int ScreenPhone::c1_call_id()
{
    return m_c1_call_id;
}

int ScreenPhone::c2_call_id()
{
    return m_c2_call_id;
}

int ScreenPhone::c1_timer()
{
    return m_c1_timer;
}

int ScreenPhone::c2_timer()
{
    return m_c2_timer;
}

bool ScreenPhone::miss_notify()
{
    return m_miss_notify;
}

int ScreenPhone::is_registered()
{
    return m_is_registered;
}

QString ScreenPhone::c1_state()
{
    return m_c1_state;
}

QString ScreenPhone::c2_state()
{
    return m_c2_state;
}

int ScreenPhone::active_channel()
{
    return m_active_channel;
}

int ScreenPhone::mwi_info()
{
    return m_mwi_info;
}

QString ScreenPhone::c1_sip_actiontext()
{
    return m_c1_sip_actiontext;
}

QString ScreenPhone::c2_sip_actiontext()
{
    return m_c2_sip_actiontext;
}

QString ScreenPhone::register_reason()
{
    return m_register_reason;
}

QString ScreenPhone::c1_stream_info()
{
    return m_c1_stream_info;
}

QString ScreenPhone::c2_stream_info()
{
    return m_c2_stream_info;
}


QStringList ScreenPhone::codec_list()
{
    return m_codec_list;
}

QStringList ScreenPhone::rec_list()
{
    return m_rec_list;
}

QString ScreenPhone::c1_downrate()
{
    return m_c1_downrate;
}

QString ScreenPhone::c1_uprate()
{
    return m_c1_uprate;
}

QString ScreenPhone::c2_downrate()
{
    return m_c2_downrate;
}

QString ScreenPhone::c2_uprate()
{
    return m_c2_uprate;
}

QString ScreenPhone::c1_loss()
{
    return m_c1_loss;
}

QString ScreenPhone::c2_loss()
{
    return m_c2_loss;
}
