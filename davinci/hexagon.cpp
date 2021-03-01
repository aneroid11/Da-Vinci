#include "hexagon.h"
#include "rotate.h"
#include "distance.h"
#include "triangle.h"
#include "convexpolygon.h"

Hexagon::Hexagon(QPointF* verts, QColor cl) : Shape(0, 0, cl, "hexagon")
{
    for (int i = 0; i < 6; i++) {
        vertices[i] = verts[i];
        vertexIsMoving[i] = false;
    }

    calculateCenter();

    calculateRadiusOfAction();
}

Hexagon::~Hexagon() {}

void Hexagon::calculateRadiusOfAction() {
    float distances[6];

    for (int i = 0; i < 6; i++) {
        distances[i] = calculateDistance(QPointF(xCenter, yCenter), vertices[i]);
    }

    int iMax = 0;

    for (int i = 1; i < 6; i++) {
        if (distances[iMax] < distances[i]) {
            iMax = i;
        }
    }

    radiusOfAction = distances[iMax];
}

void Hexagon::calculateCenter() {
    QPointF tr1[3] = {vertices[0], vertices[1], vertices[5]};
    QPointF tr2[3] = {vertices[1], vertices[2], vertices[5]};
    QPointF tr3[3] = {vertices[2], vertices[3], vertices[5]};
    QPointF tr4[3] = {vertices[3], vertices[4], vertices[5]};

    Triangle trig1(tr1);
    Triangle trig2(tr2);
    Triangle trig3(tr3);
    Triangle trig4(tr4);

    QPointF triangle1[3] = {trig1.getCenter(), trig2.getCenter(), trig4.getCenter()};
    QPointF triangle2[3] = {trig2.getCenter(), trig3.getCenter(), trig4.getCenter()};

    Triangle t1(triangle1);
    Triangle t2(triangle2);

    QPointF center = t1.getCenter() + (t2.getCenter() - t1.getCenter()) / 2;

    xCenter = center.x();
    yCenter = center.y();
}

void Hexagon::move(float dx, float dy) {
    xCenter += dx;
    yCenter += dy;

    for (int i = 0; i < 6; i++) {
        vertices[i].setX(vertices[i].x() + dx);
        vertices[i].setY(vertices[i].y() + dy);
    }
}

void Hexagon::rotate(float angleRadians) {
    Q_UNUSED(angleRadians);

    QPointF relativeVertices[6];

    for (int i = 0; i < 6; i++) {
        relativeVertices[i] = vertices[i] - QPointF(xCenter, yCenter);
        rotatePoint(relativeVertices[i], angleRadians);
        vertices[i] = relativeVertices[i] + QPointF(xCenter, yCenter);
    }
}

void Hexagon::scale(float coef) {
    QPointF relativeVertices[6];

    for (int i = 0; i < 6; i++) {
        relativeVertices[i] = vertices[i] - QPointF(xCenter, yCenter);
        relativeVertices[i] *= coef;
        vertices[i] = relativeVertices[i] + QPointF(xCenter, yCenter);
    }

    calculateRadiusOfAction();
}

void Hexagon::moveVertices(const QPointF &curPos) {
    for (int i = 0; i < 6; i++) {
        if (vertexIsMoving[i]) {
            QPointF newVertPos = curPos;
            QPointF oldVertPos = vertices[i];
            vertices[i] = newVertPos;

            if (!isConvex(vertices, 6)) {
                vertices[i] = oldVertPos;
            }
        }
    }

    calculateCenter();
    calculateRadiusOfAction();
}

void Hexagon::setVerticesMoving(const QPointF &cursor) {
    // Двигаться должна только одна вершина.
    bool someVertIsMoving = false;

    for (int i = 0; i < 6; i++) {
        if (vertexIsMoving[i]) { someVertIsMoving = true; }
    }

    for (int i = 0; i < 6; i++) {
        if (!someVertIsMoving && calculateDistance(cursor, vertices[i]) < 10.0) {
            vertexIsMoving[i] = true;
        }
    }
}

void Hexagon::resetVerticesMoving() {
    for (int i = 0; i < 6; i++) {
        vertexIsMoving[i] = false;
    }
}

void Hexagon::draw(QPainter* painter) const {
    if (clicked) {
        painter->setPen(QColor(255, 255, 255));
    } else {
        painter->setPen(color);
    }

    painter->setBrush(color);
    painter->drawPolygon(vertices, 6);

    if (showCenter) {
        painter->setPen(QColor(255, 255, 255));
        painter->setBrush(QColor(255, 255, 255));
        painter->drawEllipse(QPointF(xCenter, yCenter), 2, 2);
    }
}

double Hexagon::getPerimeter() const {
    double perim = 0.0;

    for (int i = 0; i < 6; i++) {
        if (i == 5) {
            perim += calculateDistance(vertices[0], vertices[i]);
        } else {
            perim += calculateDistance(vertices[i], vertices[i + 1]);
        }
    }

    return perim;
}

double Hexagon::getSquare() const {
    QPointF tr1[3] = {vertices[0], vertices[1], vertices[5]};
    QPointF tr2[3] = {vertices[1], vertices[2], vertices[5]};
    QPointF tr3[3] = {vertices[2], vertices[3], vertices[5]};
    QPointF tr4[3] = {vertices[3], vertices[4], vertices[5]};

    Triangle trig1(tr1);
    Triangle trig2(tr2);
    Triangle trig3(tr3);
    Triangle trig4(tr4);

    double square = trig1.getSquare() + trig2.getSquare() + trig3.getSquare() + trig4.getSquare();

    return square;
}

bool Hexagon::isCursorInside(const QPointF& cursPos) const {
    QPointF tr1[3] = {vertices[0], vertices[1], cursPos};
    QPointF tr2[3] = {vertices[1], vertices[2], cursPos};
    QPointF tr3[3] = {vertices[2], vertices[3], cursPos};
    QPointF tr4[3] = {vertices[3], vertices[4], cursPos};
    QPointF tr5[3] = {vertices[4], vertices[5], cursPos};
    QPointF tr6[3] = {vertices[5], vertices[0], cursPos};

    Triangle trig1(tr1),
             trig2(tr2),
             trig3(tr3),
             trig4(tr4),
             trig5(tr5),
             trig6(tr6);

    double trigSquare = trig1.getSquare() + trig2.getSquare() + trig3.getSquare() +
                        trig4.getSquare() + trig5.getSquare() + trig6.getSquare();
    double actualSquare = getSquare();
    bool inside = abs((int)actualSquare - (int)trigSquare) <= 5;

    return inside;
}
