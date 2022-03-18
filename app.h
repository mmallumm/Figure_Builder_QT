#ifndef APP_H
#define APP_H

#include <QVector>
#include <QString>
#include <QFile>

class MainWindow;

class App : public QObject
{
    Q_OBJECT

public:
    explicit App(MainWindow* w);
    ~App() = default;



public slots:
    void parseFile();
    void saveFile();
    void addCoords();
    void clearCoords();
    void help();

private:
    MainWindow* master = nullptr;
    QVector<QPoint> pointVector;

    void getCoords(uint& outX, uint& outY, const QString &str) const;
    void setupJointsX(const QString &str, uint& outJointStartX, uint& outJointEndX) const;
    void setupJointsY(const QString &str, uint& outJointStartY, uint& outJointEndY, const uint& JointComma) const;
    void insertCoords(const QVector<QPoint>& vec) const;
    void insertCoords(const QPoint& point) const;
    void analyzePoints(QVector<QPoint>& vec);
    bool lineIsCorrect(const QString &str) const;
    bool lineIsEmpty(const QString &str) const;
    bool charIsNumber(const QChar &ch) const;
    bool charIsCorrect(const QChar &ch) const;
    uint QStringToUint (const QString &str) const;
    uint QCharToUint(const QChar &ch) const;
    uint uint_pow(const uint value, const uint degree) const;

    friend class ShowDialog;
};



#endif // APP_H
