#include "rectangle.h"
#include "triangle.h"
#include "rotate.h"
#include "distance.h"

Rectangle::Rectangle(float xc, float yc, float w, float h, QColor cl) : Shape(xc, yc, cl, "rectangle") {
    vertices[0] = QPointF(-w/2, -h/2);
    vertices[1] = QPointF(w/2, -h/2);
    vertices[2] = QPointF(w/2, h/2);
    vertices[3] = QPointF(-w/2, h/2);

    widthChanging = heightChanging = false;

    calculateRadiusOfAction();
}

Rectangle::~Rectangle() {}

void Rectangle::calculateRadiusOfAction() {
    radiusOfAction = calculateDistance(QPointF(xCenter, yCenter), vertices[0] + QPointF(xCenter, yCenter));
}

void Rectangle::move(float dx, float dy) {
    xCenter += dx;
    yCenter += dy;
}

void Rectangle::rotate(float angleRadians) {
    for (int i = 0; i < 4; i++) {
        rotatePoint(vertices[i], angleRadians);
    }
}

void Rectangle::scale(float coef) {
    for (int i = 0; i < 4; i++) {
        vertices[i] *= coef;
    }

    calculateRadiusOfAction();
}

void Rectangle::draw(QPainter* painter) const {
    if (clicked) {
        painter->setPen(QColor(255, 255, 255));
    } else {
        painter->setPen(color);
    }

    painter->setBrush(color);

    QPointF absoluteVerts[4];

    for (int i = 0; i < 4; i++) {
        absoluteVerts[i] = QPointF(xCenter, yCenter) + vertices[i];
    }

    painter->drawPolygon(absoluteVerts, 4);

    if (showCenter) {
        painter->setPen(QColor(255, 255, 255));
        painter->setBrush(QColor(255, 255, 255));
        painter->drawEllipse(QPointF(xCenter, yCenter), 3, 3);
    }
}

void Rectangle::calculateCenter() {}

double Rectangle::getPerimeter() const {
    double width = calculateDistance(vertices[0], vertices[1]);
    double height = calculateDistance(vertices[1], vertices[2]);

    return (width + height) * 2;
}

double Rectangle::getSquare() const {
    double width = calculateDistance(vertices[0], vertices[1]);
    double height = calculateDistance(vertices[1], vertices[2]);

    return width * height;
}

bool Rectangle::isCursorInside(const QPointF& cursPos) const {
    // Берём cursPos (X) и все точки прямоугольника (A, B, C, D). Составляем треугольники ABX, BCX, CDX, ADX.
    // Если сумма их площадей равна площади прямоугольника - точка внутри прямоугольника.

    QPointF relativePos = cursPos - QPointF(xCenter, yCenter);

    QPointF tr1[3] = {vertices[0], vertices[1], relativePos};
    QPointF tr2[3] = {vertices[1], vertices[2], relativePos};
    QPointF tr3[3] = {vertices[2], vertices[3], relativePos};
    QPointF tr4[3] = {vertices[3], vertices[0], relativePos};

    Triangle trig1(tr1),
             trig2(tr2),
             trig3(tr3),
             trig4(tr4);

    double trigSquare = trig1.getSquare() + trig2.getSquare() + trig3.getSquare() + trig4.getSquare();
    double actualSquare = getSquare();
    bool inside = abs((int)actualSquare - (int)trigSquare) <= 5;

    return inside;
}
