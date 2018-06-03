#include "LocalK2To10.h"

QPair<double, QList<QList<uint>>> LocalK2To10::runUsingStartAlg(QPair<double, QList<QList<uint> > > startSol)
{
    if(startSol.first > globalLowerBound){
        QPair<double, QList<QList<uint>>> bestLocalSolution;
        bestLocalSolution.first = INF;

        if(DEBUGLEVELLOCAL >= 3) {cout << numberOfMachines << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";}

        bestLocalSolution.first = INF;
        bestLocalSolution = runLocalSearch(startSol);
        return removeZerosFromSol(bestLocalSolution);
    }
    else {
        return startSol;
    }
}

QPair<double, QList<QList<uint>>> bestGlobalSolution;
QList<uint> summedMachinesGlobal;
double mseGlobal;
double mseGlobalBack;
double globalLowerBound;
int numberOfMachines;

LocalK2To10::LocalK2To10(QList<uint> allJobs, int _numberOfMachines, QStringList startingAlgs, double _globalLowerBound):
     mseGlobal(0), mseGlobalBack(INF), globalLowerBound(_globalLowerBound), numberOfMachines(_numberOfMachines)
{
    bestGlobalSolution.first = 0;
    if(!allJobs.isEmpty()){
        QPair<double, QString> winnerSol; winnerSol.first=INF;
        QTime timer; timer.start();//time
        if(globalLowerBound <= 0){
            globalLowerBound = getLowerBound(numberOfMachines, allJobs);
        }
        if(DEBUGLEVELLOCAL >= 2) cout << "numberOfMachines:" << numberOfMachines << "; lower bound:" << globalLowerBound;

        for(QString startAlg : startingAlgs){
            cout << "startAlg" << startAlg;
            QPair<double, QList<QList<uint>>> startSol = initFirstSol(allJobs, numberOfMachines, startAlg);//init using numberOfMachines as global var
            if(DEBUGLEVELLOCAL >= 1) {printSol(QString("Start   %1").arg(startAlg), startSol);}
            QPair<double, QList<QList<uint>>> bestLocalFound = runUsingStartAlg(startSol);
            if(DEBUGLEVELLOCAL >= 1) {printSol(QString("END   -------- bestSolutionFound for alg %1").arg(startAlg), (bestLocalFound)); cout<<"";}

            if(bestLocalFound.first < winnerSol.first){
                winnerSol.first = bestLocalFound.first;
                winnerSol.second = startAlg;
                bestGlobalSolution = bestLocalFound;
            }
            if(bestLocalFound.first <= globalLowerBound){//found opt
                break;
            }
        }
        if(DEBUGLEVELLOCAL >= 1) printSol(QString("END; WINNING RESULT:::::: bestSolutionFound for start alg %1").arg(winnerSol.second), bestGlobalSolution); cout<<"";
        if(DEBUGLEVELLOCAL >= 1) cout << "Run time: " << (double(timer.elapsed()) / 1000) << "seconds";
    }
}

QPair<double, QList<QList<uint> > > LocalK2To10::initFirstSol(QList<uint> allJobs, int numberOfMachines, QString startSolAlg)
{
    QPair<double, QList<QList<uint> > > retVal;
    if(startSolAlg == "LPT"){
        QPair<double, QList<QList<uint> > > lptFirstSol = runLpt(allJobs, numberOfMachines);
        retVal = lptFirstSol;
    }
    else if(startSolAlg == "BESTFIT"){
        QPair<double, QList<QList<uint> > > bestFitSol = runBestFit(allJobs, numberOfMachines);
        retVal = bestFitSol;
    }
    else{
        QPair<double, QList<QList<uint> > > sameMachineFirstSol = runSameMachine(allJobs, numberOfMachines);
        retVal = sameMachineFirstSol;
    }
    return retVal;
}

double LocalK2To10::getLowerBound(int numberOfMachines,QList<uint> allJobs)
{
    Q_UNUSED(numberOfMachines);
    Q_UNUSED(allJobs);
    double sumAllJobs(0);
    for(const uint& job : allJobs){
        sumAllJobs += job;//using in global 1
    }

    //global lower 1 - calculation
    double perfectSplit = double(qCeil(double(numberOfMachines) + double(sumAllJobs) / double(numberOfMachines)));

    //global lower 2 - add at least 1 machine
    //Jobs are sorted
    double pMax = allJobs.first() + 1;

    //global lower 3 - calculation
    //Jobs are sorted

    uint w = qCeil(double(allJobs.size()) / double(numberOfMachines));
    uint globalLower3(0);
    for (uint j = 0; j < w; ++j) {
        globalLower3 += allJobs[allJobs.size() - j - 1];
    }
    double pigeonholePrinciple = globalLower3 + numberOfMachines;

    if(DEBUGLEVELLOCAL >= 3) {
        cout << "pMax: " << pMax;
        cout << "perfectSplit: " << perfectSplit;
        cout << "pigeonholePrinciple: " << pigeonholePrinciple;
    }
    return qMax(pigeonholePrinciple , qMax(perfectSplit, pMax));
}

QPair<double, QList<QList<uint> > > LocalK2To10::runLpt(QList<uint> allJobs, int numberOfMachines)
{
    QPair<double, QList<QList<uint> > > lptFirstSol;
    lptFirstSol.first = INF;

    //for every K<10
    int k = numberOfMachines;
    {
        QList<uint> localSummedMachines;
        QList<QList<uint>> localMachines;
        QList<uint> localRemainng(allJobs);

        /*open emtpy machines*/
        for(int r=0; r<k; ++r){
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

    return lptFirstSol;
}

QPair<double, QList<QList<uint> > > LocalK2To10::runBestFit(QList<uint> allJobs, int numberOfMachines)
{
    double sumAllJobs(0);
    for(const uint& job : allJobs){
        sumAllJobs += job;//using in global 1
    }

    //global lower 1 - calculation
    double perfectSplit = double(qCeil( double(sumAllJobs) / double(numberOfMachines)));

    //global lower 2 - add at least 1 machine
    //Jobs are sorted
    double pMax = allJobs.first();

    //global lower 3 - calculation
    //Jobs are sorted

    uint w = qCeil(double(allJobs.size()) / double(numberOfMachines));
    uint globalLower3(0);
    for (uint j = 0; j < w; ++j) {
        globalLower3 += allJobs[allJobs.size() - j - 1];
    }
    double pigeonholePrinciple = globalLower3;

    if(DEBUGLEVELLOCAL >= 3) {
        cout << "pMax: " << pMax;
        cout << "perfectSplit: " << perfectSplit;
        cout << "pigeonholePrinciple: " << pigeonholePrinciple;
    }
    double bestLow = qMax(pigeonholePrinciple , qMax(perfectSplit, pMax));

    QPair<double, QList<QList<uint> > > sol;
    QList<QList<uint>> machines;

    for(int i=0; i<numberOfMachines; ++i){
        uint currentSum(0);
        QList<uint> machine;
        QList<int> usedJobs;
        for(int j=0; j<allJobs.size(); ++j){
            if(currentSum+allJobs.at(j) <= bestLow){
                machine << allJobs.at(j);
                currentSum += allJobs.at(j);
                usedJobs <<j;
            }
        }
        machines << machine;
        QList<uint> newAllJob;
        for(int k=0; k<allJobs.size(); ++k){
            if(!usedJobs.contains(k)){
                newAllJob << allJobs.at(k);
            }
        }
        allJobs = newAllJob;
    }
    while(!allJobs.isEmpty()){
        machines[0].append(allJobs.takeAt(0));
    }
    sol.first = targetFunction(machines);
    sol.second = machines;
    return sol;
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
    cout << qPrintable(QString("      target function = %1, num of machines=%2, square root lms=%3").arg(sol.first).arg(sol.second.size()).arg(QString::number(sqrt(mseGlobal), 'g', 15)));
    uint i(0);
    uint jobsNumber(0);
    QString machinesContent;
    for(const QList<uint>& m : sol.second){
        uint total(0);
        for(uint j : m){
            if(j!=0) jobsNumber++;
            total += j;
        }
        machinesContent += QString("bucket%1 sum:%2, content= (").arg(++i).arg(total);
        for(uint j : m){
            machinesContent += QString("%1, ").arg(j);
        }
        machinesContent = machinesContent.mid(0,machinesContent.size()-2);
        machinesContent += ")\n ";
    }
    machinesContent = machinesContent.mid(0,machinesContent.size()-2);
}

QPair<double, QList<QList<uint> > > LocalK2To10::runLocalSearch(QPair<double, QList<QList<uint> > > bestGlobalSol)
{

    QPair<double, QList<QList<uint>>> bestLocalSol = bestGlobalSol;
    uint iterations(0);
    bool cont(true);
    while(cont){
        if(DEBUGLEVELLOCAL >= 3) cout << "ITER"<< ++iterations;
        bestLocalSol = doAllSearchSteps(bestLocalSol);
        if( bestLocalSol != bestGlobalSol){
            if(DEBUGLEVELLOCAL >= 2) {
                double oldmessq = sqrt(mseGlobalBack); double newmessq = sqrt(mseGlobal); float diff = float(oldmessq - newmessq);
                QString mse = QString("old square mse=%1 , new square mse=%2, diff=%3").arg(QString::number(oldmessq, 'g', 15)).arg(QString::number(newmessq, 'g', 15)).arg(QString::number(diff, 'f', 20));
                cout << QString("IMPROVEMENT found: %1").arg(mse);
                printSol("bestLocalSol",bestLocalSol);
            }
            bestGlobalSol = bestLocalSol;
        }
        else{
            if(DEBUGLEVELLOCAL >= 3) cout << qPrintable(QString("   done with %1 iterations").arg(iterations));
            cont = false;
        }

    }

    return bestLocalSol;
}

QPair<double, QList<QList<uint> > > LocalK2To10::doAllSearchSteps(const QPair<double, QList<QList<uint> > > bestGlobalSol)
{
    if(bestGlobalSol.first <= globalLowerBound){
        if(DEBUGLEVELLOCAL >= 1) cout << "Found Opt - stopping...";
        return bestGlobalSol;
    }
    if(DEBUGLEVELLOCAL >= 2) {
        double oldmessq = sqrt(mseGlobalBack); double newmessq = sqrt(mseGlobal); float diff = float(oldmessq - newmessq);
        QString mse = QString("old square mse=%1 , new square mse=%2, diff=%3").arg(QString::number(oldmessq, 'g', 15)).arg(QString::number(newmessq, 'g', 15)).arg(QString::number(diff, 'f', 20));
        cout << "globalLowerBound" << globalLowerBound << "TF" <<bestGlobalSol.first <<mse;
    }
    mseGlobalBack = mseGlobal;
    QPair<double, QList<QList<uint>>> bestLocalSol;
    summedMachinesGlobal.clear();
    for(QList<uint> machine: bestGlobalSol.second) {
        uint totalSum(0);
        for(uint job: machine) {
            totalSum += job;
        }
        summedMachinesGlobal.append(totalSum);
    }
    mseGlobal = calcMse(summedMachinesGlobal);

    //step1
    bestLocalSol = move1jobsOptimal(bestGlobalSol, true);
    if(bestLocalSol != bestGlobalSol){
        return bestLocalSol;
    }
    //step2
    bestLocalSol = swap1for1Optimal(bestGlobalSol,true);
    if(bestLocalSol != bestGlobalSol){
        return bestLocalSol;
    }
    //step3
    bestLocalSol = swap2for1(bestGlobalSol,false);
    if(bestLocalSol != bestGlobalSol){
        return bestLocalSol;
    }
    //step4
    bestLocalSol = move2jobs(bestGlobalSol,false);
    if(bestLocalSol != bestGlobalSol){
        return bestLocalSol;
    }
    //step5
    bestLocalSol = swap2for2(bestGlobalSol,false);
    if(bestLocalSol != bestGlobalSol){
        return bestLocalSol;
    }


    return bestGlobalSol;

}

QPair<double, QList<QList<uint> > > LocalK2To10::move1jobsOptimal(const QPair<double, QList<QList<uint> > > bestGlobalSol, bool opt)
{
    if(DEBUGLEVELLOCAL >= 3) cout << qPrintable(QString(__FUNCTION__))  << "sqrt(mseGlobal)" << QString::number(sqrt(mseGlobal), 'g', 15);
    QList<QList<uint>> machines = bestGlobalSol.second;
    QPair<double, QList<QList<uint> > > bestNeighborState(bestGlobalSol);
    QList<uint> summedMachineCopy = summedMachinesGlobal;

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
                summedMachineCopy[i] -= jobOnHand;
                summedMachineCopy[l] += jobOnHand;
                //MSE
                double mse = calcMse(summedMachineCopy);
                QPair<double, QList<QList<uint> > > tempCurrentState;
                tempCurrentState.first = tf; tempCurrentState.second = machines;
                if( tf < bestNeighborState.first && mse < mseGlobal) {
                    mseGlobal = mse;
                    bestNeighborState = tempCurrentState;
                    if(!opt) return tempCurrentState;
                }
                else if( tf < bestNeighborState.first) {
                    mseGlobal = mse;
                    bestNeighborState = tempCurrentState;
                    if(!opt) return tempCurrentState;
                }
                else if(tf == bestNeighborState.first && mse < mseGlobal) {
                    mseGlobal = mse;
                    bestNeighborState = tempCurrentState;
                    if(!opt) return tempCurrentState;
                }
                machines[l].pop_back();
                summedMachineCopy[i] += jobOnHand;
                summedMachineCopy[l] -= jobOnHand;
            }
            machines[i][j] = jobOnHand;
        }
    }
    return bestNeighborState;
}

QPair<double, QList<QList<uint> > > LocalK2To10::move2jobs(const QPair<double, QList<QList<uint> > > bestGlobalSol, bool opt)
{
    QList<uint> summedMachineCopy = summedMachinesGlobal;
    if(DEBUGLEVELLOCAL >= 3) cout << qPrintable(QString(__FUNCTION__))<< "sqrt(mseGlobal)" << QString::number(sqrt(mseGlobal), 'g', 15);
    QList<QList<uint>> machines = bestGlobalSol.second;
    QPair<double, QList<QList<uint> > > bestNeighborState(bestGlobalSol);

    for(int i = 0; i < bestGlobalSol.second.size(); ++i) {
        QList<QPair<QPair<uint,uint>,QPair<uint,uint>>> sawThisJobs;
        for(int j = 0; j < machines[i].size(); ++j) {
            uint jobOnHand = machines[i][j];
            machines[i][j] = 0;
            summedMachineCopy[i] -= jobOnHand;
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
                    summedMachineCopy[i2] -= jobOnHand2;
                    for(int k = 0; k < bestGlobalSol.second.size(); ++k) {
                        machines[k].push_back(jobOnHand);
                        summedMachineCopy[k] += jobOnHand;
                        for(int l = 0; l < bestGlobalSol.second.size(); ++l) {
                            machines[l].push_back(jobOnHand2);
                            double tf (targetFunction(machines));
                            summedMachineCopy[l] += jobOnHand2;
                            //MSE
                            double mse = calcMse(summedMachineCopy);

                            if( tf < bestNeighborState.first && mse < mseGlobal) {
                                mseGlobal = mse;
                                bestNeighborState.first = tf;
                                bestNeighborState.second = machines;
                                if(!opt) return bestNeighborState;
                            }
                            else if( tf < bestNeighborState.first) {
                                mseGlobal = mse;
                                bestNeighborState.first = tf;
                                bestNeighborState.second = machines;
                                if(!opt) return bestNeighborState;
                            }
                            else if(tf == bestNeighborState.first && mse < mseGlobal) {
                                mseGlobal = mse;
                                bestNeighborState.first = tf;
                                bestNeighborState.second = machines;
                                if(!opt) return bestNeighborState;
                            }
                            machines[l].pop_back();
                            summedMachineCopy[l] -= jobOnHand2;
                        }
                        machines[k].pop_back();
                        summedMachineCopy[k] -= jobOnHand;
                    }
                    machines[i2][j2] = jobOnHand2;
                    summedMachineCopy[i2] += jobOnHand2;
                }
            }
            machines[i][j] = jobOnHand;
            summedMachineCopy[i] += jobOnHand;
        }
    }
    return bestNeighborState;
}

QPair<double, QList<QList<uint>>> LocalK2To10::swap1for1Optimal(const QPair<double, QList<QList<uint>>> bestGlobalSol, bool opt) {

    QList<uint> summedMachineCopy = summedMachinesGlobal;
    if(DEBUGLEVELLOCAL >= 3) cout << qPrintable(QString(__FUNCTION__)) << "sqrt(mseGlobal)" << QString::number(sqrt(mseGlobal), 'g', 15);
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


                    summedMachineCopy[i] = summedMachineCopy[i] - jobOnHand1 + jobOnHand2;
                    summedMachineCopy[l] = summedMachineCopy[l] - jobOnHand2 + jobOnHand1;
                    //MSE
                    double mse = 0;//calcMse(summedMachineCopy);

                    QPair<double, QList<QList<uint> > > tempCurrentState;
                    tempCurrentState.first = tf; tempCurrentState.second = machines;
                    if( tf < bestLocalSol.first && mse < mseGlobal) {
                        mseGlobal = mse;
                        bestLocalSol = tempCurrentState;
                        if(!opt) return tempCurrentState;
                    }
                    else if( tf < bestLocalSol.first) {
                        mseGlobal = mse;
                        bestLocalSol = tempCurrentState;
                        if(!opt) return tempCurrentState;
                    }
                    else if(tf == bestLocalSol.first && mse < mseGlobal) {
//                        mseGlobal = mse;
//                        bestLocalSol = tempCurrentState;
//                        if(!opt) return tempCurrentState;
                    }

                    summedMachineCopy[i] = summedMachineCopy[i] - jobOnHand2 + jobOnHand1;
                    summedMachineCopy[l] = summedMachineCopy[l] - jobOnHand1 + jobOnHand2;


                    machines[i][j] = jobOnHand1;
                    machines[l][k] = jobOnHand2;

                }
            }
        }
    }
    return bestLocalSol;
}

QPair<double, QList<QList<uint> > > LocalK2To10::swap2for1(const QPair<double, QList<QList<uint> > > bestGlobalSol, bool opt)
{
    QList<uint> summedMachineCopy = summedMachinesGlobal;
    if(DEBUGLEVELLOCAL >= 3) cout << qPrintable(QString(__FUNCTION__)) << "sqrt(mseGlobal)" << QString::number(sqrt(mseGlobal), 'g', 15);
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
                        summedMachineCopy[i] = summedMachineCopy[i] - jobOnHand1 - jobOnHand2 + jobOnHand3;
                        summedMachineCopy[l] = summedMachineCopy[l] + jobOnHand1 + jobOnHand2 - jobOnHand3;
                        //MSE
                        double mse = 0;//calcMse(summedMachineCopy);

                        if( tf < bestLocalSol.first && mse < mseGlobal) {
                            mseGlobal = mse;
                            bestLocalSol.first = tf;
                            machines[i].takeAt(j);
                            machines[l][k] = jobOnHand1;
                            machines[l] << jobOnHand2;
                            bestLocalSol.second = machines;
                            if(!opt) return bestLocalSol;
                        }
                        else if( tf < bestLocalSol.first) {
                            mseGlobal = mse;
                            bestLocalSol.first = tf;
                            machines[i].takeAt(j);
                            machines[l][k] = jobOnHand1;
                            machines[l] << jobOnHand2;
                            bestLocalSol.second = machines;
                            if(!opt) return bestLocalSol;
                        }
                        else if(tf == bestLocalSol.first && mse < mseGlobal) {
//                            mseGlobal = mse;
//                            bestLocalSol.first = tf;
//                            machines[i].takeAt(j);
//                            machines[l][k] = jobOnHand1;
//                            machines[l] << jobOnHand2;
//                            bestLocalSol.second = machines;
//                            if(!opt) return bestLocalSol;
                        }

                        machines[i][j] = jobOnHand1;
                        machines[i][r] = jobOnHand2;
                        machines[l][k] = jobOnHand3;
                        summedMachineCopy[i] = summedMachineCopy[i] + jobOnHand1 + jobOnHand2 - jobOnHand3;
                        summedMachineCopy[l] = summedMachineCopy[l] - jobOnHand1 - jobOnHand2 + jobOnHand3;
                    }
                }
            }
        }
    }
    return bestLocalSol;
}

QPair<double, QList<QList<uint> > > LocalK2To10::swap2for2(const QPair<double, QList<QList<uint> > > bestGlobalSol, bool opt)
{
    QList<uint> summedMachineCopy = summedMachinesGlobal;
    if(DEBUGLEVELLOCAL >= 3) cout << qPrintable(QString(__FUNCTION__))<< "sqrt(mseGlobal)" << QString::number(sqrt(mseGlobal), 'g', 15);
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

                            summedMachineCopy[i] = summedMachineCopy[i] - jobOnHand1 - jobOnHand2 + jobOnHand3 + jobOnHand4;
                            summedMachineCopy[l] = summedMachineCopy[l] + jobOnHand1 + jobOnHand2 - jobOnHand3 - jobOnHand4;
                            //MSE
                            double mse = calcMse(summedMachineCopy);

                            if( tf < bestLocalSol.first && mse < mseGlobal) {
                                mseGlobal = mse;
                                bestLocalSol.first = tf;
                                bestLocalSol.second = machines;
                                if(!opt) return bestLocalSol;
                            }
                            else if( tf < bestLocalSol.first) {
                                mseGlobal = mse;
                                bestLocalSol.first = tf;
                                bestLocalSol.second = machines;
                                if(!opt) return bestLocalSol;
                            }
                            else if(tf == bestLocalSol.first && mse < mseGlobal) {
                                mseGlobal = mse;
                                bestLocalSol.first = tf;
                                bestLocalSol.second = machines;
                                if(!opt) return bestLocalSol;
                            }

                            machines[i][j] = jobOnHand1;
                            machines[i][r] = jobOnHand2;
                            machines[l][k] = jobOnHand3;
                            machines[l][v] = jobOnHand4;

                            summedMachineCopy[i] = summedMachineCopy[i] + jobOnHand1 + jobOnHand2 - jobOnHand3 - jobOnHand4;
                            summedMachineCopy[l] = summedMachineCopy[l] - jobOnHand1 - jobOnHand2 + jobOnHand3 + jobOnHand4;

                        }
                    }
                }
            }
        }
    }
    return bestLocalSol;
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
    return (machinesNumber + maximalMachineTime );
}

double LocalK2To10::calcMse(QList<uint> summedMachine)
{
    double mse(0);
    for(const uint machineSum :summedMachine ){
        mse += (machineSum * machineSum);
    }
    return mse;
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

