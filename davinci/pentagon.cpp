#include "pentagon.h"
#include "rotate.h"
#include "distance.h"
#include "triangle.h"
#include "convexpolygon.h"

Pentagon::Pentagon(QPointF* verts, QColor cl) : Shape(0, 0, cl, "pentagon")
{
    for (int i = 0; i < 5; i++) {
        vertices[i] = verts[i];
        vertexIsMoving[i] = false;
    }

    calculateCenter();

    calculateRadiusOfAction();
}

Pentagon::~Pentagon() {}

void Pentagon::calculateRadiusOfAction() {
    float distances[5];

    for (int i = 0; i < 5; i++) {
        distances[i] = calculateDistance(QPointF(xCenter, yCenter), vertices[i]);
    }

    int iMax = 0;

    for (int i = 1; i < 5; i++) {
        if (distances[iMax] < distances[i]) {
            iMax = i;
        }
    }

    radiusOfAction = distances[iMax];
}

void Pentagon::calculateCenter() {
    QPointF tr1[3] = {vertices[0], vertices[1], vertices[4]};
    QPointF tr2[3] = {vertices[4], vertices[3], vertices[1]};
    QPointF tr3[3] = {vertices[1], vertices[3], vertices[2]};

    Triangle trig1(tr1);
    Triangle trig2(tr2);
    Triangle trig3(tr3);

    QPointF tr[3] = {trig1.getCenter(), trig2.getCenter(), trig3.getCenter()};

    Triangle trig(tr);

    xCenter = trig.getCenter().x();
    yCenter = trig.getCenter().y();
}

void Pentagon::move(float dx, float dy) {
    xCenter += dx;
    yCenter += dy;

    for (int i = 0; i < 5; i++) {
        vertices[i].setX(vertices[i].x() + dx);
        vertices[i].setY(vertices[i].y() + dy);
    }
}

void Pentagon::rotate(float angleRadians) {
    Q_UNUSED(angleRadians);

    QPointF relativeVertices[5];

    for (int i = 0; i < 5; i++) {
        relativeVertices[i] = vertices[i] - QPointF(xCenter, yCenter);
        rotatePoint(relativeVertices[i], angleRadians);
        vertices[i] = relativeVertices[i] + QPointF(xCenter, yCenter);
    }
}

void Pentagon::scale(float coef) {
    QPointF relativeVertices[5];

    for (int i = 0; i < 5; i++) {
        relativeVertices[i] = vertices[i] - QPointF(xCenter, yCenter);
        relativeVertices[i] *= coef;
        vertices[i] = relativeVertices[i] + QPointF(xCenter, yCenter);
    }

    calculateRadiusOfAction();
}

void Pentagon::moveVertices(const QPointF &curPos) {
    for (int i = 0; i < 5; i++) {
        if (vertexIsMoving[i]) {
            QPointF oldVertPos = vertices[i];
            QPointF newVertPos = curPos;
            vertices[i] = newVertPos;

            if (!isConvex(vertices, 5)) {
                vertices[i] = oldVertPos;
            }
        }
    }

    calculateCenter();
    calculateRadiusOfAction();
}

void Pentagon::setVerticesMoving(const QPointF &cursor) {
    // Двигаться должна только одна вершина.
    bool someVertIsMoving = false;

    for (int i = 0; i < 5; i++) {
        if (vertexIsMoving[i]) { someVertIsMoving = true; }
    }

    for (int i = 0; i < 5; i++) {
        if (!someVertIsMoving && calculateDistance(cursor, vertices[i]) < 10.0) {
            vertexIsMoving[i] = true;
        }
    }
}

void Pentagon::resetVerticesMoving() {
    for (int i = 0; i < 5; i++) {
        vertexIsMoving[i] = false;
    }
}

void Pentagon::draw(QPainter* painter) const {
    if (clicked) {
        painter->setPen(QColor(255, 255, 255));
    } else {
        painter->setPen(color);
    }

    painter->setBrush(color);
    painter->drawPolygon(vertices, 5);

    if (showCenter) {
        painter->setPen(QColor(255, 255, 255));
        painter->setBrush(QColor(255, 255, 255));
        painter->drawEllipse(QPointF(xCenter, yCenter), 3, 3);
    }
}

double Pentagon::getPerimeter() const {
    double perim = 0.0;

    for (int i = 0; i < 5; i++) {
        if (i == 4) {
            perim += calculateDistance(vertices[0], vertices[i]);
        } else {
            perim += calculateDistance(vertices[i], vertices[i + 1]);
        }
    }

    return perim;
}

double Pentagon::getSquare() const {
    QPointF tr1[3] = {vertices[0], vertices[1], vertices[4]};
    QPointF tr2[3] = {vertices[4], vertices[3], vertices[1]};
    QPointF tr3[3] = {vertices[1], vertices[3], vertices[2]};

    Triangle trig1(tr1);
    Triangle trig2(tr2);
    Triangle trig3(tr3);

    double square = trig1.getSquare() + trig2.getSquare() + trig3.getSquare();

    return square;
}

bool Pentagon::isCursorInside(const QPointF& cursPos) const {
    QPointF tr1[3] = {vertices[0], vertices[1], cursPos};
    QPointF tr2[3] = {vertices[1], vertices[2], cursPos};
    QPointF tr3[3] = {vertices[2], vertices[3], cursPos};
    QPointF tr4[3] = {vertices[3], vertices[4], cursPos};
    QPointF tr5[3] = {vertices[4], vertices[0], cursPos};

    Triangle trig1(tr1),
             trig2(tr2),
             trig3(tr3),
             trig4(tr4),
             trig5(tr5);

    double trigSquare = trig1.getSquare() + trig2.getSquare() + trig3.getSquare() + trig4.getSquare() + trig5.getSquare();
    double actualSquare = getSquare();
    bool inside = abs((int)actualSquare - (int)trigSquare) <= 5;

    return inside;
}
