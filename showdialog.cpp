#include "showdialog.h"
#include "ui_showdialog.h"
#include "app.h"
#include "figure.h"

//--------------------------------------------------------------------------------------//
ShowDialog::ShowDialog(App &app, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowDialog)
{
    this->app = &app;

    figure = new Figure(this->app->pointVector, this);

    ui->setupUi(this);

    ui->verticalLayout->addSpacing(40);

    ui->verticalLayout->addWidget(figure);

    ui->verticalLayout->setDirection(QBoxLayout::BottomToTop);

    setFixedSize(700, 720);
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
ShowDialog::~ShowDialog()
{
    ui->verticalLayout->removeWidget(figure);
    delete figure;

    delete ui;
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void ShowDialog::on_pushButton_clicked()
{
    close();
}
//--------------------------------------------------------------------------------------//

