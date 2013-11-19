#include "headers/settingsmanager.h"
#include "headers/controller.h"
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomElement>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QTextStream>
#include <QHash>
#include <QtGui/QGuiApplication>
#include <qaudiodeviceinfo.h>
#include <pjsua-lib/pjsua.h>

SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent)
{
    loadSettings();
}

void SettingsManager::loadSettings()
{
    qDebug() << "Loading settings...";

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
    QDomElement audiovideoNode = audioNode.elementsByTagName("videodevice").at(0).toElement();

    QDomElement accountNode = settingsNode.elementsByTagName("account").at(0).toElement();

    QDomElement sipNode = settingsNode.elementsByTagName("sip").at(0).toElement();

    m_inputDevice.m_id = audioInputNode.elementsByTagName("id").at(0).toElement().text();
    m_inputDevice.m_name = audioInputNode.elementsByTagName("name").at(0).toElement().text();

    m_outputDevice.m_id = audioOuputNode.elementsByTagName("id").at(0).toElement().text();
    m_outputDevice.m_name = audioOuputNode.elementsByTagName("name").at(0).toElement().text();

    m_videoDevice.m_id = audiovideoNode.elementsByTagName("id").at(0).toElement().text();
    m_videoDevice.m_name = audiovideoNode.elementsByTagName("name").at(0).toElement().text();

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
                    //video
                    xmlWriter->writeStartElement("videodevice");
                        xmlWriter->writeTextElement("id", m_videoDevice.m_id);
                        xmlWriter->writeTextElement("name",m_videoDevice.m_name);
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

bool SettingsManager::createLogFile()
{

    QFile logFile(QGuiApplication::applicationDirPath()+"/o2csip.log");

    if (!logFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qCritical() << "Read only" << " The file is in read only mode";
        return false;
    }

    logWriter.setDevice(&logFile);

    QDateTime cdate = QDateTime::currentDateTime();

    logWriter << "Starting log...\t\t\t\t" << cdate.toString("hh:mm dd/MM/yyyy") << "\n\n";

    return true;
}

#define THIS_FILE "settingsmanager.cpp"

static void vid_print_dev(int id, const pjmedia_vid_dev_info *vdi,
                          const char *title)
{
    char capnames[120];
    char formats[120];
    const char *dirname;
    unsigned i;

    if (vdi->dir == PJMEDIA_DIR_CAPTURE_RENDER) {
    dirname = "capture, render";
    } else if (vdi->dir == PJMEDIA_DIR_CAPTURE) {
    dirname = "capture";
    } else {
    dirname = "render";
    }

    pjmedia_vid_dev_count();

    capnames[0] = '\0';
    for (i=0; i<sizeof(int)*8 && (1 << i) < PJMEDIA_VID_DEV_CAP_MAX; ++i) {
    if (vdi->caps & (1 << i)) {
        const char *capname = pjmedia_vid_dev_cap_name((pjmedia_vid_dev_cap)(1 << i), NULL);
        if (capname) {
        if (*capnames)
            strcat(capnames, ", ");
        strncat(capnames, capname,
                sizeof(capnames)-strlen(capnames)-1);
        }
    }
    }

    formats[0] = '\0';
    for (i=0; i<vdi->fmt_cnt; ++i) {
    const pjmedia_video_format_info *vfi =
        pjmedia_get_video_format_info(NULL, vdi->fmt[i].id);
    if (vfi) {
        if (*formats)
        strcat(formats, ", ");
        strncat(formats, vfi->name, sizeof(formats)-strlen(formats)-1);
    }
    }

    qDebug() << id << vdi->name << vdi->driver << dirname << title;
    qDebug() << capnames;
    qDebug() <<  formats;
}

static void vid_list_devs(void)
{
    unsigned i, count;
    pjmedia_vid_dev_info vdi;
    pj_status_t status;

    qDebug() << "Video device list:";
    count = pjmedia_vid_dev_count();
    if (count == 0) {
    PJ_LOG(3,(THIS_FILE, " - no device detected -"));
    return;
    } else {
    qDebug() << count << " device(s) detected";
    }

    status = pjsua_vid_dev_get_info(PJMEDIA_VID_DEFAULT_RENDER_DEV, &vdi);
    if (status == PJ_SUCCESS)
    vid_print_dev(PJMEDIA_VID_DEFAULT_RENDER_DEV, &vdi,
                  "(default renderer device)");

    status = pjsua_vid_dev_get_info(PJMEDIA_VID_DEFAULT_CAPTURE_DEV, &vdi);
    if (status == PJ_SUCCESS)
    vid_print_dev(PJMEDIA_VID_DEFAULT_CAPTURE_DEV, &vdi,
                  "(default capture device)");

    for (i=0; i<count; ++i) {
    status = pjsua_vid_dev_get_info(i, &vdi);
    if (status == PJ_SUCCESS)
        vid_print_dev(i, &vdi, "");
    }
}

QList<QVariant> SettingsManager::videoDeviceList()
{
    QList<QVariant> deviceList;

    vid_list_devs();

    qDebug() << "Total of video devices: " << QString::number(pjmedia_vid_dev_count());

    unsigned count = 64;
    pjmedia_vid_dev_info vid_dev_info[64];
    pjsua_vid_enum_devs(vid_dev_info, &count);

    for (unsigned i=0;i<count;i++)
    {
        if (vid_dev_info[i].fmt_cnt && (vid_dev_info[i].dir==PJMEDIA_DIR_ENCODING || vid_dev_info[i].dir==PJMEDIA_DIR_ENCODING_DECODING))
        {
            deviceList.append(QString(vid_dev_info[i].name).append("::::").append(QString::number(vid_dev_info[i].id)));
            //qDebug() << "Device List Video :::::::::: " << "id: (" << QString::number(vid_dev_info[i].id) << ")" << QString(vid_dev_info[i].name);
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


