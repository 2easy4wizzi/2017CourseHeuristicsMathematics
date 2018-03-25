#ifndef BANDB_H
#define BANDB_H
#include <qlist.h>
#include <QDebug>

//#define cout qDebug()<< __FILE__ << __FUNCTION__ << __LINE__
#define cout qDebug()


class Node
{
public:
    Node(QList<unsigned int> _state, QList<unsigned int> _jobsLeft){
        state = _state;
        jobsLeft = _jobsLeft;
    }
    QString toString(){
        QString up("upper Bound: "  + QString::number(U));
        QString down(". lower Bound: " + QString::number(L));
        QString left(". jobs Left: <");
        for(const unsigned int& job : jobsLeft){
            left += (QString::number(job));
            if (job != jobsLeft.last()) left.append(",");
        }
        QString _state("> State: <");
        for(const unsigned int& slot : state){
            _state += (QString::number(slot));
            if (slot != state.last()) _state.append(",");
        }
        _state.append(">");

        return (up + down + left + _state);
    }
    unsigned int L;
    unsigned int U;
    QList<unsigned int> jobsLeft;
//    QList<QList<unsigned int>> state;
    QList<unsigned int> state;
    unsigned int getJob(){
        return jobsLeft.takeLast();
    }
};


class BandB
{
public:
    BandB();
    bool getModelChoice();
    QList<unsigned int> getInput();
    Node* initializeRoot();
    Node* initNewNode(QList<unsigned int> _state, QList<unsigned int> _jobsLeft);
    void calcLowerBound(Node* node);
    void calcUpperBound(Node* node);
    void runBnb();
    Node* getActive();
    unsigned int getJob();


    Node * root;
    bool mode;
    QList<unsigned int> allJobs;
    QList<Node*> activeNodes;

};

#endif // BANDB_H
