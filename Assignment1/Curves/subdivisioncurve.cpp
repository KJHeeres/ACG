#include "subdivisioncurve.h"
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <typeinfo>

SubdivisionCurve::SubdivisionCurve()
{
    subdivSteps = 0;
    subdivMethod = QString("CPU");
}

void SubdivisionCurve::presetNet(int preset) {
    netCoords.clear();

    switch (preset) {
    case 0:
        // 'Pentagon'
        netCoords.reserve(5);
        netCoords.append(QVector2D(-0.25, -0.5));
        netCoords.append(QVector2D(-0.75, 0.0));
        netCoords.append(QVector2D(-0.25, 0.75));
        netCoords.append(QVector2D(0.75, 0.5));
        netCoords.append(QVector2D(0.5, -0.75));
        break;
    case 1:
        // 'Basis'
        netCoords.reserve(9);
        netCoords.append(QVector2D(-1.0, -0.25));
        netCoords.append(QVector2D(-0.75, -0.25));
        netCoords.append(QVector2D(-0.5, -0.25));
        netCoords.append(QVector2D(-0.25, -0.25));
        netCoords.append(QVector2D(0.0, 0.50));
        netCoords.append(QVector2D(0.25, -0.25));
        netCoords.append(QVector2D(0.5, -0.25));
        netCoords.append(QVector2D(0.75, -0.25));
        netCoords.append(QVector2D(1.0, -0.25));
        break;
    case 2:
        // 'G'
        netCoords.reserve(14);
        netCoords.append(QVector2D(0.75, 0.35));
        netCoords.append(QVector2D(0.75, 0.75));
        netCoords.append(QVector2D(-0.75, 0.75));
        netCoords.append(QVector2D(-0.75, -0.75));
        netCoords.append(QVector2D(0.75, -0.75));
        netCoords.append(QVector2D(0.75, 0.0));
        netCoords.append(QVector2D(0.0, 0.0));
        netCoords.append(QVector2D(0.0, -0.2));
        netCoords.append(QVector2D(0.55, -0.2));
        netCoords.append(QVector2D(0.55, -0.55));
        netCoords.append(QVector2D(-0.55, -0.55));
        netCoords.append(QVector2D(-0.55, 0.55));
        netCoords.append(QVector2D(0.55, 0.55));
        netCoords.append(QVector2D(0.55, 0.35));
        break;
    }
}

void SubdivisionCurve::subdivide() {

    size_t n = netCoords.size();/* number of new points in the subdivided curve */
    curveCoords.clear();
    curveCoords = QVector<QVector2D>(n);

    //subdivSteps
    //firstStencil
    //secondStencil

    qDebug() << "::called subdivide::";

    //even points
    for(size_t i = 0; i < netCoords.size()-1; ++i) {
        if(i%2 == 0){
            curveCoords[i] = QVector2D((float) i / 4, (float) i / 4)/* masked point */;
        }
    }

    //odd points
    for(size_t i = 1; i < netCoords.size() - 1; ++i) {
        //compute odd points
        if(i%2 == 1){
            curveCoords[i] = QVector2D((float) i / 4, (float) i / 4)/* masked point */;
        }
    }


}

void SubdivisionCurve::addPoint(QVector2D p) {
    netCoords.append(p);
    subdivide();
    //you should probably recalculate the curve
}

void SubdivisionCurve::setPointPosition(int idx, QVector2D p) {
    netCoords[idx] = p;
    subdivide();
    //you should probably recalculate the curve
}

void SubdivisionCurve::removePoint(int idx) {
    netCoords.remove(idx);
    subdivide();
    //you should probably recalculate the curve
}

void SubdivisionCurve::setSubdivSteps(int newSteps) {
    subdivSteps = newSteps;
    qDebug() << "::subdiv steps changed::";
}

void SubdivisionCurve::setSubdivMethod(QString method) {
    subdivMethod = method;
    qDebug() << "::subdiv method changed::";
}

void SubdivisionCurve::setMask(QString stringMask) {
    subdivMask.clear();

    QString trimmedMask;
    trimmedMask = stringMask.trimmed();

    // Convert to sequence of integers
    QTextStream intSeq(&trimmedMask);
    while (!intSeq.atEnd()) {
        int k;
        intSeq >> k;
        subdivMask.append(k);
    }

    // Stencils represent affine combinations (i.e. they should sum to unity)
    float normalizeValue = 0;

    firstStencil.clear();
    secondStencil.clear();

    for (int k = 0; k < subdivMask.size(); ++k) {
        if (k % 2) {
            normalizeValue += subdivMask[k];
            firstStencil.append(subdivMask[k]);
        }
        else {
            secondStencil.append(subdivMask[k]);
        }
    }

    qDebug() << ":: Extracted stencils" << firstStencil << "and" << secondStencil;
}



int SubdivisionCurve::findClosest(QVector2D p) {
    int ptIndex = -1;
    float currentDist, minDist = 4;


    for (int k = 0; k < netCoords.size(); k++) {
        currentDist = netCoords[k].distanceToPoint(p);
        if (currentDist < minDist) {
            minDist = currentDist;
            ptIndex = k;
        }
    }

    return ptIndex;
}
