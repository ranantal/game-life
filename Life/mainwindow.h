#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "settingswindow.h"
#include "drawpanelwidget.h"

namespace Ui {
class MainWindow;
}

//class SettingsWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
   // static MainWindow * getInstance();
    void updateSettings();

private slots:
    void on_actionNew_triggered();

    void on_actionAbout_triggered();

    void on_actionProperties_triggered();

   // void on_actionReplace_triggered();

    void on_actionReplace_triggered(bool checked);

   // void on_actionXOR_destroyed();

    void on_actionXOR_triggered(bool checked);

    void on_actionImpact_triggered(bool checked);

   // void on_actionToolbar_toggled(bool arg1);

    void on_actionToolbar_triggered(bool checked);

    void on_actionStatus_Bar_triggered(bool checked);

    void on_actionNext_triggered();

    void on_actionClear_triggered();

    void on_actionRun_triggered(bool checked);

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

private:

   // static MainWindow *instance;
    Ui::MainWindow *ui;

   // DrawPanelWidget *drawPanelWidget;
   // SettingsWindow *settingsWindow;
};

#endif // MAINWINDOW_H
