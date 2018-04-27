#include "LocalK2To10.h"

LocalK2To10::LocalK2To10(QList<uint> allJobs)
{
    for(numberOfMachines=2; numberOfMachines<=K_UPPER; ++numberOfMachines){

        QPair<double, QList<QList<uint>>> startSol = initFirstSol(allJobs);//done - continue from here
        if(DEBUGLEVEL >=2) {
            cout << numberOfMachines << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
            printSol("Start   -------- startSol", startSol);
        }
        QPair<double, QList<QList<uint>>> tempSolution = runLocalSearch(startSol);
        if(tempSolution.first < bestGlobalSolution.first){
            bestGlobalSolution = tempSolution;
        }
        if(DEBUGLEVEL >=2) cout << numberOfMachines << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    }
    printSol("END   -------- bestSolutionFound", bestGlobalSolution);
}

QPair<double, QList<QList<uint> > > LocalK2To10::initFirstSol(QList<uint> allJobs)
{
//    QPair<double, QList<QList<uint> > > lptFirstSol = runLpt(allJobs);
    QPair<double, QList<QList<uint> > > sameMachineFirstSol = runSameMachine(allJobs);
    return sameMachineFirstSol;
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

QPair<double, QList<QList<uint> > > LocalK2To10::runSameMachine(QList<uint> allJobs)
{
    QPair<double, QList<QList<uint> > > sol;
    QList<QList<uint>> machines;
    QList<uint> firstMachine;
    double tf(1);
    for (uint job : allJobs){
        tf += job;
        firstMachine << job;
    }
    machines << firstMachine;
    for(int i=1; i<numberOfMachines; ++i){
        QList<uint> machine = (QList<uint>() << 0);
        machines << machine;
    }
    sol.first = tf;
    sol.second = machines;
    return sol;
}

void LocalK2To10::printSol(const QString& name,const QPair<double, QList<QList<uint> > > sol)
{
    cout << qPrintable(QString("   %1 found:").arg(name));
    cout << qPrintable(QString("      target function = %1, num of machines=%2").arg(sol.first).arg(sol.second.size()));
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
    cout << qPrintable(QString("      machines content = %1").arg(machinesContent));
}

QPair<double, QList<QList<uint> > > LocalK2To10::runLocalSearch(QPair<double, QList<QList<uint> > > bestGlobalSol)
{

    QPair<double, QList<QList<uint>>> bestLocalSol = bestGlobalSol;
    uint iterations(0);
    bool cont(true);
    while(cont && iterations < 10){
        if(DEBUGLEVEL >=2) cout << "ITER"<< ++iterations;
        bestLocalSol = doAllSearchSteps(bestLocalSol);
        if( bestLocalSol != bestGlobalSol){
            if(DEBUGLEVEL >=2) printSol("bestLocalSol",bestLocalSol);
            bestGlobalSol = bestLocalSol;
        }
        else{
            if(DEBUGLEVEL >=2) cout << qPrintable(QString("   done with %1 iterations").arg(iterations));
            cont = false;
        }

    }

    return bestLocalSol;
}

QPair<double, QList<QList<uint> > > LocalK2To10::doAllSearchSteps(const QPair<double, QList<QList<uint> > > bestGlobalSol)
{
    QPair<double, QList<QList<uint>>> bestLocalSol;
    //step1
    bestLocalSol = move1jobsOptimal(bestGlobalSol);
    if(bestLocalSol != bestGlobalSol){
        return bestLocalSol;
    }
    //step2
    bestLocalSol = swap1for1(bestGlobalSol);
    if(bestLocalSol != bestGlobalSol){
        return bestLocalSol;
    }
    //step3
    bestLocalSol = swap2for1(bestGlobalSol);
    if(bestLocalSol != bestGlobalSol){
        return bestLocalSol;
    }
    //step4
    bestLocalSol = swap2for2(bestGlobalSol);
    if(bestLocalSol != bestGlobalSol){
        return bestLocalSol;
    }
  //step5
    bestLocalSol = swap3for3(bestGlobalSol);
    if(bestLocalSol != bestGlobalSol){
        return bestLocalSol;
    }

    return bestGlobalSol;

}

QPair<double, QList<QList<uint> > > LocalK2To10::move1jobsOptimal(const QPair<double, QList<QList<uint> > > bestGlobalSol)
{
    if(DEBUGLEVEL >=2) cout << qPrintable(QString(__FUNCTION__));
    QList<QList<uint>> machines = bestGlobalSol.second;
    QPair<double, QList<QList<uint> > > bestNeighborState(bestGlobalSol);
    int machinesNumber = bestGlobalSol.second.size();

    for(int i = 0; i < machinesNumber; ++i) {
        QList<uint> sawThisJobs;
        for(int j = 0; j < machines[i].size(); ++j) {
            QList<uint> currentMachine = machines[i];
            uint jobOnHand = currentMachine.takeAt(j);
            if(sawThisJobs.contains(jobOnHand)) continue;//no need to move job with same value from the same machine
            sawThisJobs.append(jobOnHand);
            QList<QList<uint>> machinesCopy;
            for(int k=0; k<machines.size(); ++k){
                if(!machines[k].isEmpty() && k != i){
                    machinesCopy << machines[k];
                }
                else if(!currentMachine.isEmpty() && k == i){
                    machinesCopy << currentMachine;
                }
            }
            int newMachinesNumber = machinesCopy.size();
            for(int l = 0; l < newMachinesNumber+1 && l<K_UPPER; ++l) {
                if(l == 2){
                    cout;
                }
                if(l==newMachinesNumber){
                    QList<uint> newM = (QList<uint>() << jobOnHand);
                    machinesCopy.push_back(newM);
                    double tf (targetFunction(machinesCopy));
                    QPair<double, QList<QList<uint> > > tempCurrentState;
                    tempCurrentState.first = tf; tempCurrentState.second = machinesCopy;
                    if( tf < bestNeighborState.first && tempCurrentState != bestGlobalSol) {
                        bestNeighborState = tempCurrentState;
//                        printSol("bestNeighborState",bestNeighborState);
                    }
                    machinesCopy.pop_back();
                }
                else if(newMachinesNumber != machines.size() || (newMachinesNumber == machines.size() && i!=l)){
                    //if newSize == oldSize => we don't want to put the job on the same machine it came from.
                    //so if they are equal => do 'if' only if i!=l. if they are not equal, always do the 'if' (the i machine was removed)
                    machinesCopy[l].push_back(jobOnHand);
                    double tf (targetFunction(machinesCopy));
                    QPair<double, QList<QList<uint> > > tempCurrentState;
                    tempCurrentState.first = tf; tempCurrentState.second = machinesCopy;
                    if( tf < bestNeighborState.first) {
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
//    bestNeighborState.first = targetFunction(bestNeighborState.second);
    return bestNeighborState;
}

QPair<double, QList<QList<uint>>> LocalK2To10::swap1for1(const QPair<double, QList<QList<uint>>> bestGlobalSol) {

    if(DEBUGLEVEL >=2) cout << qPrintable(QString(__FUNCTION__));
    QPair<double, QList<QList<uint>>> bestLocalSol = bestGlobalSol;
    QList<QList<uint>> machines = bestLocalSol.second;

    for(int i = 0; i < machines.size(); ++i) {
        QList<uint> sawThisJobsSrc;
        for(int j = 0; j < machines[i].size(); ++j) {
            uint jobOnHand1 = machines[i].at(j);
            if(sawThisJobsSrc.contains(jobOnHand1)) continue;//no need to move job with same value from the same machine
            sawThisJobsSrc.append(jobOnHand1);
            for(int l = i+1; l < machines.size(); ++l) {
                QList<uint> sawThisJobsDst;
                for(int k = 0; k < machines[l].size() ; ++k) {
                    uint jobOnHand2 = machines[l].at(k);
                    if(sawThisJobsDst.contains(jobOnHand2) || jobOnHand1 == jobOnHand2) continue;//no need to move job with same value from the same machine
                    machines[i][j] = jobOnHand2;
                    machines[l][k] = jobOnHand1;
                    double tf ( targetFunction(machines) );
                    if( tf < bestLocalSol.first){
                        bestLocalSol.first = tf;
                        bestLocalSol.second = machines;
                        return bestLocalSol;
                    }
                    machines[i][j] = jobOnHand1;
                    machines[l][k] = jobOnHand2;

                }
            }
        }
    }
    return bestGlobalSol;
}

QPair<double, QList<QList<uint> > > LocalK2To10::swap2for1(const QPair<double, QList<QList<uint> > > bestGlobalSol)
{
    if(DEBUGLEVEL >=2) cout << qPrintable(QString(__FUNCTION__));
    QPair<double, QList<QList<uint>>> bestLocalSol = bestGlobalSol;
    QList<QList<uint>> machines = bestLocalSol.second;

    for(int i = 0; i < machines.size(); ++i) {
        if(machines.at(i).size() < 2) continue;
        QList<QPair<uint,uint>> sawThisJobsSrc;
        for(int j = 0; j < machines[i].size(); ++j) {
            uint jobOnHand1 = machines[i].at(j);
            for(int r = j+1; r < machines[i].size(); ++r) {
                uint jobOnHand2 = machines[i][r];
                QPair<uint,uint> ir;
                ir.first = jobOnHand1; ir.second = jobOnHand2;
                if(sawThisJobsSrc.contains(ir)) continue;//no need to move job with same value from the same machine
                sawThisJobsSrc.append(ir);



                for(int l = i+1; l < machines.size(); ++l) {
                    QList<uint> sawThisJobsDst;
                    for(int k = 0; k < machines[l].size() ; ++k) {
                        uint jobOnHand3 = machines[l].at(k);
                        if(sawThisJobsDst.contains(jobOnHand2) || jobOnHand1+jobOnHand2 == jobOnHand3) continue;//no need to move job with same value from the same machine
                        machines[i][j] = 0;
                        machines[i][r] = jobOnHand3;
                        machines[l][k] = jobOnHand1 + jobOnHand2;
                        double tf ( targetFunction(machines) );
                        if( tf < bestLocalSol.first){
                            bestLocalSol.first = tf;
                            machines[i].takeAt(j);
                            machines[l][k] = jobOnHand1;
                            machines[l] << jobOnHand2;
                            bestLocalSol.second = machines;

                            return bestLocalSol;
                        }
                        machines[i][j] = jobOnHand1;
                        machines[i][r] = jobOnHand2;
                        machines[l][k] = jobOnHand3;

                    }
                }
            }
        }
    }
    return bestGlobalSol;
}

QPair<double, QList<QList<uint> > > LocalK2To10::swap2for2(const QPair<double, QList<QList<uint> > > bestGlobalSol)
{
    if(DEBUGLEVEL >=2) cout << qPrintable(QString(__FUNCTION__));
    QPair<double, QList<QList<uint>>> bestLocalSol = bestGlobalSol;
    QList<QList<uint>> machines = bestLocalSol.second;

    for(int i = 0; i < machines.size(); ++i) {
        if(machines.at(i).size() < 2) continue;
        QList<QPair<uint,uint>> sawThisJobsSrc;
        for(int j = 0; j < machines[i].size(); ++j) {
            uint jobOnHand1 = machines[i].at(j);
            for(int r = j+1; r < machines[i].size(); ++r) {
                uint jobOnHand2 = machines[i][r];
                QPair<uint,uint> ir;
                ir.first = jobOnHand1; ir.second = jobOnHand2;
                if(sawThisJobsSrc.contains(ir)) continue;//no need to move job with same value from the same machine
                sawThisJobsSrc.append(ir);



                for(int l = i+1; l < machines.size(); ++l) {
                    QList<QPair<uint,uint>> sawThisJobsDst;
                    for(int k = 0; k < machines[l].size() ; ++k) {
                        uint jobOnHand3 = machines[l].at(k);
                        for(int v = k+1; v < machines[l].size(); ++v) {
                            uint jobOnHand4 = machines[l][v];
                            QPair<uint,uint> kv;
                            kv.first = jobOnHand3; kv.second = jobOnHand4;
                            if(sawThisJobsSrc.contains(kv)) continue;//no need to move job with same value from the same machine
                            sawThisJobsSrc.append(kv);

                            if(sawThisJobsDst.contains(kv) || jobOnHand1+jobOnHand2 == jobOnHand3+jobOnHand4) continue;//no need to move job with same value from the same machine
                            machines[i][j] = jobOnHand3;
                            machines[i][r] = jobOnHand4;
                            machines[l][k] = jobOnHand1;
                            machines[l][v] = jobOnHand2;
                            double tf ( targetFunction(machines) );
                            if( tf < bestLocalSol.first){
                                bestLocalSol.first = tf;
                                bestLocalSol.second = machines;

                                return bestLocalSol;
                            }
                            machines[i][j] = jobOnHand1;
                            machines[i][r] = jobOnHand2;
                            machines[l][k] = jobOnHand3;
                            machines[l][v] = jobOnHand4;

                        }
                    }
                }
            }
        }
    }
    return bestGlobalSol;
}

QPair<double, QList<QList<uint> > > LocalK2To10::swap3for3(const QPair<double, QList<QList<uint> > > bestGlobalSol)
{
    if(DEBUGLEVEL >=2) cout << qPrintable(QString(__FUNCTION__));
    QPair<double, QList<QList<uint>>> bestLocalSol = bestGlobalSol;
    QList<QList<uint>> machines = bestLocalSol.second;

    for(int i = 0; i < machines.size(); ++i) {
        if(machines.at(i).size() < 3) continue;
//        QList<QPair<uint,uint>> sawThisJobsSrc; //QPair<uint,QPair<uint,uint>>

        for(int j = 0; j < machines[i].size(); ++j) {
            uint jobOnHand1 = machines[i].at(j);
            for(int r = j+1; r < machines[i].size(); ++r) {
                uint jobOnHand2 = machines[i][r];
                for(int s = r+1; s < machines[i].size(); ++s) {
                    uint jobOnHand3 = machines[i][s];

//                    QPair<uint,uint> ir; //QPair<uint,QPair<uint,uint>>
//                    ir.first = jobOnHand1; ir.second = jobOnHand2;
//                    if(sawThisJobsSrc.contains(ir)) continue;//no need to move job with same value from the same machine
//                    sawThisJobsSrc.append(ir);

                    for(int l = i+1; l < machines.size(); ++l) {
//                        QList<QPair<uint,uint>> sawThisJobsDst;
                        for(int k = 0; k < machines[l].size() ; ++k) {
                            uint jobOnHand4 = machines[l].at(k);
                            for(int v = k+1; v < machines[l].size(); ++v) {
                                uint jobOnHand5 = machines[l][v];
                                for(int e = v+1; e < machines[l].size(); ++e) {
                                    uint jobOnHand6 = machines[l][e];
//                                QPair<uint,uint> kv;
//                                kv.first = jobOnHand3; kv.second = jobOnHand4;
//                                if(sawThisJobsSrc.contains(kv)) continue;//no need to move job with same value from the same machine
//                                sawThisJobsSrc.append(kv);

//                                if(sawThisJobsDst.contains(kv) || jobOnHand1+jobOnHand2 == jobOnHand3+jobOnHand4) continue;//no need to move job with same value from the same machine
                                machines[i][j] = jobOnHand4;
                                machines[i][r] = jobOnHand5;
                                machines[i][s] = jobOnHand6;

                                machines[l][k] = jobOnHand1;
                                machines[l][v] = jobOnHand2;
                                machines[l][e] = jobOnHand3;
                                double tf ( targetFunction(machines) );
                                if( tf < bestLocalSol.first){
                                    bestLocalSol.first = tf;
                                    bestLocalSol.second = machines;

                                    return bestLocalSol;
                                }
                                machines[i][j] = jobOnHand1;
                                machines[i][r] = jobOnHand2;
                                machines[i][s] = jobOnHand3;

                                machines[l][k] = jobOnHand4;
                                machines[l][v] = jobOnHand5;
                                machines[l][e] = jobOnHand6;
                                }

                            }
                        }
                    }
                }
            }
        }
    }
    return bestGlobalSol;
}

//QPair<double, QList<QList<uint> > > LocalK2To10::mergeMachines(const QPair<double, QList<QList<uint> > > currentState) {
//    QPair<double, QList<QList<uint> > > myCurrentState(currentState);
//    QList<QList<uint>> machines = myCurrentState.second;
//    QList<uint> summedMachines;
//    uint maxMachine(0);
//    int ind(0);
//    for(QList<uint> m : machines){
//        summedMachines.insert(ind,0);
//        for(uint job : m){
//            summedMachines[ind] += job;
//        }
//        if(summedMachines[ind] > maxMachine) maxMachine = summedMachines[ind];
//        ++ind;
//    }

//    QPair<int,int> bestMergeIndices;
//    double bestFit(INF);//closest to the max
//    bestMergeIndices.first = -1; bestMergeIndices.second = -1;
//    int numberMachines(summedMachines.size());
//    for(int i=0; i<qMin(K_UPPER, numberMachines); ++i){
//        for(int j=0; j<qMin(K_UPPER, numberMachines) ; ++j){
//            if(i!=j){
//                uint sum = summedMachines.at(i) + summedMachines.at(j);
//                if(sum <= maxMachine && (maxMachine - sum < bestFit)){
//                    bestFit = maxMachine - sum;
//                    bestMergeIndices.first = i; bestMergeIndices.second = j;
//                }
//            }
//        }
//    }

//    if(bestMergeIndices.first != -1) {
//        QList<uint> m = myCurrentState.second.takeAt(bestMergeIndices.second);
//        myCurrentState.second[bestMergeIndices.first].append(m);
//        myCurrentState.first--;
//    }

//    return myCurrentState;
//}

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

