#ifndef BANDB_H
#define BANDB_H
#include <qlist.h>
#include <QDebug>
#include <QTime>
#include <QEvent>
#include <QWidget>

//#define cout qDebug()<< __FILE__ << __FUNCTION__ << __LINE__
#define cout qDebug()
#define INF 100000000
#define DEBUGLEVEL 2 //0 for nothing, 1 for minimum, 2 for all
#define K_UPPER 3
#define K_LOWER 3


static uint count = 0;
static uint countNow = 0;

class Node
{
public:
    Node(QList<QList<uint>> _parentsMachines, QList<uint> _jobsLeft)
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


class BandB //: public QWidget
{

//    Q_OBJECT
public:
    BandB(QList<uint> allJobs);
//    bool getModelChoice();
    Node *initializeRoot(const QList<uint>& allJobs);
    void calcLowerBound(Node* node) const;
    void calcUpperBoundAndCheckBest(Node* node);
    uint calculateGlobalLowerBound(const QList<uint>& allJobs);
    void runBnbRec(Node* node);
    uint getJob();
//    bool mode;
    uint bestGlobalLowerBound;
    QList<Node*> activeNodes;
    QPair<uint, QList<QList<uint>>> bestSolutionFound;
//protected:
//    bool eventFilter(QObject* obj, QEvent* event);

};

#endif // BANDB_H
