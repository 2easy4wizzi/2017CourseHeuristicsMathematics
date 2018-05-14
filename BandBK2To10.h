#ifndef BANDBK2TO10_H
#define BANDBK2TO10_H
#include <qlist.h>
#include <QDebug>
#include <QTime>
#include <QtMath>
#include <QWidget>

#define cout2 qDebug()
#define INF 10000000000
#define QT_MAX_UINT 2000000000
#define DEBUGLEVEL 1 //0 for nothing, 1 for minimum, 2 for all
#define K_UPPER 10
//#define K_LOWER 2

//used to track memory leaks
static uint nodesSeenSoFar = 0;
static uint nodesActive = 0;

class Node
{
public:
    Node(QList<QList<uint>> _parentsMachines, QList<uint> _jobsLeft):L(0), U(0){
        nodesSeenSoFar++;
        nodesActive++;
        if(DEBUGLEVEL >=1 && (nodesSeenSoFar % 1000000 == 0)){
            cout2 << nodesSeenSoFar << "nodes so far" << "; active" << nodesActive << "; deleted" << nodesSeenSoFar - nodesActive;
        }
        machines = _parentsMachines;
        jobsLeft = _jobsLeft;
    }
    ~Node(){
        nodesActive--;
    }

    QString leafToString(){
        QString machineStr("Target function=" + QString::number(U) + " Number of Machines=" + QString::number(machines.size()) + " . Content: ");
        int i(0);

        for(const QList<uint>& machine : machines){
            if (i++) machineStr.append(",");
            machineStr.append("<");
            int j(1);
            for(const uint& job: machine){
                machineStr += (QString::number(job));
                if (j++ != machine.size()) machineStr.append(",");
            }
            machineStr.append(">");
        }
        return machineStr;
    }
    QString endResultToString(){
        QString machineStr("Machines:");
        int i(0);
        for(const QList<uint>& machine : machines){
            if (i++) machineStr.append(",");
            machineStr.append("<");
            int j(1);
            for(const uint& job: machine){
                machineStr += (QString::number(job));
                if (j++ != machine.size()) machineStr.append(",");
            }
            machineStr.append(">");
        }

        return (machineStr);
    }
    QString toString(){
        QString up("U="  + QString::number(U));
        QString down(" L=" + QString::number(L));
        QString left(". jobs Left: <");
        int j(1);
        for(const uint& job : jobsLeft){
            left += (QString::number(job));
            if (j++ != jobsLeft.size()) left.append(",");
        }
        left.append(">");
        QString machineStr(". Machines:");
        int i(0);
        for(const QList<uint>& machine : machines){
            if (i++) machineStr.append(",");
            machineStr.append("<");
            int j(1);
            for(const uint& job: machine){
                machineStr += (QString::number(job));
                if (j++ != machine.size()) machineStr.append(",");
            }
            machineStr.append(">");
        }

        return (up + down + machineStr+left  );
    }
    uint L;
    uint U;
    QList<uint> jobsLeft;
    QList<QList<uint>> machines;
    uint getJob(){
        return jobsLeft.takeFirst();
    }
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
    uint perfectSplit;//global lower 1
    uint pMax;//global lower 2
    uint pigeonholePrinciple;//global lower 3
    QList<Node*> activeNodes;
    QPair<double, QList<QList<uint>>> bestSolutionFound;
    QMap<uint,uint> cutOffHist;
    int numberOfMachines;
    QString print();
};

#endif // BANDBK2TO10_H
