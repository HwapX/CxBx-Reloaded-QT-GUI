#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings_emulation.h"
#include "ui_settings_emulation.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString appData = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

    this->settings = new QSettings(appData + "/cxbx-reloaded/conf.ini", QSettings::IniFormat);

    this->gameTableView = this->findChild<QTableView*>("gameTableView");

    this->gameTableView->setModel(new XbeTableModel(this->settings->value("game_dir").toString()));
    this->gameTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    this->gameTableView->setColumnWidth(0, 110);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->settings;
}

//Simple exit instruction
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

//Load the CxBx-Reloaded website/Github url in the default browser
void MainWindow::on_actionGo_To_The_Official_CxBx_Reloaded_Website_triggered()
{
    QString link = "https://github.com/Cxbx-Reloaded/Cxbx-Reloaded";
    QDesktopServices::openUrl(QUrl(link));
}

//Test code to run the CxBx binary with a game as an argument, should be removed when implementing the button real funtion
void MainWindow::on_actionEmulationStart_triggered()
{
    QItemSelectionModel *selectionModel = this->gameTableView->selectionModel();
    XbeTableModel *model = dynamic_cast<XbeTableModel*>(this->gameTableView->model());

    if(!selectionModel->hasSelection())
        return;

    int row = selectionModel->selectedIndexes().at(0).row();

    this->runGame(model->getXbe(row)->m_szPath);
}

//Open a dialog window to load an .xbe and run on the emulator
void MainWindow::on_actionOpenXbe_triggered()
{
    //Obtain the .xbe path
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open"),"",tr("*.xbe"));

   //Run the .xbe as an argument for the emulator if the argument isn't empty
    if (fileName == ""){
        return;
    }

    QString emuPath = this->settings->value("cxbx_path").toString();

    QProcess *myProcess = new QProcess(qApp);
    myProcess->start(emuPath, QStringList() << QDir::toNativeSeparators(fileName)); //QDir::toNativeSeparators makes sure the path obtained by QFileDialog::getOpenFileName is valid as a native path
}


//Load the About window
void MainWindow::on_actionAbout_triggered()
{

}

//Load the Emulation settings window
void MainWindow::on_actionEmulation_triggered()
{

}

void MainWindow::on_actionEmulationStop_triggered()
{
    if(this->emulatorProcess.state() == QProcess::Running)
        this->emulatorProcess.terminate();
}

void MainWindow::on_gameTableView_doubleClicked(const QModelIndex &index)
{
    XbeTableModel *model = dynamic_cast<XbeTableModel*>(this->gameTableView->model());

    this->runGame(model->getXbe(index.row())->m_szPath);
}

void MainWindow::runGame(const QString filePath)
{
    QString emuPath = this->settings->value("cxbx_path").toString();

    this->emulatorProcess.start(emuPath, QStringList() << filePath);
}
