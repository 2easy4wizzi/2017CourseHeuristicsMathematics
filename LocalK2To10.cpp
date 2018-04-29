#include "LocalK2To10.h"

LocalK2To10::LocalK2To10(QList<uint> allJobs)
{
    allJobs.clear();
    allJobs << 4 << 4 << 4 << 4 << 4 << 4;//should be 4 machines !!!
	//TODO - finish all steps
    /*
     * cyclic
     * move 2
     * move 3
     * recover - do chaos -- take bin: split as evenly and place the smallest half in other bins
    */
	//remove move1jobsOptimal and mergeMachines

    bestGlobalSolution.first = INF;
    //solving for all different number of machines from 2 to 10 and returning best
    for(int numberOfMachines=2; numberOfMachines<=K_UPPER; ++numberOfMachines){
        QPair<double, QList<QList<uint>>> startSol = initFirstSol(allJobs, numberOfMachines);//init using numberOfMachines as global var
        if(DEBUGLEVELLOCAL >=2) {
            cout << numberOfMachines << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
            printSol("Start   -------- startSol", startSol);
        }
        QPair<double, QList<QList<uint>>> tempSolution = runLocalSearch(startSol);
        if(tempSolution.first < bestGlobalSolution.first){
            bestGlobalSolution = tempSolution;
        }
        if(DEBUGLEVELLOCAL >=2) cout << numberOfMachines << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    }

    //done - some summary info:
    printSol("Start   -------- startSol", initFirstSol(allJobs, bestGlobalSolution.second.size()));//for knowing what was the first sol)
    printSol("END   -------- with 0: bestSolutionFound", bestGlobalSolution);
    printSol("END   -------- bestSolutionFound", removeZerosFromSol(bestGlobalSolution));
}

QPair<double, QList<QList<uint> > > LocalK2To10::initFirstSol(QList<uint> allJobs, int numberOfMachines)
{
//    QPair<double, QList<QList<uint> > > lptFirstSol = runLpt(allJobs, numberOfMachines);
    QPair<double, QList<QList<uint> > > sameMachineFirstSol = runSameMachine(allJobs, numberOfMachines);
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

QPair<double, QList<QList<uint> > > LocalK2To10::runSameMachine(QList<uint> allJobs, int numberOfMachines)
{
    QPair<double, QList<QList<uint> > > sol;
    QList<QList<uint>> machines;
    machines << allJobs;
    for(int i=1; i<numberOfMachines; ++i){
        QList<uint> machine = (QList<uint>() << 0);
        machines << machine;
    }
    sol.first = targetFunction(machines);
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
        if(DEBUGLEVELLOCAL >=2) cout << "ITER"<< ++iterations;
        bestLocalSol = doAllSearchSteps(bestLocalSol);
        if( bestLocalSol != bestGlobalSol){
            if(DEBUGLEVELLOCAL >=2) printSol("bestLocalSol",bestLocalSol);
            bestGlobalSol = bestLocalSol;
        }
        else{
            if(DEBUGLEVELLOCAL >=2) cout << qPrintable(QString("   done with %1 iterations").arg(iterations));
            cont = false;
        }

    }

    return bestLocalSol;
}

QPair<double, QList<QList<uint> > > LocalK2To10::doAllSearchSteps(const QPair<double, QList<QList<uint> > > bestGlobalSol)
{
    QPair<double, QList<QList<uint>>> bestLocalSol;
//    //step1
//    bestLocalSol = move1jobsOptimal(bestGlobalSol);
//    if(bestLocalSol != bestGlobalSol){
//        return bestLocalSol;
//    }
//    //step2
//    bestLocalSol = swap1for1(bestGlobalSol);
//    if(bestLocalSol != bestGlobalSol){
//        return bestLocalSol;
//    }
//    //step3
//    bestLocalSol = swap2for1(bestGlobalSol);
//    if(bestLocalSol != bestGlobalSol){
//        return bestLocalSol;
//    }
    //step4
    bestLocalSol = move2jobsOptimal(bestGlobalSol);
    if(bestLocalSol != bestGlobalSol){
        return bestLocalSol;
    }
//    //step5
//    bestLocalSol = swap2for2(bestGlobalSol);
//    if(bestLocalSol != bestGlobalSol){
//        return bestLocalSol;
//    }
//    //step6
//    bestLocalSol = swap3for3(bestGlobalSol);
//    if(bestLocalSol != bestGlobalSol){
//        return bestLocalSol;
//    }

    //step6
    bestLocalSol = move3jobs(bestGlobalSol);
    if(bestLocalSol != bestGlobalSol){
        return bestLocalSol;
    }


    return bestGlobalSol;

}

QPair<double, QList<QList<uint> > > LocalK2To10::move1jobsOptimal(const QPair<double, QList<QList<uint> > > bestGlobalSol)
{
    if(DEBUGLEVELLOCAL >=2) cout << qPrintable(QString(__FUNCTION__));
    QList<QList<uint>> machines = bestGlobalSol.second;
    QPair<double, QList<QList<uint> > > bestNeighborState(bestGlobalSol);

    for(int i = 0; i < bestGlobalSol.second.size(); ++i) {
        QList<uint> sawThisJobs;
        for(int j = 0; j < machines[i].size(); ++j) {
            uint jobOnHand = machines[i][j];
            if(sawThisJobs.contains(jobOnHand) || jobOnHand==0) continue;//no need to move job with same value from the same machine or when job is 0
            machines[i][j] = 0;
            sawThisJobs.append(jobOnHand);
            for(int l = i+1; l < bestGlobalSol.second.size(); ++l) {
                machines[l].push_back(jobOnHand);
                double tf (targetFunction(machines));
                QPair<double, QList<QList<uint> > > tempCurrentState;
                tempCurrentState.first = tf; tempCurrentState.second = machines;
                if( tf < bestNeighborState.first) {
                    bestNeighborState = tempCurrentState;
                }
                machines[l].pop_back();
            }
            machines[i][j] = jobOnHand;
        }
    }
    return bestNeighborState;
}

QPair<double, QList<QList<uint> > > LocalK2To10::move2jobsOptimal(const QPair<double, QList<QList<uint> > > bestGlobalSol)
{
    if(DEBUGLEVELLOCAL >=2) cout << qPrintable(QString(__FUNCTION__));
    QList<QList<uint>> machines = bestGlobalSol.second;
    QPair<double, QList<QList<uint> > > bestNeighborState(bestGlobalSol);

    for(int i = 0; i < bestGlobalSol.second.size(); ++i) {
        QList<QPair<QPair<uint,uint>,QPair<uint,uint>>> sawThisJobs;
        for(int j = 0; j < machines[i].size(); ++j) {
            uint jobOnHand = machines[i][j];
            machines[i][j] = 0;
            for(int i2 = i; i2 < bestGlobalSol.second.size(); ++i2) {
                for(int j2 = 0; j2 < machines[i2].size(); ++j2) {
                    uint jobOnHand2 = machines[i2][j2];
                    QPair<uint,uint> values; values.first = jobOnHand; values.second = jobOnHand2;
                    QPair<uint,uint> ind; ind.first = i; ind.second = i2;
                    QPair<QPair<uint,uint>,QPair<uint,uint>> foursome;
                    foursome.first = values; foursome.second = ind;
                    if((j==j2 && i==i2) || sawThisJobs.contains(foursome)) continue;
                    sawThisJobs.append(foursome);
                    machines[i2][j2] = 0;
                    for(int k = 0; k < bestGlobalSol.second.size(); ++k) {
                        machines[k].push_back(jobOnHand);
                        for(int l = 0; l < bestGlobalSol.second.size(); ++l) {
                            machines[l].push_back(jobOnHand2);
                            double tf (targetFunction(machines));
                            if(tf < bestNeighborState.first){
                                bestNeighborState.first = tf;
                                bestNeighborState.second = machines;
                            }
                            machines[l].pop_back();
                        }
                        machines[k].pop_back();
                    }
                    machines[i2][j2] = jobOnHand2;
                }
            }
            machines[i][j] = jobOnHand;
        }
    }
    return bestNeighborState;
}

QPair<double, QList<QList<uint> > > LocalK2To10::move3jobs(const QPair<double, QList<QList<uint> > > bestGlobalSol)
{
    if(DEBUGLEVELLOCAL >=2) cout << qPrintable(QString(__FUNCTION__));
    QList<QList<uint>> machines = bestGlobalSol.second;
    QPair<double, QList<QList<uint> > > bestNeighborState(bestGlobalSol);

    for(int i = 0; i < bestGlobalSol.second.size(); ++i) {
//        QList< QPair< QList<uint>, QList<uint> > > sawThisJobs;
        for(int j = 0; j < machines[i].size(); ++j) {
            uint jobOnHand = machines[i][j];
            machines[i][j] = 0;
            for(int i2 = i; i2 < bestGlobalSol.second.size(); ++i2) {
                for(int j2 = 0; j2 < machines[i2].size(); ++j2) {
                    uint jobOnHand2 = machines[i2][j2];
                    machines[i2][j2] = 0;
                    for(int i3 = i; i3 < bestGlobalSol.second.size(); ++i3) {
                        for(int j3 = 0; j3 < machines[i3].size(); ++j3) {
                            uint jobOnHand3 = machines[i3][j3];
                            machines[i3][j3] = 0;
//                        QList<uint> ind; ind << i; ind << = i2;
//                        QList< QPair< QList<uint>, QList<uint> > >sixsome;
//                        sixsome.first = values; sixsome.second = ind;
//                        QPair<uint,uint> values; values.first = jobOnHand; values.second = jobOnHand2;
//                        if((j==j2 && i==i2) || sawThisJobs.contains(foursome)) continue;
//                        sawThisJobs.append(foursome);
                            for(int k = 0; k < bestGlobalSol.second.size(); ++k) {
                                machines[k].push_back(jobOnHand);
                                for(int l = 0; l < bestGlobalSol.second.size(); ++l) {
                                    machines[l].push_back(jobOnHand2);
                                    for(int m = 0; m < bestGlobalSol.second.size(); ++m) {
                                        machines[m].push_back(jobOnHand3);
                                        double tf (targetFunction(machines));
                                        if(tf < bestNeighborState.first){
                                            bestNeighborState.first = tf;
                                            bestNeighborState.second = machines;
                                            return bestNeighborState;
                                        }
                                        machines[m].pop_back();
                                    }
                                    machines[l].pop_back();
                                }
                                machines[k].pop_back();
                            }
                            machines[i3][j3] = jobOnHand3;
                        }
                    }
                    machines[i2][j2] = jobOnHand2;
                }
            }
            machines[i][j] = jobOnHand;
        }
    }
    return bestNeighborState;
}

QPair<double, QList<QList<uint>>> LocalK2To10::swap1for1(const QPair<double, QList<QList<uint>>> bestGlobalSol) {

    if(DEBUGLEVELLOCAL >=2) cout << qPrintable(QString(__FUNCTION__));
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
    if(DEBUGLEVELLOCAL >=2) cout << qPrintable(QString(__FUNCTION__));
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
    if(DEBUGLEVELLOCAL >=2) cout << qPrintable(QString(__FUNCTION__));
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
                            if(sawThisJobsDst.contains(kv) || jobOnHand1+jobOnHand2 == jobOnHand3+jobOnHand4) continue;//no need to move job with same value from the same machine
                            sawThisJobsDst.append(kv);
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
    if(DEBUGLEVELLOCAL >=2) cout << qPrintable(QString(__FUNCTION__));
    QPair<double, QList<QList<uint>>> bestLocalSol = bestGlobalSol;
    QList<QList<uint>> machines = bestLocalSol.second;

    for(int i = 0; i < machines.size(); ++i) {
        if(machines.at(i).size() < 3) continue;
        QList<QList<uint>> sawThisJobsSrc;

        for(int j = 0; j < machines[i].size(); ++j) {
            uint jobOnHand1 = machines[i].at(j);
            for(int r = j+1; r < machines[i].size(); ++r) {
                uint jobOnHand2 = machines[i][r];
                for(int s = r+1; s < machines[i].size(); ++s) {
                    uint jobOnHand3 = machines[i][s];
                    QList<uint> tupleSrc = (QList<uint>() << jobOnHand1 << jobOnHand2 << jobOnHand3);
                    if(sawThisJobsSrc.contains(tupleSrc)) continue;//no need to move job with same value from the same machine
                    sawThisJobsSrc.append(tupleSrc);

                    for(int l = i+1; l < machines.size(); ++l) {
                        QList<QList<uint>> sawThisJobsDst;
                        for(int k = 0; k < machines[l].size() ; ++k) {
                            uint jobOnHand4 = machines[l].at(k);
                            for(int v = k+1; v < machines[l].size(); ++v) {
                                uint jobOnHand5 = machines[l][v];
                                for(int e = v+1; e < machines[l].size(); ++e) {
                                    uint jobOnHand6 = machines[l][e];
                                    QList<uint> tupleDst = (QList<uint>() << jobOnHand4 << jobOnHand5 << jobOnHand6);
                                    if(sawThisJobsDst.contains(tupleDst) || jobOnHand1+jobOnHand2+jobOnHand3 == jobOnHand4+jobOnHand5+jobOnHand6) continue;//no need to move job with same value from the same machine
                                    sawThisJobsDst.append(tupleDst);

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

QPair<double, QList<QList<uint> > > LocalK2To10::removeZerosFromSol(QPair<double, QList<QList<uint> > > bestGlobalSol)
{
    QList<QList<uint>> newMachines;
    for(QList<uint> machine : bestGlobalSol.second){
        if(!machine.isEmpty()){
            QList<uint> newMachine;
            for(uint job : machine){
                if(job > 0){
                    newMachine << job;
                }
            }
            if(!newMachine.isEmpty()){
                newMachines << newMachine;
            }
        }
    }
    bestGlobalSol.first = targetFunction(newMachines);
    bestGlobalSol.second = newMachines;
    return bestGlobalSol;
}

