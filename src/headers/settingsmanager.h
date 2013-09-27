#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QHash>
#include <QtXml/QDomDocument>

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    explicit SettingsManager(QObject *parent = 0);
    void loadSettings();
    void parseXml(const QDomDocument& xml);
    bool createXml();
    QHash<QString, QVariant> deviceList(bool is_input);
    bool writeSettings();

    QString m_active_account;

    QString m_sip_port;

    struct audio_t
    {
        QString m_name;
        QString m_id;
    } m_inputDevice, m_outputDevice, m_ringingDevice;
    
signals:
    
public slots:
    
};

#endif // SETTINGSMANAGER_H
