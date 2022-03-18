#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "adddialog.h"
#include "ui_adddialog.h"
#include "showdialog.h"
#include "ui_showdialog.h"
#include "figure.h"
#include <QDebug>


//--------------------------------------------------------------------------------------//
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Figure Creator");

    ui->pointTextBrowser->setReadOnly(true);
    ui->addPushButton->setAutoFillBackground(true);

    QPalette palette = ui->addPushButton->palette();


    connect(ui->actionFileOpen, SIGNAL(triggered()), &app, SLOT(parseFile()));
    connect(ui->actionHelp, SIGNAL(triggered()), &app, SLOT(help()));
    connect(ui->actionFileSave, SIGNAL(triggered()), &app, SLOT(saveFile()));
    connect(ui->clearPushButton, SIGNAL(clicked()), &app, SLOT(clearCoords()));
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
MainWindow::~MainWindow()
{
    delete ui;
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void MainWindow::on_addPushButton_clicked()
{
    AddDialog addDialog(app);
    addDialog.setWindowTitle("Add");
    addDialog.setModal(true);
    addDialog.show();
    addDialog.exec();
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void MainWindow::on_showPushButton_clicked()
{
    ShowDialog showDialog(app);
    showDialog.setWindowTitle("Figure");
    showDialog.setModal(true);
    showDialog.show();
    showDialog.exec();
}
//--------------------------------------------------------------------------------------//



