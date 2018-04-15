#include "LocalK2To10.h"

LocalK2To10::LocalK2To10(QList<uint> allJobs)
{
    bestSolutionFound = initFirstSol(allJobs);//done - continue from here
    //change names: from LocalK2To10 to LocalKnotBiggerThen10
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

