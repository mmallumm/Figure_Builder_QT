#include "figure.h"
#include "app.h"
#include <QRect>
#include <QPainter>



//--------------------------------------------------------------------------------------//
Figure::Figure(const QVector<QPoint> &v, QWidget *parent)
    : QFrame{parent}
{
    //Настрока размеров объекта
    setFixedSize(600 + delta, 600 + delta);
    setFrameRect(QRect(delta, 0, frSize, frSize));
    setLineWidth(2);
    setFrameStyle(1);

    pVector = &v;
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//

void Figure::paintEvent(QPaintEvent *)
{
    QPainter painter;

    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::GlobalColor::blue);

    QBrush brush;
    brush.setColor(Qt::GlobalColor::black);
    brush.setStyle(Qt::SolidPattern);

    //Отрисовка осей
    painter.begin(this);
    drawAxis('x', painter, pen, brush);
    drawAxis('y', painter, pen, brush);
    painter.end();

    //Отрисовка фигуры
    painter.begin(this);
    painter.drawText(x(0) - 25, y(0) + 20, "(0, 0)");
    painter.setPen(pen);
    drawFrame(&painter);
    for (int i = 0; i < pVector->size() - 1; ++i) {
        painter.drawLine( x(pVector->at(i).x()),
                          y(pVector->at(i).y()),
                          x(pVector->at(i+1).x()),
                          y(pVector->at(i+1).y()) );
    }
    painter.end();
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
void Figure::drawAxis(QChar axis, QPainter &painter, QPen &pen, QBrush &brush) const
{
    switch(axis.unicode())
    {
    default:
        break;

    case 'x':
        {
            painter.setBrush(brush);
            painter.drawLine(8*delta, height() - delta/2, 10*delta, height() - delta/2);
            QPolygon pol;
            pol << QPoint( 10*delta, (height() - delta/2 + arrSize/2) )
                << QPoint( 10*delta, (height() - delta/2 - arrSize/2) )
                << QPoint( 11*delta, height() - delta/2);
            painter.drawPolygon(pol);
            painter.drawText(8*delta - 20, height() - delta/2 + arrSize/5, "0X");
            break;
        }


    case 'y':
        {
            painter.setBrush(brush);
            painter.drawLine(delta/2, 8*delta, delta/2, 10*delta);
            QPolygon pol;
            pol << QPoint( delta/2 - arrSize/2, 8*delta )
                << QPoint( delta/2 + arrSize/2, 8*delta )
                << QPoint( delta/2, 7*delta);
            painter.drawPolygon(pol);
            painter.drawText(delta/2 - delta/4, 10*delta + 20, "0Y");
            break;
        }
    }
};
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
int Figure::x(const int oldX) const
{
    int result = 0;
    result = oldX + delta;
    return result;
}
//--------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------//
int Figure::y(const int oldY) const
{
    int result = 0;
    result = (-1) * oldY + height() - delta;
    return result;
}
//--------------------------------------------------------------------------------------//
