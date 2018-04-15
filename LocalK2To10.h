#ifndef LOCALK2TO10_H
#define LOCALK2TO10_H

#include <QDebug>

#define cout qDebug() << __LINE__
#define INF 10000000000
#define QT_MAX_UINT 2000000000
#define DEBUGLEVEL 1 //0 for nothing, 1 for minimum, 2 for all
#define K_UPPER 10

class LocalK2To10
{
public:
    LocalK2To10(QList<uint> allJobs);
    QPair<double, QList<QList<uint>>> initFirstSol(QList<uint> allJobs);
    QPair<double, QList<QList<uint>>> runLpt(QList<uint> allJobs);
    void printSol(const QString &name, const QPair<double, QList<QList<uint> > > sol);

    QPair<double, QList<QList<uint>>> bestSolutionFound;
};

#endif // LOCALK2TO10_H
