#include "Genetic.h"
//TODO - check num of mutaions - maybe do minium mutations
uint Genetic::globalBestReplace=0;
uint Genetic::globalSumMutation=0;

Genetic::Genetic(const uint &_populationSize, const uint &_generationsNumber, const uint &_machinesNumber, const QList<uint> &_allJobs, const uint &_debugLevel, bool _specialGenes)
    :bestGeneFoundGenNumber(0), populationSize(_populationSize),  generationsNumber(_generationsNumber),
     numberOfMachines(_machinesNumber), allJobs(_allJobs),mutationXORatio(0.2),
     debugLevel(_debugLevel), currentGenIndex(0), lowerBound(0), specialGenes(_specialGenes)
{
    if(allJobs.isEmpty()){
        debugPrint(QString("allJobs is empty - aborting Genetic H"),0);
    }
    else{
        debugPrint(QString("starting Genetic with args: generationsNumber=%1, populationSize=%2, machinesNumber=%3").arg(generationsNumber).arg(populationSize).arg(numberOfMachines),2);
        calcLowerBound(allJobs);
        initFirstGeneration();
        runGenetic();
        debugPrint(QString("**END On Gen=%1").arg(bestGeneFoundGenNumber),0);
        debugPrint(QString("**Best gene found: %1").arg(bestGeneFound.toString()),0);
    }

}

void Genetic::initFirstGeneration()
{

    int specialGenesNumber = specialGenes? 2 : 0 ;//TODO LPT, BESTFIT, SAME MACHINE GENES
    for(uint i =0; i < populationSize - specialGenesNumber; ++i) {
        Gene gene(allJobs, numberOfMachines, QString::number(i));
        if(gene.targetFunctionValue < bestGeneFound.targetFunctionValue){
            bestGeneFound = gene;
            bestGeneFoundGenNumber = 0;
        }
        currentGen << gene;
    }
    if(specialGenes){
        createSpecialGenes();
    }

//    checkIfBetterTfExistInNewGen();
    debugPrint(QString("**Best gene from 1st generation: %1").arg(bestGeneFound.toString()),2);
}

void Genetic::createSpecialGenes()
{
    Gene Lpt = createLptGene();
    Gene BestFit = createBestFitGene();
    if(Lpt.targetFunctionValue < bestGeneFound.targetFunctionValue){
        bestGeneFound = Lpt;
        bestGeneFoundGenNumber = 0;
    }
    if(BestFit.targetFunctionValue < bestGeneFound.targetFunctionValue){
        bestGeneFound = BestFit;
        bestGeneFoundGenNumber = 0;
    }
    currentGen << Lpt << BestFit;
}

Gene Genetic::createLptGene()
{
    QPair<double, QList<QList<uint> > > lptFirstSol;
    lptFirstSol.first = INF;

    //for every K<10
    int k = numberOfMachines;

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

    QList<uint> content = castMachinesToContent(localMachines);
    Gene lpt(allJobs, numberOfMachines, QString::number(populationSize-2), content);
    return lpt;
}

Gene Genetic::createBestFitGene()
{
    QList<uint> allJobsCopy = allJobs;

    double bestLow = lowerBound;

    QPair<double, QList<QList<uint> > > sol;
    QList<QList<uint>> machines;

    for(uint i=0; i<numberOfMachines; ++i){
        uint currentSum(0);
        QList<uint> machine;
        QList<int> usedJobs;
        for(int j=0; j<allJobsCopy.size(); ++j){
            if(currentSum+allJobsCopy.at(j) <= bestLow){
                machine << allJobsCopy.at(j);
                currentSum += allJobsCopy.at(j);
                usedJobs <<j;
            }
        }
        machines << machine;
        QList<uint> newAllJob;
        for(int k=0; k<allJobsCopy.size(); ++k){
            if(!usedJobs.contains(k)){
                newAllJob << allJobsCopy.at(k);
            }
        }
        allJobsCopy = newAllJob;
    }
    while(!allJobsCopy.isEmpty()){
        machines[0].append(allJobsCopy.takeAt(0));
    }

    QList<uint> content = castMachinesToContent(machines);
    Gene bestFit(allJobs, numberOfMachines, QString::number(populationSize-1), content);

    return bestFit;
}

QList<uint> Genetic::castMachinesToContent(QList<QList<uint> > localMachines)
{
    QList<uint> content;
    for(int k=0; k<allJobs.size(); ++k){
        content.append(0);
    }
    for(int k=0; k<allJobs.size(); ++k){
        bool found = false;
        for(int i=0; i<localMachines.size() && !found; ++i){
            QList<uint> m = localMachines.at(i);
            for(int j=0; j<m.size() && !found; ++j){
                uint jobIter = m.at(j);
                if(jobIter == allJobs.at(k)){
                    m.takeAt(j);
                    content[k] = i+1;
                    found = true;
                }
            }
            if(found){
                localMachines[i] = m;
            }
        }
    }
    return content;
}

void Genetic::runGenetic()
{
    bool stop(checkOptimumReached());
    for(uint i=0; i<generationsNumber && !stop; ++i){
        currentGenIndex = i+1;//used in toString
        if(i){//moving gen forward
            currentGen = nextGen;
            nextGen.clear();
            checkIfBetterTfExistInNewGen();
        }

        debugPrint(toString(),3);//prints ith+1 generation genes

        stop = checkOptimumReached();
        if(i % 10 == 0){
            debugPrint(toString(), 2);
        }

        if(!stop){
            createNextGeneration();
        }
    }
    currentGen = nextGen;
    debugPrint(toString(), 2);

}

void Genetic::createNextGeneration()
{
    QPair<QList<Gene>, QList<Gene>> mutationParentSplit = splitMutationsParents();

    QList<Gene> mutations = mutationParentSplit.first;
    QList<Gene> parentsPool = mutationParentSplit.second;
//    cout << "XX: "<< parentsPool.size() << mutations.size();
    QList<QPair<Gene,float>> prob = buildProbabilityMap(parentsPool);
    QList<QPair<Gene, Gene>> parents = selectXOParents(parentsPool.size() ,prob);
    nextGen = doXOandMutate(parents, mutations);
}

QList<QPair<Gene,float>> Genetic::buildProbabilityMap(const QList<Gene> &parentsPool)
{
    QList<QPair<Gene,float>> probabilityPairs;
    double tfSum(0);
    double maximalTf(0);
    double minimalTf(INF);

    for( Gene g : parentsPool){
        if(g.targetFunctionValue < minimalTf){
            minimalTf = g.targetFunctionValue;
        }
        if(g.targetFunctionValue > maximalTf){
            maximalTf = g.targetFunctionValue;
        }
        tfSum += g.targetFunctionValue;
    }
//    cout << "tfSum: " << tfSum;
    tfSum = maximalTf * parentsPool.size() - tfSum;
//    cout << "Normalized tfSum: " << tfSum;
//    cout << "Maximal Tf: " << maximalTf;

    float totalProb(0);

    for( Gene g : parentsPool){
        float geneFitness = (maximalTf-g.targetFunctionValue)/tfSum;
//        cout << maximalTf << g.targetFunctionValue << tfSum;
//        cout << maximalTf;
//        cout << g.targetFunctionValue;
//        exit(0);
        QPair<Gene,float> newPair;
        newPair.first = g;
        if(geneFitness != geneFitness){//geneFitness is nan
//            cout << (geneFitness != geneFitness) << "nan happened";
            newPair.second = 0;
        }
        else{
            newPair.second = geneFitness;
            totalProb+=geneFitness;
        }
        probabilityPairs << newPair;
    }

    if(totalProb > 0){
        for(int i = 0; i < probabilityPairs.size(); ++i){
            probabilityPairs[i].second /= totalProb;
        }
    }
    else{
        for(int i = 0; i < probabilityPairs.size(); ++i){
            probabilityPairs[i].second = 1/float(probabilityPairs.size());
        }
    }

//    for(int i = 0; i < probabilityPairs.size(); ++i){
//        cout << totalProb<<probabilityPairs[i].second;
//    }
//    float sum(0);
//    for(const QPair<Gene,float>& p : probabilityPairs){
//        cout << p.first.toString() << p.second;
//        sum+= p.second;
//    }
//    cout << sum;
    return probabilityPairs;
}

Gene Genetic::selectGeneByFitness(const QList<QPair<Gene, float> > &genesToProb,QList<uint> percentMapping)
{
    uint rand = getRandNumberG(1,percentMapping.size());
    Gene selected = genesToProb[percentMapping[rand]].first;
    return selected;
}

QList<QPair<Gene, Gene> > Genetic::selectXOParents(const uint parentsPoolSize, const QList<QPair<Gene, float> > &genesToProb)
{
    QList<uint> percentMapping;
    uint precision(1000000);
    for(int i = 0; i < genesToProb.size(); ++i){
        int bucketSize = genesToProb[i].second * precision;
        for(int j=0; j<bucketSize; ++j){
            percentMapping.append(i);
        }
    }
    QList<QPair<Gene, Gene> > parents;
    QList<Gene> elitizmOnceOnly;
    while(uint(parents.size()) <  parentsPoolSize/2){

        Gene g1 = selectGeneByFitness(genesToProb,percentMapping);

        Gene g2 = selectGeneByFitness(genesToProb,percentMapping);

        if((g1 != g2) || (g1 == g2 && !elitizmOnceOnly.contains(g1))){
            QPair<Gene, Gene> p; p.first = g1; p.second=g2;
            parents.push_back(p);
        }
    }
    return parents;
}

QPair<Gene, Gene> Genetic::crossOver(const Gene &g1, const Gene &g2, const uint& serialNumber)
{
    uint index = getRandNumberG(1,g1.content.size());
    uint contentSize = g2.content.size();

    QList<uint> content1 = g1.content.mid(0,index) << g2.content.mid(index, contentSize-index);
    QList<uint> content2 = g2.content.mid(0,index) << g1.content.mid(index, contentSize-index);
    Gene child1(allJobs, numberOfMachines, QString::number(serialNumber), content1);
    Gene child2(allJobs, numberOfMachines, QString::number(serialNumber+1), content2);

    QPair<Gene, Gene> children(child1, child2);
    return children;
}

QList<Gene> Genetic::doXOandMutate(const QList<QPair<Gene, Gene> > &parents, QList<Gene> mutations)
{
    QList<Gene> nextGenLocal;
    uint i(0);
    for(const QPair<Gene, Gene>& p: parents) {
        QPair<Gene, Gene> children = crossOver(p.first, p.second, i);
        i += 2;
        nextGenLocal << children.first << children.second;
    }
    globalSumMutation+=mutations.size();
    for(const Gene& g: mutations) {
        Gene mutated = mutate(g,i++);
        nextGenLocal.append(mutated);
    }
    return nextGenLocal;
}

QPair<QList<Gene>, QList<Gene> > Genetic::splitMutationsParents()
{
    QPair<QList<Gene>, QList<Gene> > result;
    QList<Gene> mutations;
    QList<Gene> XO;
    for(const Gene& g: currentGen) {
        if(XO.contains(g)) {
            mutations.append(g);
        }
        else {
            XO.append(g);
        }
    }
    if(mutations.size() % 2 == 1) {
        Gene g = mutations.takeFirst();
        XO.push_back(g);
    }
    result.first = mutations;
    result.second = XO;
    return result;
}

Gene Genetic::mutate(const Gene &g, uint serialNumber)
{
    const float precent(mutationXORatio);
    QList<uint> content = g.content;
    uint amountOfBitsToChange(qFloor(content.size() * precent + 0.1));
    //Generate Indicies
    QList<uint> indicies;
    QList<uint> indiciesToChange;

    for (int i = 0; i < content.size(); ++i) {
        indicies << i;
    }
    for (uint i = 0; i < amountOfBitsToChange; ++i) {
        uint choosenIndex = getRandNumberG(0,indicies.size());
        indiciesToChange << indicies.takeAt(choosenIndex);
    }

//    cout << indiciesToChange;
//    cout <<"old tf and content"<< g.objectName << g.targetFunctionValue << content ;
    for(const uint& index: indiciesToChange) {
        uint value;
        uint j(0);
        do{
            value = getRandNumberG(1, numberOfMachines+1);
        }
        while(value == content[index] || j++>5);
        content[index] = value;
    }

    //Gene(QList<uint> _jobs, const uint& _numberOfMachines, QString _name, QList<uint> _content);
    Gene mutated(allJobs, numberOfMachines, QString("*%1").arg(serialNumber), content);
    //3,1,5,1,2,5,4,3,4,4 - 1946
//    Gene temp(allJobs, numberOfMachines, QString("*%1--").arg(serialNumber), QList<uint>() << 3<<1<<5<<1<<2<<5<<4<<3<<4<<4);
//    cout <<"new tf and content" << mutated.objectName<< mutated.targetFunctionValue << content ;
//    cout <<"TEMP new tf and content" << temp.objectName<< temp.targetFunctionValue << temp.content ;
    return mutated;
}

QString Genetic::toString()
{
    QString currentGenStr = QString("Generation %1:").arg(currentGenIndex);
    for(Gene g : currentGen){
        currentGenStr.append("\n\t");
        currentGenStr.append(g.toString());
    }
    return currentGenStr;
}

void Genetic::debugPrint(QString str, uint priority)
{
    if(priority <= debugLevel)
        qDebug() << qPrintable(str);
}

void Genetic::calcLowerBound(const QList<uint>& allJobs)
{
    double sumAllJobs(0);
    for(const uint& job : allJobs){sumAllJobs += job;}

    //global lower 1 - if buckets could be filled perfectly...
    double perfectSplit = (sumAllJobs/double(numberOfMachines));

    //global lower 2 - add at least 1 machine with the heaviest job (Jobs are sorted from high to low, so take the first)
    double pMax = allJobs.first();

    //global lower 3 - pigeonholePrinciple - Jobs are sorted so we can take from the tails n jobs
    //for example - 3 jobs to 2 machines => we know there is a machine with atleast 2 smallest jobs.
    //jobs=(2,5,3) and 2 machines => at the very least there is a machine with jobs 2 and 3 in it.
    uint w = qCeil(double(allJobs.size()) / double(numberOfMachines));
    double pigeonholePrinciple(0);
    for (uint j = 0; j < w; ++j) {
        pigeonholePrinciple += allJobs[allJobs.size() - j - 1];
    }

    lowerBound = qCeil(qMax( qMax(perfectSplit,pMax), pigeonholePrinciple ));

    debugPrint(QString("LowerBounds calc: perfectSplit=%1, pMax=%2, pigeonholePrinciple=%3 => lowerBound set to %4").arg(perfectSplit).arg(pMax).arg(pigeonholePrinciple).arg(lowerBound),2);
}

void Genetic::checkIfBetterTfExistInNewGen()
{
    for(const Gene& g: currentGen){
        if(g.targetFunctionValue < bestGeneFound.targetFunctionValue){
            bestGeneFound = g;
            bestGeneFoundGenNumber = currentGenIndex;
            debugPrint(QString("**Best gene got replaced in generation %1. Gene details: %2").arg(currentGenIndex).arg(bestGeneFound.toString()),1);
            if(currentGenIndex > globalBestReplace){
                globalBestReplace = currentGenIndex;
            }
        }
    }
}

bool Genetic::checkOptimumReached()
{
    bool optimumReached((bestGeneFound.targetFunctionValue <= lowerBound));
    if(optimumReached)
        debugPrint(QString("Best gene target function value is equal to lower bound. stopping Genetic H"),1);
    return optimumReached;
}

Gene::Gene(QList<uint> _jobs, const uint &_numberOfMachines, QString _name, QList<uint> _content)
{
    jobs = _jobs;
    numberOfMachines = _numberOfMachines;
    content = _content;
    objectName = QString("Gene id=%1 ").arg(_name);
    targetFunctionValue = 0;
    getTargetFunctionValue();
}

Gene::Gene(QList<uint> _jobs, const uint &_numberOfMachines, QString _name): jobs(_jobs), numberOfMachines(_numberOfMachines), targetFunctionValue(0)
{
    objectName = QString("Gene id=%1 ").arg(_name);
    buildRandomGene();
    getTargetFunctionValue();
}

void Gene::buildRandomGene()
{
    for(int i = 0; i < jobs.size(); ++i) {
        content.push_back(getRandNumberG(1,numberOfMachines+1));
    }
}


bool Gene::operator ==(const Gene &b) const
{
    bool retValue = this->content == b.content;
    return retValue;
}

bool Gene::operator !=(const Gene &b) const
{
    return !(this->content == b.content);
}

QString Gene::toString() const
{
    QString format = QString("%1 TF=%2, #Machines=%3, content:%4, %5");//NAME, TF, #Machines, content(cromozom), content of solution(machines)

    QString contentStr("<");
    int i = 0;
    for(const uint& machineIndex : content){
        if (i++) contentStr.append(",");
        contentStr += (QString::number(machineIndex));
    }
    contentStr.append(">");
    if(jobs.size() >10){
        contentStr = "Very long...";
    }


    QList<QList<uint>> machines;
    QList<uint> summedMachines;
    for(uint i = 0; i < numberOfMachines; ++i) {
        machines << QList<uint>();
        summedMachines << 0;
    }
    for(int i = 0; i < content.size(); ++i) {
        machines[content.at(i)-1].append(jobs.at(i));
        summedMachines[content.at(i)-1] += jobs.at(i);
    }
    QString machinesContent;
    if(jobs.size() <=10 && 0){
        for(uint i = 0; i < numberOfMachines; ++i) {
            QString mstr = QString("\n\t\tbucket%1 sum:%2 content= (").arg(i+1).arg(summedMachines.at(i));
            if(machines.at(i).isEmpty()){
                mstr.append("-)| ");
                machinesContent.append(mstr);
            }
            else{
                for(uint j : machines.at(i)){
                    mstr += QString("%1, ").arg(j);
                }
                mstr = mstr.mid(0,mstr.size()-2);
                machinesContent.append(QString("%1) ").arg(mstr));
            }
        }
        machinesContent = machinesContent.mid(0,machinesContent.size()-2);
    }
    else{
        QString mstr;
        for(uint sum : summedMachines){
           mstr.append(QString("%1,").arg(sum));
        }
        mstr = mstr.mid(0,mstr.size()-1);
        machinesContent = QString("Summed Machines(%1)").arg(mstr);
    }

    QString str = format.arg(objectName).arg(targetFunctionValue).arg(numberOfMachines).arg(contentStr).arg(machinesContent);
    return (str);
}

void Gene::getTargetFunctionValue()
{
    QList<uint> summedMachines;
    for(uint i = 0; i < numberOfMachines; ++i) {
        summedMachines << 0;
    }
    for(int i = 0; i < jobs.size(); ++i) {
        uint machineIndex = content.at(i);
        uint currentJob = jobs.at(i);
        summedMachines[machineIndex-1] += currentJob;
    }
    for(uint i = 0; i < numberOfMachines; ++i) {
        if(summedMachines[i] > targetFunctionValue)
            targetFunctionValue = summedMachines[i];
    }
}


