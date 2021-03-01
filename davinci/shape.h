#ifndef SHAPE_H
#define SHAPE_H

#include <QPainter>
#include <QColor>

class Shape
{
public:
    Shape(float xc = 10, float yc = 10, QColor cl = QColor(255, 255, 255), QString n = "null");
    virtual ~Shape();
    virtual void move(float dx, float dy) = 0;
    virtual void rotate(float angleRadians) = 0;
    virtual void scale(float coef) = 0;
    virtual void draw(QPainter* painter) const = 0;
    virtual void calculateRadiusOfAction() = 0;
    virtual void calculateCenter() = 0;
    virtual double getPerimeter() const = 0;
    virtual double getSquare() const = 0;
    QPointF getCenter() const;
    QString getName() const;
    bool isClicked() const;
    void setClicked(bool cl);
    virtual bool isCursorInside(const QPointF& cursPos) const;
    void setShowCenter(bool s = false);
    void setShowVertices(bool s = false);

protected:
    QString name;
    float xCenter, yCenter;
    float square, perimeter;
    bool clicked;
    float radiusOfAction;
    bool showCenter;
    QColor color;
};

#endif // SHAPE_H
