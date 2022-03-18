#ifndef SHOWDIALOG_H
#define SHOWDIALOG_H

#include <QDialog>

class Figure;
class App;

namespace Ui {
class ShowDialog;
}

class ShowDialog : public QDialog
{
    Q_OBJECT

private:


public:
    explicit ShowDialog(App &app, QWidget *parent = nullptr);
    ~ShowDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ShowDialog *ui;
    App *app = nullptr;
    Figure *figure = nullptr;
    friend class App;

};

#endif // SHOWDIALOG_H
