#ifndef SCREENPHONE_H
#define SCREENPHONE_H

#include <QObject>
#include <pjsua-lib/pjsua.h>
#include <QDebug>
#include <QSoundEffect>
#include "videoviewer.h"

class Controller;

struct my_call_data
{
   pj_pool_t          *pool;
   pjmedia_port       *tonegen;
   pjsua_conf_port_id  toneslot;
};

struct my_call_data *call_init_tonegen(pjsua_call_id call_id);

class ScreenPhone : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool miss_notify READ miss_notify WRITE setMiss_notify NOTIFY miss_notifyChanged)
    Q_PROPERTY(int mwi_info READ mwi_info WRITE setMwi_info NOTIFY mwi_infoChanged)
    Q_PROPERTY(int c1_timer READ c1_timer WRITE setC1_timer NOTIFY c1_timerChanged)
    Q_PROPERTY(int c2_timer READ c2_timer WRITE setC2_timer NOTIFY c2_timerChanged)
    Q_PROPERTY(int c1_call_id READ c1_call_id WRITE setC1_call_id NOTIFY c1_call_idChanged)
    Q_PROPERTY(int c2_call_id READ c2_call_id WRITE setC2_call_id NOTIFY c2_call_idChanged)
    Q_PROPERTY(QString c1_state READ c1_state WRITE setC1_state NOTIFY c1_stateChanged)
    Q_PROPERTY(QString c2_state READ c2_state WRITE setC2_state NOTIFY c2_stateChanged)
    Q_PROPERTY(int is_registered READ is_registered WRITE setIs_registered NOTIFY is_registeredChanged)
    Q_PROPERTY(int active_channel READ active_channel WRITE setActive_channel NOTIFY active_channelChanged)
    Q_PROPERTY(QString c1_sip_actiontext READ c1_sip_actiontext WRITE setC1_sip_actiontext NOTIFY c1_sip_actiontextChanged)
    Q_PROPERTY(QString c2_sip_actiontext READ c2_sip_actiontext WRITE setC2_sip_actiontext NOTIFY c2_sip_actiontextChanged)
    Q_PROPERTY(QString register_reason READ register_reason WRITE setRegister_reason NOTIFY register_reasonChanged)
    Q_PROPERTY(QStringList codec_list READ codec_list WRITE setCodec_list NOTIFY codec_listChanged)
    Q_PROPERTY(QStringList rec_list READ rec_list WRITE setRec_list NOTIFY rec_listChanged)
    Q_PROPERTY(QString c1_stream_info READ c1_stream_info WRITE setC1_stream_info NOTIFY c1_stream_infoChanged)
    Q_PROPERTY(QString c2_stream_info READ c2_stream_info WRITE setC2_stream_info NOTIFY c2_stream_infoChanged)
    Q_PROPERTY(QString c1_downrate READ c1_downrate WRITE setC1_downrate NOTIFY c1_downrateChanged)
    Q_PROPERTY(QString c1_uprate READ c1_uprate WRITE setC1_uprate NOTIFY c1_uprateChanged)
    Q_PROPERTY(QString c2_downrate READ c2_downrate WRITE setC2_downrate NOTIFY c2_downrateChanged)
    Q_PROPERTY(QString c2_uprate READ c2_uprate WRITE setC2_uprate NOTIFY c2_uprateChanged)
    Q_PROPERTY(QString c1_loss READ c1_loss WRITE setC1_loss NOTIFY c1_lossChanged)
    Q_PROPERTY(QString c2_loss READ c2_loss WRITE setC2_loss NOTIFY c2_lossChanged)

public:
    explicit ScreenPhone(Controller *ctrl, QObject *parent = 0);
    Controller *controller;
    VidWin* m_video_viewer;

    Q_INVOKABLE void keypadEvent(const QString &digit);
    Q_INVOKABLE void setVolume(const float volume, const QString &device);
    Q_INVOKABLE void setDialText(const QString &to_num);
    Q_INVOKABLE void loadCodecList();
    Q_INVOKABLE void setFileModel();
    Q_INVOKABLE void playbackAudio(const QString& path);
    Q_INVOKABLE void setNetworkRates();
    QStringList getContact(const QString& number);

    void register_call(pjsua_call_id call_id, const QString &account_id);
    void setC1_stream_info(const QString& info);
    void setC2_stream_info(const QString& info);
    void setCodec_list(const QStringList& list);
    void setRec_list(const QStringList& list);
    void setRegister_reason(const QString& reason);
    void setC1_call_id(const int cid);
    void setMwi_info(const int cid);
    void setC2_call_id(const int cid);
    void setC1_state(const QString& state);
    void setC2_state(const QString& state);
    void setIs_registered(const int is);
    void setActive_channel(const int channel);
    void setC1_sip_actiontext(const QString& text);
    void setC2_sip_actiontext(const QString& text);
    void setC1_timer(const int sec);
    void setC2_timer(const int sec);
    void setC1_downrate(const QString& text);
    void setC2_downrate(const QString& text);
    void setC1_uprate(const QString& text);
    void setC2_uprate(const QString& text);
    void setC1_loss(const QString& text);
    void setC2_loss(const QString& text);

    Q_INVOKABLE void setMiss_notify(bool show);

    int c1_call_id();
    int c2_call_id();
    int mwi_info();
    int is_registered();
    QString c1_stream_info();
    QString c2_stream_info();
    QString c1_state();
    QString c2_state();
    int active_channel();
    QString c1_sip_actiontext();
    QString c2_sip_actiontext();
    int c1_timer();
    int c2_timer();
    QString register_reason();
    bool miss_notify();
    QStringList rec_list();
    QStringList codec_list();
    QString c1_downrate();
    QString c1_uprate();
    QString c1_loss();
    QString c2_loss();
    QString c2_downrate();
    QString c2_uprate();

    Q_INVOKABLE void showVideoWindow(pjsua_call_id call_id);

    void changeChannel();
    void sendDTMF(pjsua_call_id call_id, const char *digits);
    void playDTMF(pjsua_call_id call_id, const char *digits);
    void hangUp(pjsua_call_id &call_id, const QString &account_id);
    void incomingCall(pjsua_call_id &call_id, pjsua_call_info &call_info, const QString &account_id);
    void outgoingCall(pjsua_call_id &call_id, pjsua_call_info &call_info);
    void connectedCall(pjsua_call_id &call_id, pjsua_call_info &call_info);
    void hold(pjsua_call_id &call_id);
    void unhold(pjsua_call_id &call_id);

    QSoundEffect ringsound;

private:
    QString m_c1_latency;
    QString m_c2_latency;
    QString m_c1_downrate;
    QString m_c1_uprate;
    QString m_c2_downrate;
    QString m_c2_uprate;
    int m_c1_call_id;
    int m_c2_call_id;
    int m_is_registered;
    QString m_c1_stream_info;
    QString m_c2_stream_info;
    QString m_c1_state;
    QString m_c2_state;
    int m_active_channel;
    int m_mwi_info;
    QString m_c1_sip_actiontext;
    QString m_c2_sip_actiontext;
    QString m_c1_loss;
    QString m_c2_loss;
    int m_c1_timer;
    bool m_miss_notify;
    int m_c2_timer;
    QStringList m_codec_list;
    QStringList m_rec_list;
    QString m_register_reason;

signals:
    void miss_notifyChanged(bool);
    void c1_timerChanged(const int);
    void c2_timerChanged(const int);
    void c1_sip_actiontextChanged(const QString&);
    void c2_sip_actiontextChanged(const QString&);
    void c1_call_idChanged(const int);
    void c2_call_idChanged(const int);
    void c1_stateChanged(const QString&);
    void c2_stateChanged(const QString&);
    void codec_listChanged(const QStringList&);
    void rec_listChanged(const QStringList&);
    void register_reasonChanged(const QString&);
    void is_registeredChanged(const int);
    void active_channelChanged(const int);
    void mwi_infoChanged(const int);
    void c1_stream_infoChanged(const QString&);
    void c2_stream_infoChanged(const QString&);
    void c1_downrateChanged(const QString&);
    void c1_uprateChanged(const QString&);
    void c2_downrateChanged(const QString&);
    void c2_uprateChanged(const QString&);
    void c1_lossChanged(const QString&);
    void c2_lossChanged(const QString&);
    
public slots:
    
};

#endif // SCREENPHONE_H
