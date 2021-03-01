#include "convexpolygon.h"

double crossProduct(QPointF vect1, QPointF vect2)
{
    return (vect1.x() * vect2.y() - vect1.y() * vect2.x());
}

bool isConvex(QPointF* points, int n)
{
    double prev = 0.0;
    double curr = 0.0;

    for (int i = 0; i < n; i++) {
        QVector<QPointF> temp = { points[i], points[(i + 1) % n], points[(i + 2) % n] };

        QPointF vect1 = temp[1] - temp[0];
        QPointF vect2 = temp[1] - temp[2];

        curr = crossProduct(vect1, vect2);

        if (curr != 0) {
            if (curr * prev < 0) {
                return false;
            }
            else {
                prev = curr;
            }
        }
    }

    return true;
}
