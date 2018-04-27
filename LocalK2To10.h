#ifndef LOCALK2TO10_H
#define LOCALK2TO10_H

#include <QDebug>

#define cout qDebug()
#define INF 10000000000
#define QT_MAX_UINT 2000000000
#define DEBUGLEVEL 1 //0 for nothing, 1 for minimum, 2 for all
#define K_UPPER 2
#define MAX_ITER 5

class LocalK2To10
{
public:
    LocalK2To10(QList<uint> allJobs);
    /*init*/
    QPair<double, QList<QList<uint>>> initFirstSol(QList<uint> allJobs);
    QPair<double, QList<QList<uint>>> runLpt(QList<uint> allJobs);
    QPair<double, QList<QList<uint>>> runSameMachine(QList<uint> allJobs);
    QPair<double, QList<QList<uint>>> runLocalSearch(QPair<double, QList<QList<uint>>> bestGlobalSol);

    /*Searching Steps*/
    QPair<double, QList<QList<uint>>> doAllSearchSteps(const QPair<double, QList<QList<uint>>> bestGlobalSol);
    QPair<double, QList<QList<uint>>> move1jobs(const QPair<double, QList<QList<uint>>> bestGlobalSol);
    QPair<double, QList<QList<uint>>> swap(const QPair<double, QList<QList<uint>>> bestGlobalSol);
    QPair<double, QList<QList<uint>>> move2jobs(const QPair<double, QList<QList<uint>>> bestGlobalSol);
    QPair<double, QList<QList<uint>>> swap2jobfor1(const QPair<double, QList<QList<uint>>> bestGlobalSol);
    QPair<double, QList<QList<uint>>> move3jobs(const QPair<double, QList<QList<uint>>> bestGlobalSol);
    QPair<double, QList<QList<uint>>> swap2jobfor2(const QPair<double, QList<QList<uint>>> bestGlobalSol);
    QPair<double, QList<QList<uint>>> cyclicMove(const QPair<double, QList<QList<uint>>> bestGlobalSol);
    QPair<double, QList<QList<uint>>> swap3jobfor3(const QPair<double, QList<QList<uint>>> bestGlobalSol);


    /*aux functions*/
    double targetFunction(QList<QList<uint>> machines);

    /*debug*/
    void printSol(const QString &name, const QPair<double, QList<QList<uint> > > sol);

    /*class vars*/
    QPair<double, QList<QList<uint>>> bestGlobalSolution;

};

#endif // LOCALK2TO10_H
//    QPair<double, QList<QList<uint>>> mergeMachines(const QPair<double, QList<QList<uint>>> currentState);
