#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTextEdit>
#include "app.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "adddialog.h"
#include "ui_adddialog.h"


//--------------------------------------------------------------------------------------//
App::App(MainWindow* w) : master(w) {
    pointVector.reserve(20);
};
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void App::parseFile()
{
    //Получение пути к файлу
    QFileDialog fileDialog;
    QString pointFilePath;
    pointFilePath = fileDialog.getOpenFileName();

    //Открытие файла для чтения
    QFile pointFile(pointFilePath);
    pointFile.open(QIODevice::ReadOnly);

    if (pointFile.isOpen()) {
        //Очистка виджета и контейнера
        clearCoords();

        //Чтение файла, обработка данных
        QString current = "";
        while(!pointFile.atEnd()) {
            current = pointFile.readLine();
            if (!lineIsCorrect(current)) {
                master->ui->pointTextBrowser->insertPlainText("Файл не удалось корректно открыть. Пожалуйста, проверьте соблюдение требований оформления файла.");
                return;
            }
            if (!lineIsEmpty(current)) {
                uint x = 0;
                uint y = 0;
                getCoords(x, y, current);
                pointVector.push_back(QPoint((int)x, (int)y));
            }
        }

        //Анализ точек
        analyzePoints(pointVector);

        //Обновление виджета
        insertCoords(pointVector);
    }
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void App::saveFile()
{
    //Получение пути к файлу
    QFileDialog fileDialog;
    QString pointFilePath;
    pointFilePath = fileDialog.getSaveFileName();

    //Открытие файла для записи
    QFile pointFile(pointFilePath);
    pointFile.open(QIODevice::WriteOnly);

    //Поэлементная запись точек в файл
    for (auto &el : pointVector) {
        auto currentX = QString::number(el.x());
        auto currentY = QString::number(el.y());
        auto currentSting = currentX + ", " + currentY + "\n";
        pointFile.write(currentSting.toUtf8());
    }
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void App::addCoords()
{
    //Чтение текста из textEdit в поток QTestStream
    QString str = "";
    QString current = "";
    QTextStream text;
    str = qobject_cast<AddDialog*>(sender())->ui->textEdit->toPlainText();
    text.setString(&str);

    //Обработка текста в потоке
    while(!text.atEnd()) {
        current = text.readLine();
        if (!lineIsCorrect(current)) {
            QMessageBox message;
            message.setWindowTitle("Exception");
            message.setModal(true);
            message.setText("Координаты не удалось корректно прочитать. Пожалуйста, проверьте соблюдение требований оформления.");
            message.exec();
            return;
        }
        if (!lineIsEmpty(current)) {
            uint x = 0;
            uint y = 0;
            getCoords(x, y, current);
            pointVector.push_back(QPoint((int)x, (int)y));
            analyzePoints(pointVector);
            insertCoords(pointVector.last());
        }
    }
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void App::clearCoords()
{
    //Чистка виджетов и контейнера
    master->ui->pointTextBrowser->clear();
    pointVector.clear();
    master->ui->countPointLabel->setText(QString::number(0));
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void App::help()
{
    //Окно помощи
    QMessageBox help;
    help.setWindowTitle("Help");
    help.setModal(true);
    help.setText("   Для отрисовки фигуры по координатам необходимо ввести их, нажав на кнопку Add, или открыть файл формата txt с записаннами в него координатами."
                 "\n\n   Обратите внимание, что каждая координата должна быть введена с новой строки в строгом формате \"X, Y\""
                 ", при чем координаты X и Y должны быть целочислены и находиться в диапазоне [0, 600] для корректного отображения на плоскости.");
    help.show();
    help.exec();
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
bool App::lineIsCorrect(const QString &str) const
{
    //Счетчик значищих символов
    uint charNumberCount = 0;
    //Счетчик символов ','
    uint charCommaCount = 0;

    for (auto &ch : str) {
        //Проверка корректности символов
        if (!charIsCorrect(ch)) { return false; }

        //Обновление счечика значищих символов
        if (charIsNumber(ch)) { ++charNumberCount; }

        //Обновление счетчика символов ','
        if (ch == ',') { ++charCommaCount; }
    }

    //Пустая строка считается корректной
    if (lineIsEmpty(str)) { return true; }
    //Проверка количества символов ','
    if (charCommaCount != 1) { return false; }
    //Проверка количества символов цифр
    if (charNumberCount < 2) { return false; }

    //0-е приближение для начала первой координаты в строке
    uint jointStartX = 0;
    //0-е приближение для конца первой координаты в строке
    uint jointEndX = 0;

    //Определение jointStartX, jointEndX
    setupJointsX(str, jointStartX, jointEndX);

    //Определение ',' в строке
    uint jointComma = jointEndX + 1;


    //Проверка наличия второй координаты в строке
    bool flagY = false;
    for (uint i = jointComma + 1; i < str.size(); ++i) {
        if (charIsNumber(str.at(i))) { flagY = true; }
    }
    if (flagY == false) { return false; }


    //0-е приближение для начала второй координаты в строке
    uint jointStartY = 0;
    //0-е приближение для конца второй координаты в строке
    uint jointEndY = 0;

    //Определение jointStartY, jointEndY
    setupJointsY(str, jointStartY, jointEndY, jointComma);


    //Проверка корректности символов первой координаты
    for (uint i = jointStartX; i <= jointEndX; ++i) {
        if (!charIsNumber(str.at(i))) { return false; }
    }

    //Проверка корректности символов второй координаты
    for (uint i = jointStartY; i <= jointEndY; ++i) {
        if (!charIsNumber(str.at(i))) { return false; }
    }

    //Проверка корректности символов после второй координаты
    if (jointEndY + 1 != str.size()) {
        for (uint i = jointEndY + 1; i < str.size(); ++i) {
            if (charIsNumber(str.at(i))) { return false; }
        }
    }

    return true;
};
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
bool App::lineIsEmpty(const QString &str) const
{
    //Проверка строки на вырожденность
    for (auto &ch : str) {
        if (charIsNumber(ch)) { return false; }
    }
    return true;
};
//--------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------//
bool App::charIsCorrect(const QChar &ch) const
{
    //Проверка коррестности символа
    if ( charIsNumber(ch) ||
         ch == '\r' ||
         ch == '\n' ||
         ch == '\t' ||
         ch == ','  ||
         ch == ' '
       )
    {
        return true;
    }

    else { return false; }
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
bool App::charIsNumber(const QChar &ch) const
{
    //Проверка символа на нахождение цифры в нём
    if (ch.unicode() >= '0' && ch.unicode() <= '9') {return true;}
    else {return false;}
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void App::getCoords(uint& outX, uint& outY, const QString &str) const
{
    //0-е приближение для начала первой координаты в строке
    uint jointStartX = 0;
    //0-е приближение для конца первой координаты в строке
    uint jointEndX = 0;

    //Определение jointStartX, jointEndX
    setupJointsX(str, jointStartX, jointEndX);

    //Определение ',' в строке
    uint jointComma = jointEndX + 1;

    //0-е приближение для начала второй координаты в строке
    uint jointStartY = 0;
    //0-е приближение для конца второй координаты в строке
    uint jointEndY = 0;

    //Определение jointStartY, jointEndY
    setupJointsY(str, jointStartY, jointEndY, jointComma);

    //Обработка координат
    outX = QStringToUint(str.mid(jointStartX, jointEndX - jointStartX + 1));
    outY = QStringToUint(str.mid(jointStartY, jointEndY - jointStartY + 1));
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void App::setupJointsX(const QString &str, uint& outJointStartX, uint& outJointEndX) const
{
    //0-е приближение для начала первой координаты в строке
    outJointStartX = 0;

    //Поиск начала первой координаты в строке
    for (uint i = 0; !charIsNumber(str.at(i)); ++i) {
        ++outJointStartX;
    }

    //0-е приближение для конца первой координаты в строке
    outJointEndX = outJointStartX;

    //Поиск конца первой координаты в строке
    for (uint i = outJointEndX + 1; str.at(i) != ','; ++i) {
        ++outJointEndX;
    }
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void App::setupJointsY(const QString &str, uint& outJointStartY, uint& outJointEndY, const uint& jointComma) const
{
    //0-е приближение для начала второй координаты в строке
    outJointStartY = jointComma + 1;

    //Поиск начала второй координаты в строке
    for (uint i = outJointStartY; !charIsNumber(str.at(i)); ++i) {
        ++outJointStartY;
    }

    //0-е приближение для конца второй координаты в строке
    outJointEndY = outJointStartY;

    //Поиск конца второй координаты в строке
    for (uint i = outJointEndY + 1; (i < str.size() && charIsNumber(str.at(i))); ++i) {
        ++outJointEndY;
    }
};
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void App::insertCoords(const QVector<QPoint>& vec) const
{
    //Обновление pointTextBrowser
    uint counter = 0;
    for (auto& el : vec) {
        master->ui->pointTextBrowser->insertPlainText( "(" +
                                                       QString::number(el.x()) +
                                                       ", " +
                                                       QString::number(el.y()) +
                                                       ")"
                                                      );
        if ( counter != vec.size() - 1 ) {
            master->ui->pointTextBrowser->insertPlainText(", ");
            ++counter;
        }
    }

    //Обновление countPointLabel
    master->ui->countPointLabel->setText(QString::number(vec.size()));
};
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void App::insertCoords(const QPoint& point) const
{
    //Обновление pointTextBrowser
    if(pointVector.size() != 1){ master->ui->pointTextBrowser->insertPlainText(", "); }
    master->ui->pointTextBrowser->insertPlainText( "(" +
                                                   QString::number(point.x()) +
                                                   ", " +
                                                   QString::number(point.y()) +
                                                   ")"
                                                  );


    //Обновление countPointLabel
    auto counter = QStringToUint(master->ui->countPointLabel->text());
    ++counter;
    master->ui->countPointLabel->setText(QString::number(counter));
};
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void App::analyzePoints(QVector<QPoint>& vec)
{
    if (vec.size() > 2) {
        for (int i = 1; i < vec.size() - 1; ++i) {
            int rightPart = (vec[i].y() - vec[i-1].y()) * (vec[i+1].x() - vec[i-1].x());
            int leftPart = (vec[i].x() - vec[i-1].x()) * (vec[i+1].y() - vec[i-1].y());

            if ( leftPart == rightPart ) {
                vec.removeAt(i);
                analyzePoints(vec);
            }
        }
    }
};
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
uint App::QStringToUint (const QString &str) const
{
    //Конвертация строки в число типа uint
    uint result = 0;
    uint degree = 0;
    for (uint i = str.size() - 1; (int)i >= 0; --i) {
        if (charIsNumber(str.at(i))) {
            degree = (str.size() - 1) - i;
            result += QCharToUint(str.at(i)) * uint_pow(10, degree);
        }
    }
    return result;
};
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
uint App::QCharToUint(const QChar &ch) const
{
    //Конвертация символа в число типа uint
    uint out = (uint)ch.unicode() - (uint)'0';
    return out;
};
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
uint App::uint_pow(const uint value, const uint degree) const
{
    //Возведение в степень для uint
    uint result = value;
    if (degree == 0) { result = 1; }
    for (uint i = 1; i < degree; ++i) { result *= value; }
    return result;
}
//--------------------------------------------------------------------------------------//


