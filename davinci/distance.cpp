#include "distance.h"

float calculateDistance(const QPointF& p1, const QPointF& p2) {
    QPointF delta = p1 - p2;
    float distance = sqrt(delta.x() * delta.x() + delta.y() * delta.y());
    return distance;
}
