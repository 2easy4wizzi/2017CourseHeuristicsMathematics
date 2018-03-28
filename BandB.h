#ifndef BANDB_H
#define BANDB_H
#include <qlist.h>
#include <QDebug>
#include <QTime>

//#define cout qDebug()<< __FILE__ << __FUNCTION__ << __LINE__
#define cout qDebug()
#define INF 100000000
#define DEBUGLEVEL 0 //0 for nothing, 1 for minimum, 2 for all


static double count = 0;

class Node
{
public:
    Node(QList<QList<unsigned int>> _parentsMachines, QList<unsigned int> _jobsLeft)
        :L(0), U(0){

        count++;
        machines = _parentsMachines;
        jobsLeft = _jobsLeft;
    }
    QString leafToString(){
        QString machineStr("Target function=" + QString::number(U) + " Number of Machines=" + QString::number(machines.size()) + " . Content: ");
        int i(0);

        for(const QList<unsigned int>& machine : machines){
            if (i++) machineStr.append(",");
            machineStr.append("<");
            int j(1);
            for(const unsigned int& job: machine){
                machineStr += (QString::number(job));
                if (j++ != machine.size()) machineStr.append(",");
            }
            machineStr.append(">");
        }
        return machineStr;
    }

    QString toString(){
        QString up("U="  + QString::number(U));
        QString down(" L=" + QString::number(L));
        QString left(". jobs Left: <");
        int j(1);
        for(const unsigned int& job : jobsLeft){
            left += (QString::number(job));
            if (j++ != jobsLeft.size()) left.append(",");
        }
        left.append(">");
        QString machineStr(". Machines:");
        int i(0);
        for(const QList<unsigned int>& machine : machines){
            if (i++) machineStr.append(",");
            machineStr.append("<");
            int j(1);
            for(const unsigned int& job: machine){
                machineStr += (QString::number(job));
                if (j++ != machine.size()) machineStr.append(",");
            }
            machineStr.append(">");
        }

        return (up + down + machineStr+left  );
    }
    unsigned int L;
    unsigned int U;
    QList<unsigned int> jobsLeft;
    QList<QList<unsigned int>> machines;
//    QList<unsigned int> machines;
    unsigned int getJob(){
        return jobsLeft.takeFirst();
    }
};


class BandB
{
public:
    BandB();
    bool getModelChoice();
    const QList<unsigned int> getInput(int inputBatch);
    Node* initializeRoot(const QList<unsigned int>& allJobs);
    void calcLowerBound(Node* node) const;
    void calcUpperBoundAndCheckBest(Node* node);
    unsigned int calculateGlobalLowerBound(const QList<unsigned int>& allJobs);
    void runBnb();
    Node* getActive();
    unsigned int getJob();

    bool mode;
    unsigned int bestGlobalLowerBound;
    QList<Node*> activeNodes;
    QPair<unsigned int, QList<QList<unsigned int>>> bestSolutionFound;

};

#endif // BANDB_H
