#include "mmsettings.h"
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include "mmconversioninfo.h"

MMSettings* MMSettings::_instance = nullptr;

MMSettings::MMSettings()
{
    _mmConfFile =  QDir().homePath() + "/.mmconvert.ini";
}

QString MMSettings::getDirToMonitor() const
{
    return _dirToMonitor;
}

void MMSettings::setDirToMonitor(const QString &dirToMonitor)
{
    _dirToMonitor = dirToMonitor;
}



bool MMSettings::getAutoProcessDir() const
{
    return _autoProcessDir;
}

void MMSettings::setAutoProcessDir(bool autoProcessDir)
{
    _autoProcessDir = autoProcessDir;
}

QString MMSettings::getOutPath() const
{
    return _outPath;
}

void MMSettings::setOutPath(const QString &outPath)
{
    _outPath = outPath;
}

int MMSettings::getAutocustomTrigger() const
{
    return _autocustomTrigger;
}

void MMSettings::setAutocustomTrigger(int autocustomTrigger)
{
    _autocustomTrigger = autocustomTrigger;
}

QString MMSettings::getPreset() const
{
    return _preset;
}

void MMSettings::setPreset(const QString &preset)
{
    _preset = preset;
}

bool MMSettings::getStripSubs() const
{
    return _stripSubs;
}

void MMSettings::setStripSubs(bool stripSubs)
{
    _stripSubs = stripSubs;
}

bool MMSettings::getDelete() const
{
    return _delete;
}

void MMSettings::setDelete(bool deleteV)
{
    _delete = deleteV;
}

int MMSettings::getCrf() const
{
    return _crf;
}

void MMSettings::setCrf(int crf)
{
    _crf = crf;
}

bool MMSettings::saveSettings()
{
    QString  _mmConfFile =  QDir().homePath() + "/.mmconvert.ini";
    QSettings settings(_mmConfFile, QSettings::IniFormat);
    settings.beginGroup("MMCONVERT-DB");
    settings.setValue("DBServer", _server);
    settings.setValue("DBUserID", _user);
    settings.setValue("DBPassword", _pwd);
    settings.setValue("DataBase", _dataBase);
    settings.endGroup();

    settings.beginGroup("MMCONVERT-DEFAULTS");
    settings.setValue("CRF", _crf);
    settings.setValue("Delete", _delete);
    settings.setValue("StripSubs", _stripSubs);
    settings.setValue("Preset", _preset);
    settings.setValue("Trigger", _autocustomTrigger);
    settings.setValue("OutPath", _outPath);
    settings.setValue("AutoProcessDir", _autoProcessDir);
    settings.setValue("DirToMonitor", _dirToMonitor);
    settings.endGroup();

    return true;

}

bool MMSettings::settingsExists()
{
    return QFileInfo(_mmConfFile).exists();
}

bool MMSettings::loadSettings()
{


    if(settingsExists() == false)
    {
        saveSettings();
        return false;
    }

    QSettings settings(_mmConfFile, QSettings::IniFormat);
    if(settings.status() == QSettings::NoError)
    {
        settings.beginGroup("MMCONVERT-DB");
        _server = settings.value("DBServer","").toString();
        _user = settings.value("DBUserID","").toString();
        _pwd = settings.value("DBPassword","").toString();
        _dataBase = settings.value("DataBase","").toString();
        settings.endGroup();

        settings.beginGroup("MMCONVERT-DEFAULTS");
        _crf = settings.value("CRF").toInt();
        _delete = settings.value("Delete").toBool();
        _stripSubs = settings.value("StripSubs").toBool();
        _preset = settings.value("Preset").toString();
        _autocustomTrigger = settings.value("Trigger").toInt();
        _outPath = settings.value("OutPath").toString();
        _autoProcessDir = settings.value("AutoProcessDir").toBool();
        _dirToMonitor = settings.value("DirToMonitor").toString();
        settings.endGroup();
        return true;
    }

    return false;
}

QString MMSettings::getPwd() const
{
    return _pwd;
}

void MMSettings::setPwd(const QString &pwd)
{
    _pwd = pwd;
}

QString MMSettings::getUser() const
{
    return _user;
}

void MMSettings::setUser(const QString &user)
{
    _user = user;
}

QString MMSettings::getDataBase() const
{
    return _dataBase;
}

void MMSettings::setDataBase(const QString &dataBase)
{
    _dataBase = dataBase;
}

QString MMSettings::getServer() const
{
    return _server;
}

void MMSettings::setServer(const QString &server)
{
    _server = server;
}

MMSettings* MMSettings::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new MMSettings();
    }

    return _instance;
}
