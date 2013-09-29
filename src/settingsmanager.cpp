#include "headers/settingsmanager.h"
#include "headers/controller.h"
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomElement>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QHash>
#include <QtGui/QGuiApplication>
#include <qaudiodeviceinfo.h>

SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent)
{
    loadSettings();
}

void SettingsManager::loadSettings()
{
    qDebug() << "Loading settings...";

    unsigned count = 64;
    pjmedia_aud_dev_info aud_dev_info[64];
    pjsua_enum_aud_devs(aud_dev_info, &count);
    for (unsigned i=0;i<count;i++)
    {
        if(aud_dev_info[i].output_count>0)
        {
            qDebug() << "Debug audio SPK: " << aud_dev_info[i].name;
        }
        else
        {
            qDebug() << "Debug audio MIC: " << aud_dev_info[i].name;
        }

    }

    QFile xmlFile(QGuiApplication::applicationDirPath()+"/settings.xml");
    QDomDocument xmlDoc;

    if(xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Loading settings file [DONE]";
        xmlDoc.setContent(&xmlFile);

        parseXml(xmlDoc);
    }
    else
    {
        qCritical() << "[ERROR] Loading settings file [FAIL]" << xmlFile.errorString() << " ";
        createXml();
    }
}

void SettingsManager::parseXml(const QDomDocument& xmlDoc)
{
    qDebug() << "Parsing XML settings...";

    QDomElement settingsNode = xmlDoc.elementsByTagName("settings").at(0).toElement();
    QDomElement audioNode = settingsNode.elementsByTagName("audio").at(0).toElement();

    QDomElement audioInputNode = audioNode.elementsByTagName("inputdevice").at(0).toElement();
    QDomElement audioOuputNode = audioNode.elementsByTagName("outputdevice").at(0).toElement();
    QDomElement audioRingingNode = audioNode.elementsByTagName("ringingdevice").at(0).toElement();

    QDomElement accountNode = settingsNode.elementsByTagName("account").at(0).toElement();

    QDomElement sipNode = settingsNode.elementsByTagName("sip").at(0).toElement();

    m_inputDevice.m_id = audioInputNode.elementsByTagName("id").at(0).toElement().text();
    m_inputDevice.m_name = audioInputNode.elementsByTagName("name").at(0).toElement().text();

    m_outputDevice.m_id = audioOuputNode.elementsByTagName("id").at(0).toElement().text();
    m_outputDevice.m_name = audioOuputNode.elementsByTagName("name").at(0).toElement().text();

    m_ringingDevice.m_id = audioRingingNode.elementsByTagName("id").at(0).toElement().text();
    m_ringingDevice.m_name = audioRingingNode.elementsByTagName("name").at(0).toElement().text();

    m_active_account = accountNode.elementsByTagName("active_id").at(0).toElement().text();

    m_sip_port = sipNode.elementsByTagName("sip_port").at(0).toElement().text();

    qDebug() << "Parsing XML settings... [DONE]";

    deviceList(true);

    return;
}

bool SettingsManager::createXml()
{
    qDebug() << "Creating XML Settings file...";
    QFile xmlFile(QGuiApplication::applicationDirPath()+"/settings.xml");

    if (!xmlFile.open(QIODevice::WriteOnly))
    {
        qCritical() << "Read only" << " The file is in read only mode";
        return false;
    }
    else
    {
        try
        {
            QAudioDeviceInfo audioDevices;

            QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();

            xmlWriter->setDevice(&xmlFile);

            xmlWriter->writeStartDocument();
            //settings
            xmlWriter->writeStartElement("settings");
                //audio
                xmlWriter->writeStartElement("audio");
                    //output
                    xmlWriter->writeStartElement("outputdevice");
                        xmlWriter->writeTextElement("id", m_outputDevice.m_id);
                        xmlWriter->writeTextElement("name",m_outputDevice.m_name);
                    xmlWriter->writeEndElement();
                    //output end
                    //input
                    xmlWriter->writeStartElement("inputdevice");
                        xmlWriter->writeTextElement("id", m_inputDevice.m_id);
                        xmlWriter->writeTextElement("name",m_inputDevice.m_name);
                    xmlWriter->writeEndElement();
                    //input end
                    //ringing
                    xmlWriter->writeStartElement("ringingdevice");
                        xmlWriter->writeTextElement("id", m_ringingDevice.m_id);
                        xmlWriter->writeTextElement("name",m_ringingDevice.m_name);
                    xmlWriter->writeEndElement();
                xmlWriter->writeEndElement();
                //audio end
                //account
                xmlWriter->writeStartElement("account");
                    xmlWriter->writeTextElement("active_id", m_active_account);
                xmlWriter->writeEndElement();
                //audio end
                //sip
                xmlWriter->writeStartElement("sip");
                    xmlWriter->writeTextElement("sip_port", m_sip_port);
                xmlWriter->writeEndElement();
                //sip end
            xmlWriter->writeEndElement();
            //document end
            xmlWriter->writeEndDocument();

            delete xmlWriter;
            return true;
        }
        catch (std::exception ex)
        {
            qDebug() << "error xml create " << ex.what();
            return false;
        }
    }
}

bool SettingsManager::writeSettings()
{
    return createXml();
}

QHash<QString, QVariant> SettingsManager::videoDeviceList()
{
    QHash<QString, QVariant> deviceList;

    qDebug() << "Total of video devices: " << QString::number(pjsua_vid_dev_count());

    unsigned count = 64;
    pjmedia_vid_dev_info vid_dev_info[64];
    pjsua_vid_enum_devs(vid_dev_info, &count);

    for (unsigned i=0;i<count;i++)
    {
        if (vid_dev_info[i].fmt_cnt && (vid_dev_info[i].dir==PJMEDIA_DIR_ENCODING || vid_dev_info[i].dir==PJMEDIA_DIR_ENCODING_DECODING))
        {
            deviceList.insert(QString::number(vid_dev_info[i].id), QString(vid_dev_info[i].name));
            qDebug() << "Device List Video :::::::::: " << "id: (" << QString::number(vid_dev_info[i].id) << ")" << QString(vid_dev_info[i].name);
        }
    }

    return deviceList;
}

QHash<QString, QVariant> SettingsManager::deviceList(bool is_input)
{
    QHash<QString, QVariant> deviceList;
    QAudio::Mode mode;

    if(is_input)
    {
        mode = QAudio::AudioInput;
    }
    else
    {
        mode = QAudio::AudioOutput;
    }

    deviceList.clear();

    foreach (const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(mode))
    {
        deviceList.insert(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
    }

    return deviceList;
}


