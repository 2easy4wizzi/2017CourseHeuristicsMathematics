#ifndef LOCALK2TO10_H
#define LOCALK2TO10_H

#include <QDebug>

#define cout qDebug()
#define INF 10000000000
#define QT_MAX_UINT 2000000000
#define DEBUGLEVEL 1 //0 for nothing, 1 for minimum, 2 for all
#define K_UPPER 10
#define MAX_ITER 5

class LocalK2To10
{
public:
    LocalK2To10(QList<uint> allJobs);
    QPair<double, QList<QList<uint>>> initFirstSol(QList<uint> allJobs);
    QPair<double, QList<QList<uint>>> runLpt(QList<uint> allJobs);
    void printSol(const QString &name, const QPair<double, QList<QList<uint> > > sol);
    QPair<double, QList<QList<uint>>> runLocalSearch(QPair<double, QList<QList<uint>>> currentState);
    QPair<double, QList<QList<uint>>> move1jobs(const QPair<double, QList<QList<uint>>> currentState);
    QPair<double, QList<QList<uint>>> mergeMachines(const QPair<double, QList<QList<uint>>> currentState);
    double targetFunction(QList<QList<uint>> machines);

    QPair<double, QList<QList<uint>>> bestSolutionFound;
    QPair<double, QList<QList<uint>>> currentState;
};

#endif // LOCALK2TO10_H
