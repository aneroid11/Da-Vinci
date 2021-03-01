#ifndef HEXAGON_H
#define HEXAGON_H

#include "shape.h"

class Hexagon : public Shape
{
public:
    Hexagon(QPointF* verts, QColor color = QColor(255, 255, 255));
    ~Hexagon();
    void calculateCenter();
    void move(float dx, float dy);
    void rotate(float angle);
    void scale(float coef);
    void draw(QPainter* painter) const;
    void calculateRadiusOfAction();
    double getPerimeter() const;
    double getSquare() const;
    bool isCursorInside(const QPointF& cursPos) const;
    void moveVertices(const QPointF& curPos);
    void setVerticesMoving(const QPointF& cursor);
    void resetVerticesMoving();

private:
    QPointF vertices[6];
    bool vertexIsMoving[6];
};

#endif // HEXAGON_H
