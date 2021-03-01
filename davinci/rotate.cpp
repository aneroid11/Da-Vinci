#include "rotate.h"

void rotatePoint(QPointF& point, float angle) {
    float newX, newY;
    newX = point.x() * cos(angle) - point.y() * sin(angle);
    newY = point.x() * sin(angle) + point.y() * cos(angle);
    point.setX(newX);
    point.setY(newY);
}
