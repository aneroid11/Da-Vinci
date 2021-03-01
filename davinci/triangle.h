#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"

class Triangle : public Shape
{
public:
    Triangle(QPointF* verts, QColor color = QColor(255, 255, 255));
    ~Triangle();
    void calculateCenter();
    void move(float dx, float dy);
    void rotate(float angle);
    void scale(float coef);
    void moveVertices(const QPointF& curPos);
    void setVerticesMoving(const QPointF& cursor);
    void resetVerticesMoving();
    void draw(QPainter* painter) const;
    void calculateRadiusOfAction();
    double getPerimeter() const;
    double getSquare() const;
    bool isCursorInside(const QPointF& cursPos) const;

private:
    QPointF vertices[3];
    bool vertexIsMoving[3];
};

#endif // TRIANGLE_H
