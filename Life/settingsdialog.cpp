#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <iostream>

#include "settings.h"
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->liveBeginSpinBox->setValue((double)(Settings::liveBegin) / 10);
    ui->birthBeginSpinBox->setValue((double)(Settings::birthBegin) / 10);
    ui->birthEndSpinBox->setValue((double)(Settings::birthEnd) / 10);
    ui->liveEndSpinBox->setValue((double)(Settings::liveEnd) / 10);

    ui->firstImpactSpinBox->setValue((double)Settings::firstImpact / 10);
    ui->secondImpactSpinBox->setValue((double)Settings::secondImpact / 10);

    ui->impactCheckBox->setChecked(Settings::impact);
    ui->replaceRadioButton->setChecked(Settings::replaceMode);
    ui->speedSlider->setValue(Settings::speed);
    ui->speedSpinBox->setValue(Settings::speed);
    ui->xorRadioButton->setChecked(Settings::xorMode);
    ui->xSizeSlider->setValue(Settings::xSize);
    ui->xSizeSpinBox->setValue(Settings::xSize);
    ui->ySizeSlider->setValue(Settings::ySize);
    ui->ySizeSpinBox->setValue(Settings::ySize);
    ui->cellSizeSlider->setValue(Settings::cellSize);
    ui->cellSizeSpinBox->setValue(Settings::cellSize);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{

    if (
            ui->liveBeginSpinBox->value() > ui->birthBeginSpinBox->value() ||
            ui->birthBeginSpinBox->value() > ui->birthEndSpinBox->value() ||
            ui->birthEndSpinBox->value() > ui->liveEndSpinBox->value()
            ) {
        QMessageBox::information(0, "ERROR", "Must be\nLIVE_BEGIN <= BIRTH_BEGIN <= BIRTH_END <= LIVE_END");
        return;
    }
    Settings::birthBegin = 10 * ui->birthBeginSpinBox->value();
    Settings::birthEnd = 10 * ui->birthEndSpinBox->value();
    Settings::firstImpact = 10 * ui->firstImpactSpinBox->value();
    Settings::impact = ui->impactCheckBox->isChecked();
    Settings::liveBegin = 10 * ui->liveBeginSpinBox->value();
    Settings::liveEnd = 10 * ui->liveEndSpinBox->value();
    Settings::replaceMode = ui->replaceRadioButton->isChecked();
    Settings::secondImpact = 10 * ui->secondImpactSpinBox->value();
    Settings::speed = ui->speedSlider->value();
    Settings::xorMode = ui->xorRadioButton->isChecked();
    Settings::xSize = ui->xSizeSlider->value();
    Settings::ySize = ui->ySizeSlider->value();
    Settings::cellSize = ui->cellSizeSlider->value();

    this->accept();
}

void SettingsDialog::on_buttonBox_rejected()
{

}
