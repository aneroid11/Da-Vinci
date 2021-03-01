#include "circle.h"

Circle::Circle(float xc, float yc, float r, QColor cl) : Shape(xc, yc, cl, "circle")
{
    radius = r;
    calculateRadiusOfAction();
}

Circle::~Circle() {}

void Circle::calculateRadiusOfAction() {
    radiusOfAction = radius;
}

void Circle::move(float dx, float dy) {
    xCenter += dx;
    yCenter += dy;
}

void Circle::rotate(float angleRadians) {
    Q_UNUSED(angleRadians);
}

void Circle::scale(float coef) {
    radius *= coef;

    calculateRadiusOfAction();
}

void Circle::draw(QPainter* painter) const {
    if (clicked) {
        painter->setPen(QColor(255, 255, 255));
    } else {
        painter->setPen(color);
    }
    painter->setBrush(color);
    painter->drawEllipse(QPointF(xCenter, yCenter), radius, radius);

    if (showCenter) {
        painter->setPen(QColor(255, 255, 255));
        painter->setBrush(QColor(255, 255, 255));
        painter->drawEllipse(QPointF(xCenter, yCenter), 3, 3);
    }
}

void Circle::calculateCenter() {}

double Circle::getPerimeter() const {
    return 2 * 3.14 * radius;
}

double Circle::getSquare() const {
    return 3.14 * radius * radius;
}
