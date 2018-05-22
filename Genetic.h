#ifndef GENETIC_H
#define GENETIC_H

#include <QDebug>
#include <QTime>
#include <QtMath>
#define cout qDebug()
#define INF 10000000000
#define QT_MAX_UINT 2000000000

uint getRandNumberG(uint low, uint high);

class Gene  {
public:
    Gene():targetFunctionValue(INF){}
    Gene(QList<uint> _jobs, const uint& _numberOfMachines, QString _name);
    void buildRandomGene();
    QString toString() const;
    /*Members*/
    QList<uint> content;
    QList<uint> jobs;
    uint numberOfMachines;
    double targetFunctionValue;
    QString objectName;

private:
    void getTargetFunctionValue();

private:

};


class Genetic
{
public:
    Genetic(const uint& _populationSize, const uint& _generationsNumber, const uint& _machinesNumber, const QList<uint>& _allJobs, const uint& _debugLevel);

    /*Create and assigns currentGeneration */
    void initFirstGeneration();

    void runGenetic();

    /*Using currentGen to create next assings result to nextGen */
    void createNextGeneration();

    /*Using currentGen to calculate probability to each gene*/
    QList<QPair<Gene, float> > buildProbabilityMap();

    Gene selectGeneByFitness(const QList<QPair<Gene,float>>& genesToProb, QList<uint> percentMapping);

    QList<QPair<Gene, Gene>> selectParents(const QList<QPair<Gene,float>>& genesToProb);

    QPair<Gene, Gene> crossOver(const Gene& g1, const Gene& g2);

    Gene mutate(const Gene& g);

    /*Aux Function*/
    float calcFitness(const Gene& gene);
    QString toString();
    void debugPrint(QString str, uint priority);

    /*Members*/
    QList<Gene> currentGen;
    QList<Gene> nextGen;
    Gene bestGeneFound;
    const uint populationSize;
    const uint generationsNumber;
    const uint numberOfMachines;
    const QList<uint> allJobs;
    /*Aux Members*/
    const uint debugLevel;
    uint currentGenIndex;
};


#endif // GENETIC_H
