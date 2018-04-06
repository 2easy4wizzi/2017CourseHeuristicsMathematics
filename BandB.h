#ifndef BANDB_H
#define BANDB_H
#include <qlist.h>
#include <QDebug>
#include <QTime>
#include <QEvent>

//#define cout qDebug()<< __FILE__ << __FUNCTION__ << __LINE__
#define cout qDebug()
#define INF 100000000
#define DEBUGLEVEL 1 //0 for nothing, 1 for minimum, 2 for all


static unsigned int count = 0;
static unsigned int countNow = 0;

class Node
{
public:
    Node(QList<QList<unsigned int>> _parentsMachines, QList<unsigned int> _jobsLeft)
        :L(0), U(0){

        count++;
        countNow++;
        if(DEBUGLEVEL >=1 && (count % 10000000 == 0)){
            cout << count << "nodes so far" << "; active" << countNow << "; deleted" << count - countNow;
        }
        machines = _parentsMachines;
        jobsLeft = _jobsLeft;
    }
    ~Node(){
        countNow--;
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
    unsigned int getJob(){
        return jobsLeft.takeFirst();
    }
};


class BandB
{
public:
    BandB(QList<unsigned int> allJobs);
//    bool getModelChoice();
    Node *initializeRoot(const QList<unsigned int>& allJobs);
    void calcLowerBound(Node* node) const;
    void calcUpperBoundAndCheckBest(Node* node);
    unsigned int calculateGlobalLowerBound(const QList<unsigned int>& allJobs);
    void runBnbRec(Node* node);
    unsigned int getJob();
//    bool mode;
    unsigned int bestGlobalLowerBound;
    QList<Node*> activeNodes;
    QPair<unsigned int, QList<QList<unsigned int>>> bestSolutionFound;
protected:
//    bool eventFilter(QObject* obj, QEvent* event);

};

#endif // BANDB_H
