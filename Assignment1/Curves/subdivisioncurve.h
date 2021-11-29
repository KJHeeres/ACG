#ifndef SUBDIVISIONCURVE_H
#define SUBDIVISIONCURVE_H

#include <QVector>
#include <QVector2D>
#include <QString>

class SubdivisionCurve
{
public:
    SubdivisionCurve();

    inline QVector<QVector2D> getNetCoords() { return netCoords; }
    inline QVector<QVector2D> getCurveCoords() { return curveCoords; }
    inline QString getSubdivMethod() { return subdivMethod; }

    void setMask(QString stringMask);
    void setSubdivSteps(int newSteps);
    void setSubdivMethod(QString method);
    void presetNet(int preset);

    int findClosest(QVector2D p);

    void addPoint(QVector2D p);
    void setPointPosition(int idx, QVector2D p);
    void removePoint(int idx);

    void subdivide();


private:
    int subdivSteps;

    QString subdivMethod;

    QVector<int> subdivMask, firstStencil, secondStencil;

    QVector<QVector2D> curveCoords;

    QVector<QVector2D> netCoords;
};

#endif // SUBDIVISIONCURVE_H
