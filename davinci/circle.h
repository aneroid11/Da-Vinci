#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape
{
public:
    Circle(float xc = 10, float yc = 10, float r = 5, QColor cl = QColor(255, 255, 255));
    ~Circle();
    void move(float dx, float dy);
    void rotate(float angle);
    void scale(float coef);
    void draw(QPainter* painter) const;
    void calculateCenter();
    void calculateRadiusOfAction();
    double getPerimeter() const;
    double getSquare() const;

private:
    float radius;
};

#endif // CIRCLE_H
