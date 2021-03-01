#include "paint.h"
#include "distance.h"

#include <QMessageBox>

Paint::Paint(QWidget *parent)
    : QMainWindow(parent)
{    
    screenWidth = 640;
    screenHeight = 480;
    resize(screenWidth, screenHeight);

    srand(time(nullptr));

    timer = startTimer(10);
    goToMovingMode();

    showMenu();

    setMouseTracking(true);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &Paint::showContextMenu);
}

Paint::~Paint()
{
    for (int i = 0; i < figures.size(); i++) {
        delete figures[i];
    }
}

void Paint::createRectangle() {
    QPointF center = QCursor::pos() - geometry().topLeft();

    Shape* fig = new Rectangle(center.x(), center.y(), rand() % 100, rand() % 100, QColor(rand() % 256, rand()%256, rand()%256));
    figures.push_back(fig);
}

void Paint::createTriangle() {
    QPointF center = QCursor::pos() - geometry().topLeft();
    QPointF verts[3];

    verts[0] = QPointF(center.x(), center.y() - rand() % 50);
    verts[1] = QPointF(center.x() + 30, center.y() + 20);
    verts[2] = QPointF(center.x() - 30, center.y() + 20);

    Shape* fig = new Triangle(verts, QColor(rand()%256, rand()%256, rand()%256));
    fig->calculateCenter();

    fig->move(center.x() - fig->getCenter().x(), center.y() - fig->getCenter().y());
    figures.push_back(fig);
}

void Paint::createCircle() {
    Shape* fig = new Circle(0, 0, rand() % 100, QColor(rand()%256, rand()%256, rand()%256));
    figures.push_back(fig);
}

void Paint::createPentagon() {
    QPointF verts[5];
    verts[0] = QPointF(0, 0);
    verts[1] = QPointF(verts[0].x() + 30, verts[0].y() + 20);
    verts[2] = QPointF(verts[1].x(), verts[0].y() + 30);
    verts[3] = QPointF(verts[2].x() - 30, verts[2].y() + 20);
    verts[4] = QPointF(verts[3].x() - 30, verts[3].y() - 30);

    Shape* fig = new Pentagon(verts, QColor(rand()%256, rand()%256, rand()%256));
    figures.push_back(fig);
}

void Paint::createHexagon() {
    QPointF verts[6];
    verts[0] = QPointF(0, 0);
    verts[1] = QPointF(verts[0].x() + 30, verts[0].y());
    verts[2] = QPointF(verts[0].x() + 50, verts[0].y() + 20);
    verts[3] = QPointF(verts[0].x() + 30, verts[0].y() + 50);
    verts[4] = QPointF(verts[0].x(), verts[0].y() + 50);
    verts[5] = QPointF(verts[0].x() - 20, verts[0].y() + 20);

    Shape* fig = new Hexagon(verts, QColor(rand()%256, rand()%256, rand()%256));
    figures.push_back(fig);
}

void Paint::showMenu() {
    QAction* actions[6];
    actions[0] = new QAction("Треугольник", nullptr);
    actions[1] = new QAction("Прямоугольник", nullptr);
    actions[2] = new QAction("Круг", nullptr);
    actions[3] = new QAction("Выпуклый пятиугольник", nullptr);
    actions[4] = new QAction("Выпуклый шестиугольник", nullptr);

    QMenu* menu = menuBar()->addMenu("Создать фигуру");

    for (int i = 0; i < 5; i++) {
        menu->addAction(actions[i]);
    }

    connect(actions[0], &QAction::triggered, this, &Paint::createTriangle);
    connect(actions[1], &QAction::triggered, this, &Paint::createRectangle);
    connect(actions[2], &QAction::triggered, this, &Paint::createCircle);
    connect(actions[3], &QAction::triggered, this, &Paint::createPentagon);
    connect(actions[4], &QAction::triggered, this, &Paint::createHexagon);
}

void Paint::clearScreen(QPainter* painter, QColor color) {
    painter->setPen(color);
    painter->setBrush(color);
    painter->drawRect(0, 0, screenWidth, screenHeight);
}

void Paint::update() {
    QPainter painter(this);

    QPointF cursor = QCursor::pos() - geometry().topLeft();

    clearScreen(&painter, QColor(0, 0, 0));

    foreach (Shape* s, figures) {
        QPointF center = s->getCenter();

        QPointF delta = cursor - center;

        if (s->isClicked()) {
            switch (mode) {
                case MOVING: {
                    s->move(delta.x(), delta.y());
                    break;
                }
                case SCALING: {
                    float dist = calculateDistance(cursor, center);
                    static float oldDist = dist;
                    float d = dist - oldDist;
                    s->scale(1.0 + d / 30.0);
                    oldDist = dist;
                    break;
                }
                case ROTATING: {
                    float deltaY;
                    static float oldY = cursor.y();
                    float curY = cursor.y();
                    deltaY = curY - oldY;

                    s->rotate(deltaY * 3.0 / 180.0 * 3.14);

                    oldY = curY;
                    break;
                }
                case MOVING_VERTICES: {
                    if (s->getName() == "triangle") {
                        Triangle* trig = static_cast<Triangle*>(s);
                        trig->moveVertices(cursor);
                    } else if (s->getName() == "pentagon") {
                        Pentagon* pent = static_cast<Pentagon*>(s);
                        pent->moveVertices(cursor);
                    } else if (s->getName() == "hexagon") {
                        Hexagon* hex = static_cast<Hexagon*>(s);
                        hex->moveVertices(cursor);
                    }

                    break;
                }
            }
        }

        s->draw(&painter);
    }
}

void Paint::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    update();
}

void Paint::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);
    repaint();
}

void Paint::mousePressEvent(QMouseEvent* event) {
    QPointF cursor = QCursor::pos() - geometry().topLeft();

    bool oneFigureIsClicked = false;

    foreach (Shape* s, figures) {
        if (s->isClicked()) {
            oneFigureIsClicked = true;
            break;
        }
    }

    if (event->button() == Qt::LeftButton) {
        foreach (Shape* s, figures) {
            if (s->isCursorInside(cursor) && !s->isClicked() && !oneFigureIsClicked) {
                s->setClicked(true);
            } else if (s->isClicked()) {
                s->setClicked(false);
            }

            if (s->isClicked()) {
                if (s->getName() == "triangle") {
                    Triangle* trig = static_cast<Triangle*>(s);
                    trig->setVerticesMoving(cursor);
                } else if (s->getName() == "pentagon") {
                    Pentagon* pent = static_cast<Pentagon*>(s);
                    pent->setVerticesMoving(cursor);
                } else if (s->getName() == "hexagon") {
                    Hexagon* hex = static_cast<Hexagon*>(s);
                    hex->setVerticesMoving(cursor);
                }
            } else if (!s->isClicked()) {
                if (s->getName() == "triangle") {
                    Triangle* trig = static_cast<Triangle*>(s);
                    trig->resetVerticesMoving();
                } else if (s->getName() == "pentagon") {
                    Pentagon* pent = static_cast<Pentagon*>(s);
                    pent->resetVerticesMoving();
                } else if (s->getName() == "hexagon") {
                    Hexagon* hex = static_cast<Hexagon*>(s);
                    hex->resetVerticesMoving();
                }
            }
        }
    }
}

void Paint::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event);
}

void Paint::showContextMenu(const QPoint& p) {
    QMenu contextMenu(this);

    QAction rotate("Повернуть", this);
    QAction scale("Масштабировать", this);
    QAction move("Перемещать (по умолчанию)", this);
    QAction findSquare("Найти площадь", this);
    QAction findPerimeter("Найти периметр", this);
    QAction findCenter("Найти центр масс", this);
    QAction moveVertices("Переместить вершины (для треугольника, пятиугольника и шестиугольника)", this);

    contextMenu.addAction(&rotate);
    contextMenu.addAction(&scale);
    contextMenu.addAction(&move);
    contextMenu.addAction(&findSquare);
    contextMenu.addAction(&findPerimeter);
    contextMenu.addAction(&findCenter);
    contextMenu.addAction(&moveVertices);

    connect(&rotate, &QAction::triggered, this, &Paint::goToRotatingMode);
    connect(&scale, &QAction::triggered, this, &Paint::goToScalingMode);
    connect(&move, &QAction::triggered, this, &Paint::goToMovingMode);
    connect(&moveVertices, &QAction::triggered, this, &Paint::goToMovingVerticesMode);
    connect(&findCenter, &QAction::triggered, this, &Paint::showCenters);
    connect(&findPerimeter, &QAction::triggered, this, &Paint::tellPerimeter);
    connect(&findSquare, &QAction::triggered, this, &Paint::tellSquare);

    QPointF cursor = QCursor::pos() - geometry().topLeft();

    foreach (Shape* s, figures) {
        if (s->isCursorInside(cursor)) {
            fixCursorCoordinates();
            contextMenu.exec(mapToGlobal(p));
        }
    }
}

void Paint::goToMovingMode() {
    mode = MOVING;
}

void Paint::goToScalingMode() {
    mode = SCALING;
}

void Paint::goToRotatingMode() {
    mode = ROTATING;
}

void Paint::goToMovingVerticesMode() {
    mode = MOVING_VERTICES;
}

void Paint::showCenters() {
    QPointF cursor = curPos;

    foreach (Shape* s, figures) {
        if (s->isCursorInside(cursor)) {
            s->setShowCenter(true);
        }
    }
}

void Paint::fixCursorCoordinates() {
    curPos = QCursor::pos() - geometry().topLeft();
}

void Paint::tellPerimeter() {
    double perim = 6.0;

    QPointF cursor = curPos;

    foreach (Shape* s, figures) {
        if (s->isCursorInside(cursor)) {
            perim = s->getPerimeter();
        }
    }

    QMessageBox msgBox;
    msgBox.setInformativeText("Периметр фигуры: " + QString::number(perim) + " точек");
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.exec();
}

void Paint::tellSquare() {
    QMessageBox msgBox(this);
    msgBox.setDefaultButton(QMessageBox::Cancel);

    double square = -10.0;

    QPointF cursor = curPos;

    foreach (Shape* s, figures) {
        if (s->isCursorInside(cursor)) {
            square = s->getSquare();
        }
    }

    msgBox.setInformativeText("Площадь фигуры: " + QString::number(square) + " точек");
    msgBox.exec();
}
