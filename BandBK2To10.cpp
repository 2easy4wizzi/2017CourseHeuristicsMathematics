#include "BandBK2To10.h"

BandBK2To10::BandBK2To10(QList<uint> allJobs)
{
    if(allJobs.isEmpty()){ cout << "input is empty"; return; }
    QTime timer; timer.start();//time
    nodesSeenSoFar = 0; nodesActive = 0;//memory leak
    calculateGlobalLowerBound(allJobs);
    bestSolutionFound.first = INF;
    Node* root = initializeRoot(allJobs);
    if(root != NULL){
        runBnbRec(root, 0);
        Node* bestSol = new Node(bestSolutionFound.second, QList<uint>());
        bestSol->U = bestSolutionFound.first;
        cout << "BEST FOUND: " << bestSol->leafToString();
        cout << "nodes seen:" << nodesSeenSoFar << ". run time: " << (double(timer.elapsed()) / 1000) << "seconds";
    }
}

Node* BandBK2To10::initializeRoot(const QList<uint> &allJobs)
{
    Node* treeHead = new Node(QList<QList<uint>>(), allJobs);
    uint job = treeHead->getJob();
    QList<uint> firstMachine;
    firstMachine.push_back(job);
    treeHead->machines.push_back(firstMachine);
    calcLowerBound(treeHead);
    calcUpperBoundAndCheckBest(treeHead);

    if(treeHead->L >= bestSolutionFound.first){
        if(DEBUGLEVEL == 1){
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
 *find the "heaviest" machine + machines size
 *return the max("heaviest" machine, bestGlobalLowerBound) as lower bound
*/
void BandBK2To10::calcLowerBound(Node *node) const
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

//given a nodes current number of machines - return the maximum of 3 global lower bounds
uint BandBK2To10::getGlobalLowerByMachinesSize(uint machinesSize) const{
    return qMax( qMax(perfectSplit[machinesSize],pMax), pigeonholePrinciple[machinesSize] );
}

/*upper bound - "alg" for upper bound
    from i=(current size of machines) to K_UPPER(upper bound)
        do LPT on i machines
            calculate for the i machines the target function -> targetLocal
    save the best targetLocal as this node Upper bound
    if targetLocal < bestSolutionFound
        save the solution and the target function value in a global variable
*/
void BandBK2To10::calcUpperBoundAndCheckBest(Node *node)
{
    QList<uint> remaning = node->jobsLeft;
    QList<QList<uint>> machines = node->machines;

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
    double lowestUpperfound(INF);
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
            double localPotentialTarget(INF);
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
            /*Add next to job to minimal machine*/
            localMachines[ind2].append(job);
            localSummedMachines[ind2] += job;
        }
        uint maxSum(0);
        for(uint summedMachine: localSummedMachines) {
            if(summedMachine > maxSum) {
                maxSum = summedMachine;
            }
        }
        double ithTargetFunc(maxSum + localSummedMachines.size());

        if(ithTargetFunc < lowestUpperfound) {
            lowestUpperfound = ithTargetFunc;
            bestMachines = localMachines;
        }
    }

    if(lowestUpperfound < bestSolutionFound.first){//found a new best
        bestSolutionFound.first = lowestUpperfound;
        bestSolutionFound.second = bestMachines;
        if(DEBUGLEVEL >= 1){
            Node* newBestSol = new Node(bestMachines, QList<uint>()); nodesSeenSoFar--;
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
  3 different lower bounds:
  1) perfectSplit: for each number of machines calculate the avg of jobs and add the number of machines
  2) pMax: the biggest job + 1
  3) pigeonholePrinciple: for each number of machines i do ((number of jobs)/i)+i
*/
void BandBK2To10::calculateGlobalLowerBound(const QList<uint> &allJobs)
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

//the heart of the BNB
void BandBK2To10::runBnbRec(Node *parentNode, uint depth)
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