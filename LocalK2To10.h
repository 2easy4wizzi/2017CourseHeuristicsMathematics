#ifndef LOCALK2TO10_H
#define LOCALK2TO10_H
#define cout qDebug()

#include <QDebug>


class LocalK2To10
{
public:
    LocalK2To10(QList<uint> allJobs);
    QPair<double, QList<QList<uint>>> bestSolutionFound;
};

#endif // LOCALK2TO10_H
