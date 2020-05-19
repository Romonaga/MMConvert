#include "dialogmmsettings.h"
#include "ui_dialogmmsettings.h"
#include "mmsettings.h"
#include "mmconversioninfo.h"
#include <QFileDialog>

DialogMMSettings::DialogMMSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMMSettings)
{
    ui->setupUi(this);
    if(MMSettings::getInstance()->settingsExists() == true)
    {
        ui->txtServer->setText(MMSettings::getInstance()->getServer());
        ui->txtDataBase->setText(MMSettings::getInstance()->getDataBase());
        ui->txtUser->setText(MMSettings::getInstance()->getUser());
        ui->txtPwd->setText(MMSettings::getInstance()->getPwd());
        ui->spinCrf->setValue(MMSettings::getInstance()->getCrf());
        ui->cboxDelete->setChecked(MMSettings::getInstance()->getDelete());
        ui->cboxStrip->setChecked(MMSettings::getInstance()->getStripSubs());
        ui->spinAutoCustom->setValue(MMSettings::getInstance()->getAutocustomTrigger());

        ui->cboxPreset->insertItems(0, MMConversionInfo::getPresets());
        ui->cboxPreset->setCurrentIndex(ui->cboxPreset->findText(MMSettings::getInstance()->getPreset()));
        ui->btnSelPath->setText(MMSettings::getInstance()->getOutPath());
        ui->cbAutoProcess->setChecked(MMSettings::getInstance()->getAutoProcessDir());
        ui->btnDirToMonitor->setText(MMSettings::getInstance()->getDirToMonitor());

    }

}

DialogMMSettings::~DialogMMSettings()
{
    delete ui;
}

void DialogMMSettings::on_btnSave_clicked()
{
    MMSettings::getInstance()->setServer(ui->txtServer->text());
    MMSettings::getInstance()->setDataBase(ui->txtDataBase->text());
    MMSettings::getInstance()->setUser(ui->txtUser->text());
    MMSettings::getInstance()->setPwd(ui->txtPwd->text());
    MMSettings::getInstance()->setPreset(ui->cboxPreset->currentText());

    MMSettings::getInstance()->saveSettings();

    this->setResult(QDialog::Accepted);
    this->close();
}

void DialogMMSettings::on_cboxDelete_clicked(bool checked)
{
    MMSettings::getInstance()->setDelete(checked);
}

void DialogMMSettings::on_cboxStrip_clicked(bool checked)
{
    MMSettings::getInstance()->setStripSubs(checked);
}


void DialogMMSettings::on_spinAutoCustom_valueChanged(int arg1)
{
    MMSettings::getInstance()->setAutocustomTrigger(arg1);
}

void DialogMMSettings::on_spinCrf_valueChanged(int arg1)
{
    MMSettings::getInstance()->setCrf(arg1);
}

void DialogMMSettings::on_btnSelPath_clicked()
{
    QFileDialog* fileDialog = new QFileDialog(this);

    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setOption(QFileDialog::ShowDirsOnly, true);
    MMSettings::getInstance()->setOutPath(fileDialog->getExistingDirectory(this));
    ui->btnSelPath->setText(MMSettings::getInstance()->getOutPath());

}

void DialogMMSettings::on_cbAutoProcess_clicked(bool checked)
{
    MMSettings::getInstance()->setAutoProcessDir(checked);
}

void DialogMMSettings::on_btnDirToMonitor_clicked()
{
    QFileDialog* fileDialog = new QFileDialog(this);

    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setOption(QFileDialog::ShowDirsOnly, true);
    MMSettings::getInstance()->setDirToMonitor(fileDialog->getExistingDirectory(this));
    ui->btnDirToMonitor->setText(MMSettings::getInstance()->getDirToMonitor());

}

