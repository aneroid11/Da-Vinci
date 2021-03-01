#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include <QMainWindow>
#include <QPainter>
#include <QVector>
#include <QMenu>
#include <QMenuBar>
#include <QMouseEvent>

#include "shape.h"
#include "rectangle.h"
#include "circle.h"
#include "triangle.h"
#include "pentagon.h"
#include "hexagon.h"

class Paint : public QMainWindow
{
    Q_OBJECT

public:
    Paint(QWidget *parent = nullptr);
    ~Paint();
    void paintEvent(QPaintEvent* event);
    void timerEvent(QTimerEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void clearScreen(QPainter* painter, QColor color);
    void update();
    void showMenu();
    void fixCursorCoordinates();

private slots:
    void createTriangle();
    void createRectangle();
    void createPentagon();
    void createCircle();
    void createHexagon();
    void showContextMenu(const QPoint& p);
    void goToScalingMode();
    void goToMovingMode();
    void goToMovingVerticesMode();
    void goToRotatingMode();
    void tellPerimeter();
    void tellSquare();
    void showCenters();

private:
    int screenWidth, screenHeight;
    int timer;
    QVector<Shape*> figures;
    QPointF curPos;
    enum {MOVING, SCALING, ROTATING, MOVING_VERTICES} mode;
};
#endif // PAINT_H
