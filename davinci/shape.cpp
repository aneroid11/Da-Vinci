#include "shape.h"
#include "distance.h"

#include <QCursor>
#include <QMainWindow>
#include <cmath>

Shape::Shape(float xc, float yc, QColor cl, QString n)
{
    xCenter = xc;
    yCenter = yc;
    color = cl;
    name = n;
    clicked = true;
    showCenter = false;
}

Shape::~Shape() {}

QPointF Shape::getCenter() const {
    return QPointF(xCenter, yCenter);
}

QString Shape::getName() const {
    return name;
}

bool Shape::isClicked() const {
    return clicked;
}

void Shape::setClicked(bool cl) {
    clicked = cl;
}

bool Shape::isCursorInside(const QPointF& cursPos) const {
    float distance = calculateDistance(cursPos, QPointF(xCenter, yCenter));
    return distance < radiusOfAction;
}

void Shape::setShowCenter(bool s) {
    showCenter = s;
}
