#ifndef GENETIC_H
#define GENETIC_H
#include "Utils.h"


uint getRandNumberG(uint low, uint high);

class Gene  {
public:
    Gene():targetFunctionValue(INF){}
    Gene(QList<uint> _jobs, const uint& _numberOfMachines, QString _name);
    Gene(QList<uint> _jobs, const uint& _numberOfMachines, QString _name, QList<uint> _content);
    void buildRandomGene();
    bool operator ==(const Gene &b) const;
    bool operator !=(const Gene &b) const;
    QString toString() const;
    /*Members*/
    QList<uint> content;
    QList<uint> jobs;
    uint numberOfMachines;
    double targetFunctionValue;
    QString objectName;

private:
    void getTargetFunctionValue();

};


class Genetic
{
public:
    Genetic(const uint& _populationSize, const uint& _generationsNumber, const uint& _machinesNumber, const QList<uint>& _allJobs, const uint& _debugLevel, bool _specialGenes);

    /*Create and assigns currentGeneration */
    void initFirstGeneration();

    void createSpecialGenes();

    Gene createLptGene();
    Gene createBestFitGene();
    QList<uint> castMachinesToContent(QList<QList<uint>> localMachines);

    void runGenetic();

    /*Using currentGen to create next assings result to nextGen */
    void createNextGeneration();

    /*Using currentGen to calculate probability to each gene*/
    QList<QPair<Gene, float> > buildProbabilityMap(const QList<Gene>& parentsPool);

    Gene selectGeneByFitness(const QList<QPair<Gene,float>>& genesToProb, QList<uint> percentMapping);

    QList<QPair<Gene, Gene>> selectXOParents(const uint parentsPoolSize, const QList<QPair<Gene,float>>& genesToProb);

    QPair<Gene, Gene> crossOver(const Gene& g1, const Gene& g2, const uint &serialNumber);

    QList<Gene> doXOandMutate(const QList<QPair<Gene, Gene>>& parents, QList<Gene> mutations);

    QPair<QList<Gene>, QList<Gene>> splitMutationsParents();

    Gene mutate(const Gene& g, uint serialNumber);

    /*Aux Function*/
//    float calcFitness(const Gene& gene);
    QString toString();
    void debugPrint(QString str, uint priority);
    void calcLowerBound(const QList<uint>& allJobs);
    void checkIfBetterTfExistInNewGen();
    bool checkOptimumReached();

    /*Members*/
    QList<Gene> currentGen;
    QList<Gene> nextGen;
    Gene bestGeneFound;
    uint bestGeneFoundGenNumber;
    const uint populationSize;
    const uint generationsNumber;
    const uint numberOfMachines;
    const QList<uint> allJobs;
    uint mutationXORatio;
    /*Aux Members*/
    const uint debugLevel;
    uint currentGenIndex;
    uint lowerBound;
    static uint globalBestReplace; //TODO - TEMP var to see how much generation are effective
    static uint globalSumMutation; //TODO - TEMP var to see how much generation are effective
    bool specialGenes;
};


#endif // GENETIC_H
