#ifndef PENTAGON_H
#define PENTAGON_H

#include "shape.h"

class Pentagon : public Shape
{
public:
    Pentagon(QPointF* verts, QColor color = QColor(255, 255, 255));
    ~Pentagon();
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
    QPointF vertices[5];
    bool vertexIsMoving[5];
};

#endif // PENTAGON_H
