#include "bandb.h"

BandB::BandB(QList<unsigned int> allJobs)
{
    QTime timer; timer.start();
//    mode = getModelChoice();
//    QList<unsigned int> allJobs = getInput(1);
    bestGlobalLowerBound = calculateGlobalLowerBound(allJobs);
    bestSolutionFound.first = INF;
    Node* root = initializeRoot(allJobs);
    if(root != NULL){
        runBnbRec(root);
        Node* bestSol = new Node(bestSolutionFound.second, QList<unsigned int>());
        bestSol->U = bestSolutionFound.first;
        if(DEBUGLEVEL >= 1) cout << "BEST FOUND: " << bestSol->leafToString();
        cout << "nodes seen:" << count << ". run time: " << (double(timer.elapsed()) / 1000) << "seconds";
    }
}

//bool BandB::getModelChoice()
//{
//    //if true => 2<=k<=10
//    //if false => k->(k^3)
//    return true;
//}


Node* BandB::initializeRoot(const QList<unsigned int> &allJobs)
{
    Node* treeHead = new Node(QList<QList<unsigned int>>(), allJobs);
    unsigned int job = treeHead->getJob();
    QList<unsigned int> firstMachine;
    firstMachine.push_back(job);
    treeHead->machines.push_back(firstMachine);
    calcLowerBound(treeHead);
    calcUpperBoundAndCheckBest(treeHead);

    if(treeHead->L >= bestSolutionFound.first){
        if(DEBUGLEVEL == 2){
            QString msg("CUTOFF was made on ROOT. ");
            msg.append("job on hand: <" + QString::number(job) + ">. ");
            msg.append("lower bound=" + QString::number(treeHead->L) + " is bigger than " + "best solution so far=" + QString::number(bestSolutionFound.first) );
            cout << "       " << msg;
        }
        if(treeHead){
            delete treeHead;
            treeHead = NULL;
        }
    }
    return treeHead;
}

void BandB::calcLowerBound(Node *node) const
{
    unsigned int bestLocalLowerBound = 0;

    for(const QList<unsigned int>& machine: node->machines) {
        unsigned int sumI = 0;
        for(const unsigned int& job: machine) {
            sumI += job;
        }
        if(sumI > bestLocalLowerBound)
            bestLocalLowerBound = sumI;
    }
    node->L = qMax(bestGlobalLowerBound, bestLocalLowerBound);
}

void BandB::calcUpperBoundAndCheckBest(Node *node)
{
    QList<unsigned int> remaning = node->jobsLeft;
    QList<QList<unsigned int>> machines = node->machines;
    unsigned int startIndex(0);


    if(machines.size() < 10){
        for(int i=machines.size(); i<10 && !remaning.isEmpty() ; ++i){
            unsigned int job = remaning.takeLast();
            QList<unsigned int> newMachine;
            newMachine.push_back(job);
            machines.push_back(newMachine);
        }
    }
    else{
        for(int i=1; i<machines.size(); ++i){
            if( machines[i].size() != machines[i-1].size()){
                startIndex = i;
            }
        }
    }

    while(!remaning.isEmpty()){
        unsigned int job = remaning.takeLast();
        machines[startIndex].push_back(job);
        startIndex = (startIndex+1)%10;
    }

    unsigned int maxMachine(0);
    for(const QList<unsigned int>& machine : machines){
        unsigned int sigmaMachine(0);
        for(const unsigned int& job : machine){
            sigmaMachine += job;
        }
        if(sigmaMachine > maxMachine){
            maxMachine = sigmaMachine;
        }
    }
    unsigned int currentTargetFunc = machines.size() + maxMachine;
    if(currentTargetFunc < bestSolutionFound.first){//found a new best
        bestSolutionFound.first = currentTargetFunc;
        bestSolutionFound.second = machines;
        if(DEBUGLEVEL >= 1){
            Node* newBestSol = new Node(machines, QList<unsigned int>()); count--;
            newBestSol->U = currentTargetFunc;
            cout << "       #########bestSolutionFound just got replaced"  << newBestSol->leafToString() << "#########";
            if(newBestSol){
                delete newBestSol;
            }
        }
    }
    node->U = currentTargetFunc;
}

unsigned int BandB::calculateGlobalLowerBound(const QList<unsigned int> &allJobs)
{
    double sumAllJobs(0);
    for(const unsigned int& job : allJobs){
        sumAllJobs += job;
    }


    //TODO figure out if no need for loop
    double firstGlobalLower(INFINITY);
    for(int i=2; i<=10; ++i){
        double current = i + sumAllJobs/i;
        if(current < firstGlobalLower){
            firstGlobalLower = current;
        }
    }
    double maximalLowerBound = qRound(firstGlobalLower);//    qMax()
    return maximalLowerBound;
}

void BandB::runBnbRec(Node *parentNode)
{
    static int iter =1;
    unsigned int job = parentNode->getJob();
    if(DEBUGLEVEL == 2) cout << "current active(job=" + QString::number(job) + ")"<<parentNode->toString();
    for(int i=0; i < qMin(parentNode->machines.size()+1, 10); ++i){
        Node* sonI =  new Node(parentNode->machines, parentNode->jobsLeft);
        if(parentNode->machines.size() -1 >= i){
            sonI->machines[i].push_back(job);
        }
        else{
            QList<unsigned int> newMachine;
            newMachine.push_back(job);
            sonI->machines.push_back(newMachine);
        }
        calcLowerBound(sonI);
        calcUpperBoundAndCheckBest(sonI);
        if(DEBUGLEVEL == 2) cout << "   " << "son" + QString::number(i) << sonI->toString();
        if(!sonI->jobsLeft.isEmpty()){
            if(sonI->L >= bestSolutionFound.first){
                if(DEBUGLEVEL == 2){
                    QString msg("CUTOFF was made. ");
                    msg.append("job on hand: <" + QString::number(job) + ">. ");
                    msg.append("lower bound=" + QString::number(sonI->L) + " is bigger than " + "best solution so far=" + QString::number(bestSolutionFound.first) );
                    cout << "       " << msg;
                }
                if(sonI){
                    delete sonI;
                }
            }
            else{
                runBnbRec(sonI);
            }
        }
        else{
            if(DEBUGLEVEL == 2) cout << "       " << "leaf:" << sonI->leafToString();
            if(sonI){
                delete sonI;
            }
        }
    }
    delete parentNode;
}

//bool BandB::eventFilter(QObject *obj, QEvent *event)
//{
//    cout << event->type();
//    return QObject::eventFilter(obj, event);
//}
