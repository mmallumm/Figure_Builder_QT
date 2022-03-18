#ifndef FIGURE_H
#define FIGURE_H
#include <QFrame>

const int delta = 40;
const int arrSize = delta / 4;
const int frSize = 600;

class Figure : public QFrame
{
    Q_OBJECT

public:
    explicit Figure(const QVector<QPoint> &v, QWidget *parent = nullptr);

    void paintFigure();

protected:
    void paintEvent(QPaintEvent*) override;

private:
    const QVector<QPoint> *pVector;

    void drawAxis(QChar axis, QPainter &painter, QPen &pen, QBrush &brush) const;
    int x(const int oldX) const;
    int y(const int oldY) const;
};

#endif // FIGURE_H
