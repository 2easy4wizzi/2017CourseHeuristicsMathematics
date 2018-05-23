#include "Genetic.h"

Genetic::Genetic(const uint &_populationSize, const uint &_generationsNumber, const uint &_machinesNumber, const QList<uint> &_allJobs, const uint &_debugLevel)
    :populationSize(_populationSize), generationsNumber(_generationsNumber),
     numberOfMachines(_machinesNumber), allJobs(_allJobs),
     debugLevel(_debugLevel), currentGenIndex(0), lowerBound(0)
{
    if(allJobs.isEmpty()){
        debugPrint(QString("allJobs is empty - aborting Genetic H"),0);
    }
    else{
        debugPrint(QString("starting Genetic with args: generationsNumber=%1, populationSize=%2, machinesNumber=%3").arg(generationsNumber).arg(populationSize).arg(numberOfMachines),2);
        calcLowerBound(allJobs);
        initFirstGeneration();
        runGenetic();
        debugPrint(QString("**END Lower bound is: %1").arg(lowerBound),0);
        debugPrint(QString("**Best gene found: %1").arg(bestGeneFound.toString()),0);
    }

}

void Genetic::initFirstGeneration()
{
    int specialGenes(0);//TODO LPT, BESTFIT, SAME MACHINE GENES
    for(uint i =0; i < populationSize - specialGenes; ++i) {
        Gene gene(allJobs, numberOfMachines, QString::number(i));
        if(gene.targetFunctionValue < bestGeneFound.targetFunctionValue){
            bestGeneFound = gene;
        }
        currentGen << gene;
    }
    debugPrint(QString("Best gene from 1st generation: %1").arg(bestGeneFound.toString()),2);
}

void Genetic::runGenetic()
{

    bool stop(false);
    //TODO return this
//    bool stop(checkOptimumReached());
    for(uint i=0; i<generationsNumber && !stop; ++i){
        currentGenIndex = i+1;//used in toString
        if(i){//moving gen forward
            currentGen = nextGen;
            nextGen.clear();
            checkIfBetterTfExistInNewGen();
        }
        debugPrint(toString(),3);//prints ith+1 generation genes

//        stop = checkOptimumReached();//calc optimum TODO
        //TODO return
        if(!stop){
            createNextGeneration();
        }
    }
}

void Genetic::createNextGeneration()
{

    QList<QPair<Gene,float>> prob = buildProbabilityMap();
    QList<QPair<Gene, Gene>> parents = selectParents(prob);
    nextGen = doXOandMutate(parents);
}

QList<QPair<Gene,float>> Genetic::buildProbabilityMap()
{
    QList<QPair<Gene,float>> probabilityPairs;
    double tfSum(0);
    double maximalTf(0);
    double minimalTf(INF);

    for( Gene g : currentGen){
        if(g.targetFunctionValue < minimalTf)
            minimalTf = g.targetFunctionValue;
        if(g.targetFunctionValue > maximalTf)
            maximalTf = g.targetFunctionValue;



        tfSum += g.targetFunctionValue;
    }
    cout << "tfSum: " << tfSum;
    tfSum = maximalTf * currentGen.size() - tfSum;
    cout << "Normalized tfSum: " << tfSum;
    cout << "Maximal Tf: " << maximalTf;

    float totalProb(0);

    for( Gene g : currentGen){
        float geneFitness = (maximalTf-g.targetFunctionValue)/tfSum;
//        cout << maximalTf << g.targetFunctionValue << tfSum;
//        cout << geneFitness;
//        exit(0);

        totalProb+=geneFitness;
        QPair<Gene,float> newPair;
        newPair.first = g;
        newPair.second = geneFitness;
        probabilityPairs << newPair;
    }

    for(int i = 0; i < probabilityPairs.size(); ++i){
        probabilityPairs[i].second /= totalProb;
    }

    float sum(0);
    for(const QPair<Gene,float>& p : probabilityPairs){
        cout << p.first.toString() << p.second;
        sum+= p.second;
    }
    cout << sum;
    exit(0);
    return probabilityPairs;
}

Gene Genetic::selectGeneByFitness(const QList<QPair<Gene, float> > &genesToProb,QList<uint> percentMapping)
{
    uint rand = getRandNumberG(1,percentMapping.size());
    Gene selected = genesToProb[percentMapping[rand]].first;
    return selected;
}

QList<QPair<Gene, Gene> > Genetic::selectParents(const QList<QPair<Gene, float> > &genesToProb)
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
    while(uint(parents.size()) <  populationSize/2){
        Gene g1 = selectGeneByFitness(genesToProb,percentMapping);
        Gene g2 = selectGeneByFitness(genesToProb,percentMapping);
        if((g1 != g2) || (g1 == g2 && !elitizmOnceOnly.contains(g1))){
            QPair<Gene, Gene> p; p.first = g1; p.second=g2;
            parents.push_back(p);
            //TODO delete
            for(QPair<Gene, float> geneProb: genesToProb) {
                if(geneProb.first == p.first) {
//                    cout << geneProb.second <<  p.first.toString();
                    for(QPair<Gene, float> geneProb: genesToProb) {
//                        if(geneProb.first.objectName == p.second.objectName) {
//                            cout << geneProb.second << p.second.toString();
//                            cout << "--";
//                        }
                    }
                }
            }
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

QList<Gene> Genetic::doXOandMutate(const QList<QPair<Gene, Gene> > &parents)
{
    QList<Gene> nextGenLocal;
    uint i(0);
    for(const QPair<Gene, Gene>& p: parents) {
        QPair<Gene, Gene> children = crossOver(p.first, p.second, i);
        i += 2;
        if(nextGenLocal.contains(children.first) || nextGenLocal.contains(children.second)){
            //TODO - if converge fast add here fixed re attempts of XO
        }
        nextGenLocal << children.first << children.second;
    }

//    currentGen = nextGenLocal;
//    debugPrint(toString(), 3);
    return nextGenLocal;
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

    lowerBound = qMax( qMax(perfectSplit,pMax), pigeonholePrinciple );

    debugPrint(QString("LowerBounds calc: perfectSplit=%1, pMax=%2, pigeonholePrinciple=%3 => lowerBound set to %4").arg(perfectSplit).arg(pMax).arg(pigeonholePrinciple).arg(lowerBound),2);
}

void Genetic::checkIfBetterTfExistInNewGen()
{
    for(const Gene& g: currentGen){
        if(g.targetFunctionValue < bestGeneFound.targetFunctionValue){
            bestGeneFound = g;
            debugPrint(QString("Best gene got replaced in generation %1. Gene details: %2").arg(currentGenIndex).arg(bestGeneFound.toString()),1);
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

QString Gene::toString() const //TODO - remove jobs and content - add summed Machine conetent
{
    QString format = QString("TF: %1, #Machines: %2 Jobs: %3 Content: %4");
    format.prepend(objectName);


    QString jobsStr("<");
    int i(0);
    for(const uint& job : jobs){
        if (i++) jobsStr.append(",");
        jobsStr += (QString::number(job));
    }
    jobsStr.append(">");

    QString contentStr("<");
    i = 0;
    for(const uint& machineIndex : content){
        if (i++) contentStr.append(",");
        contentStr += (QString::number(machineIndex));
    }
    contentStr.append(">");


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
    for(uint i = 0; i < numberOfMachines; ++i) {
        QString mstr = QString("bucket%1 sum:%2 content= (").arg(i+1).arg(summedMachines.at(i));
        if(machines.at(i).isEmpty()){
            mstr.append("-)| ");
            machinesContent.append(mstr);
        }
        else{
            for(uint j : machines.at(i)){
                mstr += QString("%1, ").arg(j);
            }
            mstr = mstr.mid(0,mstr.size()-2);
            machinesContent.append(QString("%1)| ").arg(mstr));
        }
    }
    machinesContent = machinesContent.mid(0,machinesContent.size()-2);
    //TODO -TEMP
    jobsStr ="";
    contentStr ="";
    machinesContent ="";
    //TEMP

    QString str = format.arg(targetFunctionValue).arg(numberOfMachines).arg(jobsStr).arg(contentStr);
    str+=machinesContent;
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


