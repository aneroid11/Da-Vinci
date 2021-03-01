#include "triangle.h"
#include "rotate.h"
#include "distance.h"

#include <math.h>

Triangle::Triangle(QPointF* verts, QColor cl) : Shape(0, 0, cl, "triangle")
{
    for (int i = 0; i < 3; i++) {
        vertices[i] = verts[i];
        vertexIsMoving[i] = false;
    }

    calculateCenter();

    calculateRadiusOfAction();
}

Triangle::~Triangle() {}

void Triangle::calculateRadiusOfAction() {
    float distances[3];

    for (int i = 0; i < 3; i++) {
        distances[i] = calculateDistance(QPointF(xCenter, yCenter), vertices[i]);
    }

    int iMax = 0;

    for (int i = 1; i < 3; i++) {
        if (distances[iMax] < distances[i]) {
            iMax = i;
        }
    }

    radiusOfAction = distances[iMax];
}

void Triangle::calculateCenter() {
    QPointF a = vertices[0];
    QPointF b = vertices[1] + (vertices[2] - vertices[1]) / 2;

    QPointF center = a + (b - a) * (2.0/3.0);
    xCenter = center.x();
    yCenter = center.y();
}

void Triangle::move(float dx, float dy) {
    xCenter += dx;
    yCenter += dy;

    for (int i = 0; i < 3; i++) {
        vertices[i].setX(vertices[i].x() + dx);
        vertices[i].setY(vertices[i].y() + dy);
    }
}

void Triangle::rotate(float angleRadians) {
    Q_UNUSED(angleRadians);

    QPointF relativeVertices[3];

    for (int i = 0; i < 3; i++) {
        relativeVertices[i] = vertices[i] - QPointF(xCenter, yCenter);
        rotatePoint(relativeVertices[i], angleRadians);
        vertices[i] = relativeVertices[i] + QPointF(xCenter, yCenter);
    }
}

void Triangle::scale(float coef) {
    QPointF relativeVertices[3];

    for (int i = 0; i < 3; i++) {
        relativeVertices[i] = vertices[i] - QPointF(xCenter, yCenter);
        relativeVertices[i] *= coef;
        vertices[i] = relativeVertices[i] + QPointF(xCenter, yCenter);
    }

    calculateRadiusOfAction();
}

void Triangle::moveVertices(const QPointF &curPos) {
    for (int i = 0; i < 3; i++) {
        if (vertexIsMoving[i]) {
            QPointF newVertPos = curPos;
            vertices[i] = newVertPos;
        }
    }

    calculateCenter();
    calculateRadiusOfAction();
}

void Triangle::setVerticesMoving(const QPointF &cursor) {
    // Двигаться должна только одна вершина.
    bool someVertIsMoving = false;

    for (int i = 0; i < 3; i++) {
        if (vertexIsMoving[i]) { someVertIsMoving = true; }
    }

    for (int i = 0; i < 3; i++) {
        if (!someVertIsMoving && calculateDistance(cursor, vertices[i]) < 10.0) {
            vertexIsMoving[i] = true;
        }
    }
}

void Triangle::resetVerticesMoving() {
    for (int i = 0; i < 3; i++) {
        vertexIsMoving[i] = false;
    }
}

void Triangle::draw(QPainter* painter) const {
    if (clicked) {
        painter->setPen(QColor(255, 255, 255));
    } else {
        painter->setPen(color);
    }

    painter->setBrush(color);
    painter->drawPolygon(vertices, 3);

    if (showCenter) {
        painter->setPen(QColor(255, 255, 255));
        painter->setBrush(QColor(255, 255, 255));
        painter->drawEllipse(QPointF(xCenter, yCenter), 3, 3);
    }
}

double Triangle::getPerimeter() const {
    double perim = 0.0;

    for (int i = 0; i < 3; i++) {
        if (i == 2) {
            perim += calculateDistance(vertices[0], vertices[i]);
        } else {
            perim += calculateDistance(vertices[i], vertices[i + 1]);
        }
    }

    return perim;
}

double Triangle::getSquare() const {
    double p = getPerimeter() / 2.0;
    double a = calculateDistance(vertices[0], vertices[1]);
    double b = calculateDistance(vertices[1], vertices[2]);
    double c = calculateDistance(vertices[0], vertices[2]);
    double square = sqrt(p * (p - a) * (p - b) * (p - c));

    return square;
}

bool Triangle::isCursorInside(const QPointF& cursPos) const {
    QPointF tr1[3] = {vertices[0], vertices[1], cursPos};
    QPointF tr2[3] = {vertices[1], vertices[2], cursPos};
    QPointF tr3[3] = {vertices[2], vertices[0], cursPos};

    Triangle trig1(tr1),
             trig2(tr2),
             trig3(tr3);

    double trigSquare = trig1.getSquare() + trig2.getSquare() + trig3.getSquare();
    double actualSquare = getSquare();
    bool inside = abs((int)actualSquare - (int)trigSquare) <= 5;

    return inside;
}
