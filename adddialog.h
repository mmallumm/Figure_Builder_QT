#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

class App;

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(App &app, QWidget *parent = nullptr);
    ~AddDialog();

signals:
    void dialogAccepted();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddDialog *ui;
    App *app = nullptr;
    friend class App;
};



#endif // ADDDIALOG_H
