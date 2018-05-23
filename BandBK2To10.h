#ifndef BANDBK2TO10_H
#define BANDBK2TO10_H
#include "Utils.h"

class Node
{
public:
    Node(QList<QList<uint>> _parentsMachines, QList<uint> _jobsLeft);
    ~Node();
    QString leafToString();
    QString endResultToString();
    QString toString();
    uint L;
    uint U;
    QList<uint> jobsLeft;
    QList<QList<uint>> machines;
    uint getJob(){return jobsLeft.takeFirst();}
};

class BandBK2To10
{
public:
    BandBK2To10(QList<uint> allJobs);
    BandBK2To10(QList<uint> allJobs, int _numberOfMachines);
    Node *initializeRoot(const QList<uint>& allJobs);
    void calcLowerBound(Node* node) const;
    uint getGlobalLowerByMachinesSize() const;
    void calcUpperBoundAndCheckBest(Node* node);
    void calculateGlobalLowerBound(const QList<uint>& allJobs);
    void runBnbRec(Node* node, uint depth);
    uint getJob();
    double perfectSplit;//global lower 1
    uint pMax;//global lower 2
    uint pigeonholePrinciple;//global lower 3
    QList<Node*> activeNodes;
    QPair<double, QList<QList<uint>>> bestSolutionFound;
    QMap<uint,uint> cutOffHist;
    int numberOfMachines;
    QString print();
};

#endif // BANDBK2TO10_H
