#include "LocalK2To10.h"

LocalK2To10::LocalK2To10(QList<uint> allJobs)
{
//    currentState = bestSolutionFound = initFirstSol(allJobs);//done - continue from here

    QList<uint> first = (QList<uint>() << 5 << 5<< 5);
    QList<uint> second = (QList<uint>() << 50);
    QList<uint> th = (QList<uint>() << 70);
    QList<uint> fo= (QList<uint>() << 30);
    QList<uint> five= (QList<uint>() << 50);
    currentState.second.clear();
//    currentState.second << allJobs;
    currentState.second << first;
    currentState.first = 15;


    printSol("currentState", currentState);
    bestSolutionFound = runLocalSearch(currentState);
    printSol("#####bestSolutionFound", bestSolutionFound);
}

QPair<double, QList<QList<uint> > > LocalK2To10::initFirstSol(QList<uint> allJobs)
{
    QPair<double, QList<QList<uint> > > lptFirstSol = runLpt(allJobs);
    //QPair<double, QList<QList<uint> > > anotherFirstSol = runBla(allJobs);

    //select best first solution
    QPair<double, QList<QList<uint> > >bestSolutionFound;
    bestSolutionFound.first = lptFirstSol.first;
    bestSolutionFound.second = lptFirstSol.second;
    return bestSolutionFound;
}

QPair<double, QList<QList<uint> > > LocalK2To10::runLpt(QList<uint> allJobs)
{
    QPair<double, QList<QList<uint> > > lptFirstSol;
    lptFirstSol.first = INF;

    //for every K<10
    for(int k = 0; k < K_UPPER; k++){
        QList<uint> localSummedMachines;
        QList<QList<uint>> localMachines;
        QList<uint> localRemainng(allJobs);

        /*open emtpy machines*/
        for(int r=0; r<=k; ++r){
            localSummedMachines.append(0);
            localMachines.append(QList<uint>());
        }

        int jobsNumber(localRemainng.size());
        //run on all jobs
        for (int i = 0; i < jobsNumber; ++i) {
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
            /*Add next to job to minimal cost machine*/
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

        if(ithTargetFunc < lptFirstSol.first) {
            lptFirstSol.first = ithTargetFunc;
            lptFirstSol.second = localMachines;
        }
    }

    printSol("lptFirstSol",lptFirstSol);

    return lptFirstSol;
}

void LocalK2To10::printSol(const QString& name,const QPair<double, QList<QList<uint> > > sol)
{
    cout << qPrintable(QString("%1 found:").arg(name));
    cout << qPrintable(QString("   target function = %1, num of machines=%2").arg(sol.first).arg(sol.second.size()));
    QString machinesContent;
    for(const QList<uint>& m : sol.second){
        machinesContent += "(";
        for(uint j : m){
            machinesContent += QString("%1, ").arg(j);
        }
        machinesContent = machinesContent.mid(0,machinesContent.size()-2);
        machinesContent += "), ";
    }
    machinesContent = machinesContent.mid(0,machinesContent.size()-2);
    cout << qPrintable(QString("   machines content = %1").arg(machinesContent));
}

QPair<double, QList<QList<uint> > > LocalK2To10::runLocalSearch(QPair<double, QList<QList<uint> > > currentState)
{
    QMap<QString, QPair<double, QList<QList<uint>>>> bestSolutions;
    for(int i=0; i<MAX_ITER; ++i){
        cout << "ITER"<< i;
        bestSolutions["MergeMachines"] = mergeMachines(currentState);
        printSol("MergeMachines",bestSolutions["MergeMachines"]);
        bestSolutions["Move1jobs"]     = move1jobs(currentState);
        printSol("Move1jobs",bestSolutions["Move1jobs"]);
        double bestSoFar(currentState.first);
        QString bestKey;
        for(QString s : bestSolutions.keys()){
            if(bestSolutions[s].first <= bestSoFar){
                bestSoFar = bestSolutions[s].first;
                bestKey = s;
            }
        }
        QPair<double, QList<QList<uint>>> maybeSol = bestSolutions[bestKey];
        if(maybeSol == currentState){
            //handle no improvment
            cout << "death"; return currentState;
        }
        else{
            currentState = maybeSol;
        }

    }
    return currentState;
}

QPair<double, QList<QList<uint> > > LocalK2To10::move1jobs(const QPair<double, QList<QList<uint> > > currentState)
{
    QList<QList<uint>> machines = currentState.second;
    QList<QList<uint>> machinesCopy;
    QPair<double, QList<QList<uint> > > bestNeighborState(currentState);
    int machinesNumber = currentState.second.size();

    for(int i = 0; i < machinesNumber; ++i) {
        for(int j = 0; j < machines[i].size(); ++j) {
            QList<uint> currentMachine = machines[i];
            uint jobOnHand = currentMachine.takeAt(j);
            for(int k=0; k<machines.size(); ++k){
                if(!machines[k].isEmpty() && k != i){
                    machinesCopy << machines[k];
                }
                else if(!currentMachine.isEmpty() && k == i){
                    machinesCopy << currentMachine;
                }
            }

            int newMachinesNumber = machinesCopy.size();
            for(int l = 0; l < newMachinesNumber+1 ; ++l) {
                if(l==newMachinesNumber && l<K_UPPER){
                    QList<uint> newM = (QList<uint>() << jobOnHand);
                    machinesCopy.push_back(newM);
                    double tf (targetFunction(machinesCopy));
                    QPair<double, QList<QList<uint> > > tempCurrentState;
                    tempCurrentState.first = tf; tempCurrentState.second = machinesCopy;
                    if( tf <= bestNeighborState.first && tempCurrentState != currentState) {
                        bestNeighborState = tempCurrentState;
//                        printSol("bestNeighborState",bestNeighborState);
                    }
                    machinesCopy.pop_back();
                }
                else{
                    machinesCopy[l].push_back(jobOnHand);
                    double tf (targetFunction(machinesCopy));
                    QPair<double, QList<QList<uint> > > tempCurrentState;
                    tempCurrentState.first = tf; tempCurrentState.second = machinesCopy;
                    if( tf <= bestNeighborState.first&& tempCurrentState != currentState) {
                        bestNeighborState = tempCurrentState;
//                        printSol("bestNeighborState",bestNeighborState);
                    }
                    machinesCopy[l].pop_back();
                }
            }
        }
    }

//printSol("bestNeighborState",bestNeighborState);

    for(int i=0; i< bestNeighborState.second.size(); ++i){
        if(bestNeighborState.second.at(i).isEmpty()){
            bestNeighborState.second.removeAt(i);
        }
    }
    return bestNeighborState;
}

QPair<double, QList<QList<uint> > > LocalK2To10::mergeMachines(const QPair<double, QList<QList<uint> > > currentState) {
    QPair<double, QList<QList<uint> > > myCurrentState(currentState);
    QList<QList<uint>> machines = myCurrentState.second;
    QList<uint> summedMachines;
    uint maxMachine(0);
    int ind(0);
    for(QList<uint> m : machines){
        summedMachines.insert(ind,0);
        for(uint job : m){
            summedMachines[ind] += job;
        }
        if(summedMachines[ind] > maxMachine) maxMachine = summedMachines[ind];
        ++ind;
    }

    QPair<int,int> bestMergeIndices;
    double bestFit(INF);//closest to the max
    bestMergeIndices.first = -1; bestMergeIndices.second = -1;
    int numberMachines(summedMachines.size());
    for(int i=0; i<qMin(K_UPPER, numberMachines); ++i){
        for(int j=0; j<qMin(K_UPPER, numberMachines) ; ++j){
            if(i!=j){
                uint sum = summedMachines.at(i) + summedMachines.at(j);
                if(sum <= maxMachine && (maxMachine - sum < bestFit)){
                    bestFit = maxMachine - sum;
                    bestMergeIndices.first = i; bestMergeIndices.second = j;
                }
            }
        }
    }

    if(bestMergeIndices.first != -1) {
        QList<uint> m = myCurrentState.second.takeAt(bestMergeIndices.second);
        myCurrentState.second[bestMergeIndices.first].append(m);
        myCurrentState.first--;
    }

    return myCurrentState;
}

double LocalK2To10::targetFunction(QList<QList<uint>> machines) {
    //Number of machines k
    uint machinesNumber = machines.size();
    uint maximalMachineTime = 0;
    uint emptyMachines = 0;
    //Sum all machines jobs time and gets maximal machine by time
    for(QList<uint> machine : machines) {
        if(machine.isEmpty()){
            emptyMachines++;
        }
        else{
            uint machineTime = 0;
            for(uint job : machine) {
                machineTime += job;
            }
            if(machineTime > maximalMachineTime) {
                maximalMachineTime = machineTime;
            }
        }
    }
    return (machinesNumber + maximalMachineTime - emptyMachines);
}

