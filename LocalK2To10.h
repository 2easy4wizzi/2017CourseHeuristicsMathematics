#ifndef LOCALK2TO10_H
#define LOCALK2TO10_H
#include "Utils.h"

class LocalK2To10
{
public:
    LocalK2To10(QList<uint> allJobs, int _numberOfMachines, QStringList startingAlgs, double _globalLowerBound);
    /*init*/
    QPair<double, QList<QList<uint>>> initFirstSol(QList<uint> allJobs, int numberOfMachines, QString startSolAlg);
    QPair<double, QList<QList<uint>>> runLpt(QList<uint> allJobs, int numberOfMachines);
    QPair<double, QList<QList<uint>>> runBestFit(QList<uint> allJobs, int numberOfMachines);
    QPair<double, QList<QList<uint>>> runSameMachine(QList<uint> allJobs, int numberOfMachines);
    QPair<double, QList<QList<uint>>> runLocalSearch(QPair<double, QList<QList<uint>>> bestGlobalSol);

    /*Searching Steps*/
    QPair<double, QList<QList<uint>>> doAllSearchSteps(const QPair<double, QList<QList<uint>>> bestGlobalSol);

    QPair<double, QList<QList<uint>>> move1jobsOptimal(const QPair<double, QList<QList<uint>>> bestGlobalSol, bool opt = false);
    QPair<double, QList<QList<uint>>> move2jobs(const QPair<double, QList<QList<uint>>> bestGlobalSol, bool opt = false);

    QPair<double, QList<QList<uint>>> swap1for1Optimal(const QPair<double, QList<QList<uint>>> bestGlobalSol, bool opt = false);
    QPair<double, QList<QList<uint>>> swap2for1(const QPair<double, QList<QList<uint>>> bestGlobalSol, bool opt = false);
    QPair<double, QList<QList<uint>>> swap2for2(const QPair<double, QList<QList<uint>>> bestGlobalSol, bool opt = false);


    /*aux functions*/
    QPair<double, QList<QList<uint> > > runUsingStartAlg(QPair<double, QList<QList<uint>>> startSol);
    double targetFunction(QList<QList<uint>> machines);
    double calcMse(QList<uint> summedMachinesGlobal);
    QPair<double, QList<QList<uint>>> removeZerosFromSol(QPair<double, QList<QList<uint>>> bestGlobalSol);
    double getLowerBound(int numberOfMachines,QList<uint> allJobs);

    /*debug*/
    void printSol(const QString &name, const QPair<double, QList<QList<uint> > > sol);

    /*class vars*/
    QPair<double, QList<QList<uint>>> bestGlobalSolution;
    QList<uint> summedMachinesGlobal;
    double mseGlobal;
    double mseGlobalBack;
    double globalLowerBound;
    int numberOfMachines;
};

#endif // LOCALK2TO10_H
