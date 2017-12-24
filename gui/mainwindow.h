#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QSettings>
#include <QProcess>
#include "models/xbetablemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:


    void on_actionExit_triggered();

    void on_actionGo_To_The_Official_CxBx_Reloaded_Website_triggered();

    void on_actionEmulationStart_triggered();

    void on_actionOpenXbe_triggered();

    void on_actionAbout_triggered();

    void on_actionEmulation_triggered();

    void on_actionEmulationStop_triggered();

    void on_gameTableView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QTableView *gameTableView;
    QSettings *settings;
    QProcess emulatorProcess;

    void runGame(const QString filePath);
};

#endif // MAINWINDOW_H
