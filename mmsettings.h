#ifndef MMSETTINGS_H
#define MMSETTINGS_H

#include <QString>



class MMSettings
{

public:
    static MMSettings* getInstance();

    bool loadSettings();
    bool saveSettings();
    bool settingsExists();

    QString getServer() const;
    void setServer(const QString &server);

    QString getDataBase() const;
    void setDataBase(const QString &dataBase);

    QString getUser() const;
    void setUser(const QString &user);

    QString getPwd() const;
    void setPwd(const QString &pwd);


    int getCrf() const;
    void setCrf(int crf);

    bool getDelete() const;
    void setDelete(bool deleteV);

    bool getStripSubs() const;
    void setStripSubs(bool stripSubs);

    QString getPreset() const;
    void setPreset(const QString &preset);

    int getAutocustomTrigger() const;
    void setAutocustomTrigger(int autocustomTrigger);

    QString getOutPath() const;
    void setOutPath(const QString &outPath);

    bool getAutoProcessDir() const;
    void setAutoProcessDir(bool autoProcessDir);


    QString getDirToMonitor() const;
    void setDirToMonitor(const QString &dirToMonitor);

private:
    MMSettings();
    static MMSettings* _instance;
    QString _server;
    QString _dataBase;
    QString _user;
    QString _pwd;
    QString  _mmConfFile;
    int _crf;
    bool _delete;
    bool _stripSubs;
    QString _preset;
    int _autocustomTrigger;
    QString _outPath;
    bool _autoProcessDir;
    QString _dirToMonitor;







};

#endif // MMSETTINGS_H
