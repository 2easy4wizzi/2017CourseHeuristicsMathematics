#include "LocalK2To10.h"

LocalK2To10::LocalK2To10(QList<uint> allJobs)
{
//    bestSolutionFound.first = 0;

    cout << allJobs;
}

//void LocalK2To10::runLpt(Node *node)
//{
//    QList<uint> remaning = node->jobsLeft;
//    QList<QList<uint>> machines = node->machines;

//    /*pre process*/
//    QList<uint> summedMachines;
//    for(const QList<uint>& m : machines){
//       uint localSum(0);
//       for(uint job : m){
//           localSum+=job;
//       }
//       summedMachines.append(localSum);
//    }

//    /**/
//    double lowestUpperfound(INF);
//    QList<QList<uint>> bestMachines;

//    for(int k = summedMachines.size(); k <= K_UPPER; k++){
//        QList<uint> localSummedMachines(summedMachines);
//        QList<QList<uint>> localMachines(machines);
//        QList<uint> localRemainng(remaning);

//        /*open emtpy machines (k - summedMachines.size())*/
//        for(int r=summedMachines.size(); r<k; ++r){
//            localSummedMachines.append(0);
//            localMachines.append(QList<uint>());
//        }

//        int startSize (localRemainng.size());
//        for (int i = 0; i < startSize; ++i) {
//            uint job (localRemainng.takeFirst());
//            uint ind2(0);
//            double localPotentialTarget(INF);
//            for (int j = 0; j < localSummedMachines.size(); ++j) {
//                //find heaviest
//                uint heaviestMachine(0);
//                for(int u = 0; u<localSummedMachines.size(); ++u){
//                    uint localSummedMachinesValue = (j==u) ? localSummedMachines[u] + job : localSummedMachines[u];
//                    if(localSummedMachinesValue > heaviestMachine){
//                        heaviestMachine = localSummedMachinesValue;
//                    }
//                }
//                uint potentialTarget(localSummedMachines.size() + heaviestMachine);
//                if(potentialTarget < localPotentialTarget){
//                    localPotentialTarget = potentialTarget;
//                    ind2 = j;
//                }
//            }
//            /*Add next to job to minimal machine*/
//            localMachines[ind2].append(job);
//            localSummedMachines[ind2] += job;
//        }
//        uint maxSum(0);
//        for(uint summedMachine: localSummedMachines) {
//            if(summedMachine > maxSum) {
//                maxSum = summedMachine;
//            }
//        }
//        double ithTargetFunc(maxSum + localSummedMachines.size());

//        if(ithTargetFunc < lowestUpperfound) {
//            lowestUpperfound = ithTargetFunc;
//            bestMachines = localMachines;
//        }
//    }

//    if(lowestUpperfound < bestSolutionFound.first){//found a new best
//        bestSolutionFound.first = lowestUpperfound;
//        bestSolutionFound.second = bestMachines;
//        if(DEBUGLEVEL >= 1){
//            Node* newBestSol = new Node(bestMachines, QList<uint>()); nodesSeenSoFar--;
//            newBestSol->U = lowestUpperfound;
//            cout << "       #########bestSolutionFound just got replaced"  << newBestSol->leafToString() << "#########";
//            if(newBestSol){
//                delete newBestSol;
//            }
//        }
//    }
//    node->U = lowestUpperfound;
//}
