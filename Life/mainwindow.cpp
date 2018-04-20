#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settings.h"
#include "settingsdialog.h"

#include <QtWidgets>
#include <string>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionReplace->setChecked(Settings::replaceMode);
    ui->actionXOR->setChecked(Settings::xorMode);
    ui->actionImpact->setChecked(Settings::impact);

    ui->actionToolbar->setChecked(true);
    ui->actionStatus_Bar->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(0, "About", "Game Life by Nikita Bychkov, group 14202\n\n2017 (c)");
}

void MainWindow::on_actionProperties_triggered()
{
    SettingsDialog dialog;
    dialog.exec();

    ui->actionReplace->setChecked(Settings::replaceMode);
    ui->actionXOR->setChecked(Settings::xorMode);

    if (Settings::cellSize < 15) {
        ui->actionImpact->setEnabled(false);
        ui->actionImpact->setChecked(false);
        Settings::impact = false;
    } else {
        ui->actionImpact->setEnabled(true);
        ui->actionImpact->setChecked(Settings::impact);
    }
    ui->drawPanelWidget->resizeImage();

   // ui->drawPanelWidget->model->printField();
}

void MainWindow::on_actionReplace_triggered(bool checked)
{
    Settings::replaceMode = checked;
    Settings::xorMode = !checked;

    ui->actionXOR->setChecked(!checked);
}

void MainWindow::on_actionXOR_triggered(bool checked)
{
    Settings::xorMode = checked;
    Settings::replaceMode = !checked;

    ui->actionReplace->setChecked(!checked);
}

void MainWindow::on_actionImpact_triggered(bool checked)
{
    Settings::impact = checked;
    ui->drawPanelWidget->repaint();
}

void MainWindow::on_actionToolbar_triggered(bool checked)
{
    ui->toolBar->setVisible(checked);
}

void MainWindow::on_actionStatus_Bar_triggered(bool checked)
{
    ui->statusbar->setVisible(checked);
}

void MainWindow::on_actionNext_triggered()
{
    ui->drawPanelWidget->nextStep();
}

void MainWindow::on_actionClear_triggered()
{
    ui->drawPanelWidget->clear();
}

void MainWindow::on_actionRun_triggered(bool checked)
{
    if (checked) {
        ui->actionAbout->setEnabled(false);
        ui->actionClear->setEnabled(false);
       // ui->actionImpact->setEnabled(false);
        ui->actionNew->setEnabled(false);
        ui->actionNext->setEnabled(false);
        ui->actionOpen->setEnabled(false);
        ui->actionProperties->setEnabled(false);
       // ui->actionReplace->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->actionSave_As->setEnabled(false);
       // ui->actionXOR->setEnabled(false);

        ui->drawPanelWidget->runGame();
    }

    else {
        ui->actionAbout->setEnabled(true);
        ui->actionClear->setEnabled(true);
       // ui->actionImpact->setEnabled(true);
        ui->actionNew->setEnabled(true);
        ui->actionNext->setEnabled(true);
        ui->actionOpen->setEnabled(true);
        ui->actionProperties->setEnabled(true);
       // ui->actionReplace->setEnabled(true);
        ui->actionSave->setEnabled(true);
        ui->actionSave_As->setEnabled(true);
       // ui->actionXOR->setEnabled(true);

        ui->drawPanelWidget->stopGame();
    }
}

void MainWindow::on_actionNew_triggered()
{
    if (!ui->drawPanelWidget->checkModel()) {
        QMessageBox::StandardButton reply;
        QString str("Save changes to ");
        str += ui->drawPanelWidget->actualFileName;
        str += '?';
        reply = QMessageBox::question(this, QString::fromUtf8("Save"),
                              str,
                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes)
            on_actionSave_triggered();
        else if (reply == QMessageBox::Cancel)
            return;
    }

    on_actionProperties_triggered();
    ui->drawPanelWidget->clear();
    ui->drawPanelWidget->actualFileName = "Untitled";
}

void MainWindow::on_actionOpen_triggered()
{
    if (!ui->drawPanelWidget->checkModel()) {
        QMessageBox::StandardButton reply;
        QString str("Save changes to ");
        str += ui->drawPanelWidget->actualFileName;
        str += '?';
        reply = QMessageBox::question(this, QString::fromUtf8("Save"),
                              str,
                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes)
            on_actionSave_triggered();
        else if (reply == QMessageBox::Cancel)
            return;
    }

    QString fileName = QFileDialog::getOpenFileName(this);
    if (fileName.isEmpty())
        return;

    try {
        ui->drawPanelWidget->openModel(fileName);
    } catch (std::string str) {
        QMessageBox::information(0, "Error", str.c_str());
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (ui->drawPanelWidget->actualFileName == "Untitled")
        on_actionSave_As_triggered();
    else {
        if (!ui->drawPanelWidget->saveModel(ui->drawPanelWidget->actualFileName) ) {
            QMessageBox::information(0, "Error", "Can't open this file");
        }
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return;

    if (!ui->drawPanelWidget->saveModel(ui->drawPanelWidget->actualFileName) ) {
        QMessageBox::information(0, "Error", "Can't open this file");
    }
}
