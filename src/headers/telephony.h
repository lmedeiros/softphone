#ifndef TELEPHONY_H
#define TELEPHONY_H
#include <pjsua-lib/pjsua.h>
#include <QObject>
#include <QTextStream>
class Controller;

class Telephony : public QObject
{
    Q_OBJECT
public:
    explicit Telephony(Controller *ctrl, QObject *parent = 0);

    Q_INVOKABLE bool registerAccount(const QString &account);
    Q_INVOKABLE bool unregisterAccount();
    Q_INVOKABLE int callTo(const QString &url, unsigned has_video);
    Q_INVOKABLE void hangUp(const int &call_id);
    Q_INVOKABLE void hold(const int &call_id);
    Q_INVOKABLE void unhold(const int &call_id);
    Q_INVOKABLE void answerCall(const int &call_id);
    Q_INVOKABLE int transferCall(int callId, const QString& number);
    Q_INVOKABLE void stopRecordCall(int call_id);
    Q_INVOKABLE int recordCall(int call_id);

    QVector<QString> loadAccount(const QString &account_id);

    static void on_mwi_info(pjsua_acc_id acc_id, pjsua_mwi_info* mwi_info);
    static void error_exit(const char *title, pj_status_t status);
    static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata);
    static void on_call_state(pjsua_call_id call_id, pjsip_event *e);
    static void on_reg_state2(pjsua_acc_id acc, pjsua_reg_info *info);
    static void on_call_media_state(pjsua_call_id call_id);

    int init();

    Controller *controller;
    pjsua_acc_id acc_id;
    pjsua_recorder_id rec_id;
    QString activeServer;
    QString activeUser;
    QString activeAccountID;

    pjsua_transport_id t_udp;
    pjsua_transport_id t_tls;
    pjsua_transport_id t_tcp;

    QString m_transport;

    bool has_srtp;

    int input_dev;
    int output_dev;
    int ring_dev;

private:
    static Telephony *telephony;

signals:
    
public slots:
    
};

#endif // TELEPHONY_H
