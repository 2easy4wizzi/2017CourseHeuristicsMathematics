#include "bandb.h"
#include <QtMath>
#define QT_MAX_UINT 4294967295
BandB::BandB(QList<uint> allJobs)
{
    if(allJobs.isEmpty()){
        cout << "input is empty";
        return;
    }
    QTime timer; timer.start();
    count = 0;
    countNow = 0;
//    mode = getModelChoice();
//    QList<uint> allJobs = getInput(1);
    calculateGlobalLowerBound(allJobs);
    bestSolutionFound.first = INF;
    Node* root = initializeRoot(allJobs);
    if(root != NULL){
        runBnbRec(root, 0);
        Node* bestSol = new Node(bestSolutionFound.second, QList<uint>());
        bestSol->U = bestSolutionFound.first;
        cout << "BEST FOUND: " << bestSol->leafToString();
        cout << "nodes seen:" << count << ". run time: " << (double(timer.elapsed()) / 1000) << "seconds";
    }
}

//bool BandB::getModelChoice()
//{
//    //if true => 2<=k<=10
//    //if false => k->(k^3)
//    return true;
//}


Node* BandB::initializeRoot(const QList<uint> &allJobs)
{
    Node* treeHead = new Node(QList<QList<uint>>(), allJobs);
    uint job = treeHead->getJob();
    QList<uint> firstMachine;
    firstMachine.push_back(job);
    treeHead->machines.push_back(firstMachine);
    calcLowerBound(treeHead);
    calcUpperBoundAndCheckBest(treeHead);

    if(treeHead->L >= bestSolutionFound.first){
        if(DEBUGLEVEL == 2){
            QString msg("CUTOFF was made on ROOT. ");
            msg.append("job on hand: <" + QString::number(job) + ">. ");
            msg.append("lower bound=" + QString::number(treeHead->L) + " is bigger or equal than " + "best solution so far=" + QString::number(bestSolutionFound.first) );
            cout << "       " << msg;
        }
        if(treeHead){
            delete treeHead;
            treeHead = NULL;
        }
    }
    return treeHead;
}
/*local lower bound - check the current machins states
 *find the "heaviest" machine
 *return the max("heaviest" machine, bestGlobalLowerBound) as lower bound
*/
void BandB::calcLowerBound(Node *node) const
{
    uint bestLocalLowerBound = 0;

    for(const QList<uint>& machine: node->machines) {
        uint sumI = node->machines.size();
        for(const uint& job: machine) {
            sumI += job;
        }
        if(sumI > bestLocalLowerBound){
            bestLocalLowerBound = sumI;
        }
    }
    uint bestGlobalLowerBound = getGlobalLowerByMachinesSize(node->machines.size());
    node->L = qMax(bestGlobalLowerBound, bestLocalLowerBound);
}

uint BandB::getGlobalLowerByMachinesSize(uint machinesSize) const
{
//    cout << machinesSize;
//    cout << "perfectSplit";
//    for(int i = 0; i< 11; ++i){
//        cout << i<<perfectSplit[i];
//    }
//    cout << "pMax"<<pMax;
//    cout << "pigeonholePrinciple";
//    for(int i = 0; i< 11; ++i){
//        cout << i<<pigeonholePrinciple[i];
//    }
//    exit(0);
    return qMax( qMax(perfectSplit[machinesSize],pMax), pigeonholePrinciple[machinesSize] );
}
/*upper bound - "alg" for upper bound
 * if number of machines is smaller than K_UPPER - create new machines while jobLeft isn't empty
 * else try to locate where the "unbalance" state exists ( machine5.size=2 and all the rest are 1 -> return 6 as index )
 * foreach job in jobsLeft
 *  assign to the machine at index++
 * now see if a best solution yet is found
 * locate "heaviest" machine
 * calculate target function (machines.size + "heaviest" machine)
 * if it's better than bestSolutionFound -> replace with current found
*/
void BandB::calcUpperBoundAndCheckBest(Node *node)
{
    QList<uint> remaning = node->jobsLeft;
    QList<QList<uint>> machines = node->machines;
//    remaning = (QList<uint>() << 10<<3 );
//    QList<uint> machine1 = (QList<uint>() << 10);
//    QList<uint> machine2 = (QList<uint>() << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1);
//    machines.clear();
//    machines << machine1 << machine2;

    /*pre process*/
    QList<uint> summedMachines;
    for(const QList<uint>& m : machines){
       uint localSum(0);
       for(uint job : m){
           localSum+=job;
       }
       summedMachines.append(localSum);
    }

    /**/
    uint lowestUpperfound(QT_MAX_UINT);
    QList<QList<uint>> bestMachines;

    for(int k = summedMachines.size(); k <= K_UPPER; k++){
        QList<uint> localSummedMachines(summedMachines);
        QList<QList<uint>> localMachines(machines);
        QList<uint> localRemainng(remaning);

        /*open emtpy machines (k - summedMachines.size())*/
        for(int r=summedMachines.size(); r<k; ++r){
            localSummedMachines.append(0);
            localMachines.append(QList<uint>());
        }

        int startSize (localRemainng.size());
        for (int i = 0; i < startSize; ++i) {
            uint job (localRemainng.takeFirst());
            uint ind2(0);
            uint localPotentialTarget(QT_MAX_UINT);
            for (int j = 0; j < localSummedMachines.size(); ++j) {

                //find heaviest
                uint heaviestMachine(0);
                for(int u = 0; u<localSummedMachines.size(); ++u){
                    uint localSummedMachinesValue = (j==u) ? localSummedMachines[u] + job : localSummedMachines[u];
                    if(localSummedMachinesValue > heaviestMachine){
                        heaviestMachine = localSummedMachinesValue;
                    }
                }
                uint potentialTarget(localSummedMachines.size() + heaviestMachine);
                if(potentialTarget < localPotentialTarget){
                    localPotentialTarget = potentialTarget;
                    ind2 = j;
                }
            }
//            cout << minSum << minMachineIndex;
            /*Add next to job to minimal machine*/
            localMachines[ind2].append(job);
            localSummedMachines[ind2] += job;
//            cout << localMachines << localSummedMachines <<  localRemainng.size();
        }
        uint maxSum(0);
        for(uint summedMachine: localSummedMachines) {
            if(summedMachine > maxSum) {
                maxSum = summedMachine;
            }
        }
        uint ithTargetFunc(maxSum + localSummedMachines.size());
//        cout << "\n";
//        cout << "ithTargetFunction: " << ithTargetFunc;
//        cout << "Machines: " << localMachines;
//        cout << "Summed Machines: " << localSummedMachines;
//        cout << "Size: " << localSummedMachines.size();

        if(ithTargetFunc < lowestUpperfound) {
            lowestUpperfound = ithTargetFunc;
            bestMachines = localMachines;
        }
    }
//    cout << bestMachines << lowestUpperfound;

//    if(machines.size() < K_UPPER){
//        for(int i=machines.size(); i<K_UPPER && !remaning.isEmpty() ; ++i){
//            uint job = remaning.takeLast();
//            QList<uint> newMachine;
//            newMachine.push_back(job);
//            machines.push_back(newMachine);
//        }
//    }
//    else{
//        for(int i=1; i<machines.size(); ++i){
//            if( machines[i].size() != machines[i-1].size()){
//                startIndex = i;
//            }
//        }
//    }

//    while(!remaning.isEmpty()){
//        uint job = remaning.takeLast();
//        machines[startIndex].push_back(job);
//        startIndex = (startIndex+1)%K_UPPER;
//    }
/*see if best solution so far need to be replaced by this one*/
//    uint maxMachine(0);
//    for(const QList<uint>& machine : machines){
//        uint sigmaMachine(0);
//        for(const uint& job : machine){
//            sigmaMachine += job;
//        }
//        if(sigmaMachine > maxMachine){
//            maxMachine = sigmaMachine;
//        }
//    }
//    uint currentTargetFunc = machines.size() + maxMachine;
    if(lowestUpperfound < bestSolutionFound.first){//found a new best
        bestSolutionFound.first = lowestUpperfound;
        bestSolutionFound.second = bestMachines;
        if(DEBUGLEVEL >= 1){
            Node* newBestSol = new Node(bestMachines, QList<uint>()); count--;
            newBestSol->U = lowestUpperfound;
            cout << "       #########bestSolutionFound just got replaced"  << newBestSol->leafToString() << "#########";
            if(newBestSol){
                delete newBestSol;
            }
        }
    }
    node->U = lowestUpperfound;
}

/*global lower bound
 * for each K_LOWER <= k <= K_UPPER
 * (k + sumAllJobs/k) -> best lower bound yet
 * return the best
*/
void BandB::calculateGlobalLowerBound(const QList<uint> &allJobs)
{
    double sumAllJobs(0);
    for(const uint& job : allJobs){
        sumAllJobs += job;//using in global 1
    }

    //global lower 1 - calculation
    perfectSplit[0] = 0;
    perfectSplit[K_UPPER+1] = QT_MAX_UINT;
    for (int i = K_UPPER; i >= 1; --i) {
        perfectSplit[i] = qMin(perfectSplit[i+1], uint(qCeil(double(i) + double(sumAllJobs) / double(i))));
    }

    //global lower 2 - add at least 1 machine
    //Jobs are sorted
    pMax = allJobs.first() + 1;
//    cout << allJobs.first()<< allJobs.first()+1 << pMax;

    //global lower 3 - calculation
    //Jobs are sorted
    pigeonholePrinciple[0] = 0;
    pigeonholePrinciple[K_UPPER+1] = QT_MAX_UINT;
    for (int i = K_UPPER; i > 0; --i) {
        uint w = qCeil(double(allJobs.size()) / double(i));
        uint globalLower3(0);
        for (uint j = 0; j < w; ++j) {
            globalLower3 += allJobs[allJobs.size() - j - 1];
        }
        pigeonholePrinciple[i] = qMin(globalLower3 + i, pigeonholePrinciple[i+1]);
    }
    return;
}

void BandB::runBnbRec(Node *parentNode, uint depth)
{
    uint job = parentNode->getJob();
    if(DEBUGLEVEL == 2) {
        QString spaces(""); for(uint i=0; i<depth ; ++i){ spaces.append("  "); }
        cout << qPrintable("\n" + spaces + "*New Sub Tree Parent- depth=" + QString::number(depth)) <<parentNode->toString() <<QString("current active(job=%1)").arg(job) ;
    }
    for(int i=0; i < qMin(parentNode->machines.size()+1, K_UPPER); ++i){
        Node* sonI =  new Node(parentNode->machines, parentNode->jobsLeft);
        if(parentNode->machines.size() -1 >= i){
            sonI->machines[i].push_back(job);
        }
        else{
            QList<uint> newMachine;
            newMachine.push_back(job);
            sonI->machines.push_back(newMachine);
        }
        calcLowerBound(sonI);
        calcUpperBoundAndCheckBest(sonI);
        if(DEBUGLEVEL == 2) {
            QString spaces(""); for(uint i=0; i<depth+1; ++i){ spaces.append("  "); }
            cout << qPrintable(spaces +  "depth:" + QString::number(depth+1)) << "son" + QString::number(i) << sonI->toString();
        }
        if(!sonI->jobsLeft.isEmpty()){
            if(sonI->L >= bestSolutionFound.first){
                if(DEBUGLEVEL == 2){
                    QString spaces(""); for(uint i=0; i<depth+1; ++i){ spaces.append("  "); }
                    QString msg(spaces + "CUTOFF was made. ");
                    msg.append("job on hand: <" + QString::number(job) + ">. ");
                    msg.append("lower bound=" + QString::number(sonI->L) + " is bigger than " + "best solution so far=" + QString::number(bestSolutionFound.first) );
                    msg.prepend(spaces);
                    cout << qPrintable(msg);
                }
                if(sonI){
                    delete sonI;
                }
            }
            else{
                runBnbRec(sonI, depth + 1);
            }
        }
        else{
            if(DEBUGLEVEL == 2){
                QString spaces(""); for(uint i=0; i<depth+1; ++i){ spaces.append("  "); }
                cout << qPrintable(spaces +  "leaf:") << sonI->leafToString();
            }
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
