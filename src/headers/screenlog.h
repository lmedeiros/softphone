#ifndef SCREENLOG_H
#define SCREENLOG_H

#include <QObject>
#include "dbtablemobel.h"
class Controller;

class ScreenLog : public QObject
{
    Q_OBJECT
public:
    ScreenLog(Controller *ctrl, QObject *parent = 0);
    Controller *controller;
    bool createTable();

    Q_INVOKABLE void contactInfo(const QString &number);
    Q_INVOKABLE bool insertLog(const QString &from_num, const QString &to_num, const QString &log_type, const QString &datetime, const QString &duration,
                               const QString &account_id, const QString &name, const QString &description, const QString &pic_path, const QString &rx,
                               const QString &tx, const QString &cost, const QString &local_loss, const QString &remote_loss, const QString &codec,
                               const QString &contact_id, const QString &number_id);
    Q_INVOKABLE bool clearLogs(const QString &account_id);
    Q_INVOKABLE void getLogs(const QString &account_id);
    Q_INVOKABLE bool delLog(const QString &log_id);
signals:
    
public slots:
    
};

#endif // SCREENLOG_H
