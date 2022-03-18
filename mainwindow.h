#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "app.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_addPushButton_clicked();


    void on_showPushButton_clicked();

private:
    App app{this};
    Ui::MainWindow *ui = nullptr;
    friend class App;
};

#endif // MAINWINDOW_H
