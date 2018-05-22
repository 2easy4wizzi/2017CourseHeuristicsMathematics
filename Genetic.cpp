#include <windows.h>

#include "Genetic.h"
#define cout3 qDebug() << __LINE__
Genetic::Genetic(const uint &_populationSize, const uint &_generationsNumber, const uint &_machinesNumber, const QList<uint> &_allJobs, const uint &_debugLevel)
    :populationSize(_populationSize), generationsNumber(_generationsNumber), numberOfMachines(_machinesNumber), allJobs(_allJobs), debugLevel(_debugLevel), currentGenIndex(0)
{
    //TODO calc lower bound and assign the qmax -> lowerBound
    initFirstGeneration();
    runGenetic();

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
    debugPrint(toString(),3);
}

void Genetic::runGenetic()
{
    bool stop(false);
    for(uint i=0; i<generationsNumber && !stop; ++i){
        currentGenIndex = i;//used in toString
        if(i){//moving gen forward
            currentGen = nextGen;
            nextGen.clear();
            //TODO add here function that goes over all current gen and seeks improvment in the TF ->bestGeneFound
        }
//        stop = checkOptimumReached();//calc optimum TODO
        createNextGeneration();
    }
}

void Genetic::createNextGeneration()
{
    QList<QPair<Gene,float>> prob = buildProbabilityMap();
    QList<QPair<Gene, Gene>> parents = selectParents(prob);
    cout3 << parents.size();
    //TODO XO: go over all pairs -> create 2 children from each pair -> nextGen << child1 << child2

}

QList<QPair<Gene,float>> Genetic::buildProbabilityMap()
{
    QList<QPair<Gene,float>> probabilityPairs;
    double tfSum(0);

    for( Gene g : currentGen){
        tfSum += g.targetFunctionValue;
    }

    float totalProb(0);
    for( Gene g : currentGen){
        float geneFitness = 1 - (g.targetFunctionValue / tfSum);
        totalProb+=geneFitness;
        QPair<Gene,float> newPair;
        newPair.first = g;
        newPair.second = geneFitness;
        probabilityPairs << newPair;
    }

    for(int i = 0; i < probabilityPairs.size(); ++i){
        probabilityPairs[i].second /= totalProb;
    }

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
    QList<QString> elitizmOnceOnly;
    while(uint(parents.size()) <=  populationSize/2){
        Gene g1 = selectGeneByFitness(genesToProb,percentMapping);
        Gene g2 = selectGeneByFitness(genesToProb,percentMapping);
        if((g1.objectName!=g2.objectName) || (g1.objectName == g2.objectName && !elitizmOnceOnly.contains(g1.objectName))){
            QPair<Gene, Gene> p; p.first = g1; p.second=g2;
            parents.push_back(p);
        }
    }
    return parents;
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

Gene::Gene(QList<uint> _jobs, const uint &_numberOfMachines, QString _name): jobs(_jobs), numberOfMachines(_numberOfMachines), targetFunctionValue(0)
{
    objectName = QString("Gene id=%1 ").arg(_name);
    buildRandomGene();
    getTargetFunctionValue();
}

void Gene::buildRandomGene()
{
    for(int i = 0; i < jobs.size(); ++i) {
        content.push_back(getRandNumberG(1,numberOfMachines));
    }
}

QString Gene::toString() const
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


    QString str = format.arg(targetFunctionValue).arg(numberOfMachines).arg(jobsStr).arg(contentStr);
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

uint getRandNumberG(uint low, uint high )
{
   float prob = (float) qrand() / (RAND_MAX+1);
   double a = low + prob*(high-low);
   return uint(a);
}
