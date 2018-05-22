#include <windows.h>

#include "Genetic.h"
#define cout3 qDebug() << __LINE__
Genetic::Genetic(const uint &_populationSize, const uint &_generationsNumber, const uint &_machinesNumber, const QList<uint> &_allJobs, const uint &_debugLevel)
    :populationSize(_populationSize), generationsNumber(_generationsNumber), numberOfMachines(_machinesNumber), allJobs(_allJobs), debugLevel(_debugLevel)
{
    initFirstGeneration();
    //pick fittestGene
}

void Genetic::initFirstGeneration()
{
    int specialGenes(0);//TODO LPT, BESTFIT, SAME MACHINE GENES
    for(uint i =0; i < populationSize - specialGenes; ++i) {
        Gene gene(allJobs, numberOfMachines, QString::number(i));
//        debugPrint(gene.toString(),3);
        currentGen << gene;
    }

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
        content.push_back(getRandNumber(1,numberOfMachines));
    }
}

QString Gene::toString()
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

uint Gene::getRandNumber(uint low, uint high )
{
   Sleep(100);//for rand to have different time
   qsrand(uint(QTime::currentTime().msec()));
   return (qrand() % ((high + 1) - low) + low);
}
