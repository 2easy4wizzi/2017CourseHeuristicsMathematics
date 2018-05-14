#ifndef GENETIC_H
#define GENETIC_H

#include <QDebug>
#include <QTime>
#include <QtMath>
#define cout qDebug()
#define INF 10000000000
#define QT_MAX_UINT 2000000000

class Gene;

class Genetic
{
public:
    Genetic();
    Genetic(const uint& _populationSize, const uint& _generationsNumber, const uint& _machinesNumber, const QList<uint>& _allJobs, const uint& _debugLevel);

    /*Create and assigns currentGeneration */
    void initFirstGeneration();

    void runGenetic();

    /*Using currentGen to create next assings result to nextGen */
    void createNextGeneration();

    /*Using currentGen to calculate probability to each gene*/
    QMap<Gene, float> buildProbabilityMap();

    Gene selectGeneByFitness(const QMap<Gene, float>& genesToProb);

    QPair<Gene, Gene> crossOver(const Gene& g1, const Gene& g1);

    Gene mutate(const Gene& g);

    /*Aux Function*/
    float calcFitness(const Gene& gene);

    /*Members*/
    QList<Gene> currentGen;
    QList<Gene> nextGen;
    Gene fittestGene;
};

/*Q_OBJECT*/
class Gene {
    Gene(QList<uint> _content);
    /*Members*/
    QList<uint> content;
    double targetFunctionValue;
};

#endif // GENETIC_H
