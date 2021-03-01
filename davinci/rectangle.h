#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QPointF>

#include "shape.h"

class Rectangle : public Shape
{
public:
    Rectangle(float xc = 10, float yc = 10, float w = 0, float h = 0, QColor cl = QColor(255, 255, 255));
    ~Rectangle();
    void move(float dx, float dy);
    void rotate(float angle);
    void scale(float coef);
    void draw(QPainter* painter) const;
    void calculateCenter();
    void calculateRadiusOfAction();
    double getPerimeter() const;
    double getSquare() const;
    bool isCursorInside(const QPointF& cursPos) const;

private:
    QPointF vertices[4];
    bool widthChanging;
    bool heightChanging;
};

#endif // RECTANGLE_H
