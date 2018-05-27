#include "Utils.h"
#include "BandBK2To10.h"
#include "LocalK2To10.h"
#include "Genetic.h"

static QMap<uint, QList<uint>> inputsMap;
void initInputs();
const QList<uint> getInput(int inputBatch);
QList<uint> shuffleMyQLIST(QList<uint> &input3rand);
uint getRandNumber(uint low, uint high );
void runOnAllFolder();
QList<QPair<QString, QString> > getAllTestsNames();
const QList<uint> getInputFromFile(QPair<QString,QString> inputToSol, double& tf, int& numberOfMachines, double &globalLowerBound);
QList<uint> parseFiles(QPair<QString,QString> inputToSol, double& tf, int &numberOfMachines, double &globalLowerBound);
void runLocalSearch(QList<QPair<QString,QString>> inputToSol);
void runGenetic(QList<QPair<QString,QString>> inputToSol, uint _populationSize, uint _generationsNumber, uint _debugLevel, bool specialGenes, int attempts, const float &_mutationPercentage, const float &_genesMutationPercentage, const uint &_fitnessFunctionIndex);
QList<QPair<QString,QString>> getInputByDemand(QString dist, int range, int jobs, int numMachines, QList<QPair<QString, QString> > inputToSol);
QList<QPair<QString,QString>> getInputByNames(QStringList names, QList<QPair<QString, QString>> inputToSol);
void runBNB(QList<QPair<QString,QString>> inputToSol);

int main(int argc, char *argv[])
{
    qsrand(uint(QTime::currentTime().msec()));//keep this line to init seed for qrand()
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    initInputs();
    QList<QPair<QString,QString>> inputToSol = getAllTestsNames();

/*bnb*/
//    //all 10 jobs tasks
//    QList<QPair<QString,QString>> all10Jobs = getInputByDemand("", -1, 10, -1, inputToSol);
//    runBNB(all10Jobs);

//    //all Uniform 50 jobs tasks
//    QList<QPair<QString,QString>> all50Jobs = getInputByDemand("U", -1, 50, -1, inputToSol);
//    runBNB(all50Jobs);

//    //by hardcode name
//    QStringList namesBnb = (QStringList() << "U_1_0050_05_0.txt");
//    QList<QPair<QString,QString>> namedTasksBnb = getInputByNames(namesBnb, inputToSol);
//    runBNB(namedTasksBnb);

//    //by hardcode manual input
//    QList<QPair<QString,QString>> costumTasksBnb = getInputByDemand("U", 5, -1, -1, inputToSol);
//    cout << costumTasksBnb;
//    runBNB(costumTasksBnb);

/*local*/
//    //all Uniform - 130
//    QList<QPair<QString,QString>> uniformTasksLocal = getInputByDemand("U", 1, -1, -1, inputToSol);
//    runLocalSearch(uniformTasksLocal);

//    //all NON-Uniform - 130
//    QList<QPair<QString,QString>> nonUniformsTasksLocal = getInputByDemand("NU", 1, -1, -1, inputToSol);
//    cout << nonUniformsTasksLocal;
//    runLocalSearch(nonUniformsTasksLocal);

//    //by hardcode name
//    QStringList namesLocal = (QStringList() << "NU_1_0500_25_3.txt");
//    QList<QPair<QString,QString>> namedTasksLocal = getInputByNames(namesLocal, inputToSol);
//    runLocalSearch(namedTasksLocal);

/*Genetic*/
//    //by hardcode name
//    QStringList namesGenetic = (QStringList() << "NU_1_0500_25_3.txt");
//    QList<QPair<QString,QString>> namedTasksGenetic = getInputByNames(namesGenetic, inputToSol);
//    const uint& _populationSize(100);
//    const uint& _generationsNumber(3);
//    const uint& _debugLevel(3);
//    runGenetic(namedTasksGenetic,_populationSize, _generationsNumber, _debugLevel );

//    //by manual input
//    const uint& _populationSize(100);
//    const uint& _generationsNumber(100);
//    const uint& _machinesNumber(5);
//    QList<uint> allJobs = getInput(38);
//    Genetic g(_populationSize, _generationsNumber, _machinesNumber, allJobs,_debugLevel);

//    //first 10 tasks of 1000 jobs
//    QList<QPair<QString,QString>> allTasks1000Jobs = getInputByDemand("U", -1, 1000, -1, inputToSol);
//    QList<QPair<QString,QString>> first10Tasks1000Jobs;
//    for(int i=0; i<10; i++){ first10Tasks1000Jobs.push_back(allTasks1000Jobs.at(i)); }
//    bool specialGenes(false);
//    const uint& _populationSize(100);
//    const uint& _generationsNumber(100);
//    const uint& _debugLevel(3);
//    const uint& _attempts(1);
//    const float& _mutationPercentage(0.02);
//    const float& _genesMutationPercentage(0.02);
//    const uint& _fitnessFunctionIndex(0);
//    runGenetic(first10Tasks1000Jobs,_populationSize, _generationsNumber, _debugLevel,specialGenes,_attempts, _mutationPercentage, _genesMutationPercentage, _fitnessFunctionIndex);

//    //first 10 jobs task
//    QList<QPair<QString,QString>> all10Jobs = getInputByDemand("U", -1, 10, -1, inputToSol);
//    bool specialGenes(false);
//    const uint& _populationSize(100);
//    const uint& _generationsNumber(100);
//    const uint& _debugLevel(0);
//    const uint& _attempts(1);
//    const float& _mutationPercentage(0.02);
//    const float& _genesMutationPercentage(0.02);
//    const uint& _fitnessFunctionIndex(2);
//    runGenetic(QList<QPair<QString,QString>>()<<all10Jobs.first(),_populationSize, _generationsNumber, _debugLevel,specialGenes,_attempts, _mutationPercentage, _genesMutationPercentage, _fitnessFunctionIndex);

    //first 1000 jobs task
    QList<QPair<QString,QString>> all1000Jobs = getInputByDemand("U", -1, 1000, -1, inputToSol);
    QList<QPair<QString,QString>> first1000Job = QList<QPair<QString,QString>>() << all1000Jobs.first();
    bool specialGenes(false);
    const uint& _populationSize(100);
    const uint& _generationsNumber(100);
    const uint& _debugLevel(0);
    const uint& _attempts(1);
    const float& _mutationPercentage(0.02);
    const float& _genesMutationPercentage(0.02);
//    const uint& _fitnessFunctionIndex(3);
//    runGenetic(first1000Job,_populationSize, _generationsNumber, _debugLevel,specialGenes,_attempts, _mutationPercentage, _genesMutationPercentage, _fitnessFunctionIndex);
    runGenetic(first1000Job,_populationSize, _generationsNumber, _debugLevel,specialGenes,_attempts, _mutationPercentage, _genesMutationPercentage, 1);
    runGenetic(first1000Job,_populationSize, _generationsNumber, _debugLevel,specialGenes,_attempts, _mutationPercentage, _genesMutationPercentage, 2);
    runGenetic(first1000Job,_populationSize, _generationsNumber, _debugLevel,specialGenes,_attempts, _mutationPercentage, _genesMutationPercentage, 3);
    runGenetic(first1000Job,_populationSize, _generationsNumber, _debugLevel,specialGenes,_attempts, _mutationPercentage, _genesMutationPercentage, 4);
    runGenetic(first1000Job,_populationSize, _generationsNumber, _debugLevel,specialGenes,_attempts, _mutationPercentage, _genesMutationPercentage, 5);


    return 0;
}

QList<QPair<QString,QString>> getInputByNames(QStringList names, QList<QPair<QString, QString> > inputToSol){
    QList<QPair<QString,QString>> newInputs;
    for(QPair<QString,QString> p : inputToSol){
        QString baseName = p.first.split("/").back();
        if(names.contains(baseName)){
            newInputs.push_back(p);
        }
    }
//    cout << newInputs;
    return newInputs;
}

QList<QPair<QString,QString>> getInputByDemand(QString dist, int range, int jobs, int numMachines, QList<QPair<QString,QString>> inputToSol){
    QList<QPair<QString,QString>> newInputs;
    for(QPair<QString,QString> p : inputToSol){
        QString baseName = p.first.split("/").back();
        QStringList splited = baseName.split("_");
        bool take(true);
        take = take * (dist == splited.at(0) || dist=="");
        QString s1 (splited.at(1));
        take = take * (range == s1.toInt() || range==-1);
        QString s2 (splited.at(2));
        take = take * (jobs == s2.toInt() || jobs==-1);
        QString s3 (splited.at(3));
        take = take * (numMachines == s3.toInt() || numMachines==-1);

        if(take){
            newInputs.push_back(p);
        }
    }
//    cout << QString("number of input=%1. distribution=%2. range=%3 #jobs=%4. #machines=%5").arg(newInputs.size()).arg(dist==""?"all":dist).arg(range==-1?"all":QString("[1, %1]").arg(qPow(10,range+1))).arg(jobs==-1?"all":QString::number(jobs)).arg(numMachines==-1?"all":QString::number(numMachines));
    return newInputs;
}
void runBNB(QList<QPair<QString,QString>> inputToSol) {

    QMap<int,int> good;
    QMap<int,int> bad;
    double std(0);
    QTime timerTotal; timerTotal.start();
    for (int i = 0; i < inputToSol.size(); ++i) {
        cout << QString("--------------------START %1 from %2--------------------------------").arg(i+1).arg(inputToSol.size());
        QTime timer; timer.start();
        QPair<QString,QString> inputToSolPair = inputToSol.at(i);
        cout << QString("input file number %1: inputName=%2 and solutionName=%3").arg(i+1).arg(inputToSolPair.first).arg(inputToSolPair.second);
        double tf(0); int numberOfMachines(0); double u;
        const QList<uint> allJobs = getInputFromFile(inputToSolPair, tf, numberOfMachines,u); //getting jobs from input file, printing data from files(input and sol file) and taking the upperBound as targer function(tf)

        BandBK2To10* bnb = new BandBK2To10(allJobs, numberOfMachines);

        cout << "----Our Results-------";
        cout << bnb->print();

        cout << QString("----Comparison for the %1 example----").arg(i);
        cout << QString("***tf from benchmark was %1(we added the number of machines) and target function from our bnb search is %2").arg(tf).arg(bnb->bestSolutionFound.first);
        if(tf == bnb->bestSolutionFound.first){
            good[allJobs.size()]++;
            cout << "***RESULT IS THE SAME";
        }
        else{
            bad[allJobs.size()]++;
            std += bnb->bestSolutionFound.first - tf;
            cout << "***Different" << bnb->bestSolutionFound.first - tf;
        }
        cout << "Run time: " << (double(timer.elapsed()) / 1000) << "seconds";
        cout << QString("Correct  (size-numberCorrect):") << good; cout << QString("Mistakes(size-numberMistakes):") << bad; cout << QString("Avegare error: %1").arg(std/i);
        cout << QString("-----------END %1 from %2-----------------------------------------").arg(i+1).arg(inputToSol.size());
    }
    cout << QString("Correct  (size-numberCorrect):") << good;
    cout << QString("Mistakes(size-numberMistakes):") << bad;
    cout << QString("Total Avegare error: %1").arg(std/inputToSol.size());
    cout << QString("Total time: %1 seconds").arg((double(timerTotal.elapsed()) / 1000));
}

void runLocalSearch(QList<QPair<QString,QString>> inputToSol){

    QMap<int,int> good;
    QMap<int,int> bad;
    double std(0);
    QTime timerTotal; timerTotal.start();
    for (int i = 0; i < inputToSol.size(); ++i) {
        cout << QString("--------------------START %1 from %2--------------------------------").arg(i+1).arg(inputToSol.size());
        QTime timer; timer.start();
        QPair<QString,QString> inputToSolPair = inputToSol.at(i);
        cout << QString("input file number %1: inputName=%2 and solutionName=%3").arg(i+1).arg(inputToSolPair.first).arg(inputToSolPair.second);
        double tf(0); int numberOfMachines(0); double globalLower(0);
        const QList<uint> allJobs = getInputFromFile(inputToSolPair, tf, numberOfMachines, globalLower); //getting jobs from input file, printing data from files(input and sol file) and taking the upperBound as targer function(tf)
        QStringList startingAlgs = (QStringList() << "LPT"<< "SameMachine" << "BESTFIT" );
        LocalK2To10* local = new LocalK2To10(allJobs, numberOfMachines, startingAlgs, globalLower + numberOfMachines);
        cout << "----Our Results-------";

        local->printSol("best from Our local search",local->bestGlobalSolution);

        cout << QString("----Comparison for the %1 example----").arg(i);
        cout << QString("***tf from benchmark was %1(we added the number of machines) and target function from our local search is %2").arg(tf).arg(local->bestGlobalSolution.first);
        if(tf == local->bestGlobalSolution.first){
            good[allJobs.size()]++;
            cout << "***RESULT IS THE SAME";
        }
        else{
            bad[allJobs.size()]++;
            std += local->bestGlobalSolution.first - tf;
            cout << "***Different" << local->bestGlobalSolution.first - tf;
        }
        cout << "Run time: " << (double(timer.elapsed()) / 1000) << "seconds";
        cout << QString("Correct  (size-numberCorrect):") << good; cout << QString("Mistakes(size-numberMistakes):") << bad; cout << QString("Avegare error: %1").arg(std/i);
        cout << QString("-----------END %1 from %2-----------------------------------------").arg(i+1).arg(inputToSol.size());
    }
    cout << QString("Correct  (size-numberCorrect):") << good;
    cout << QString("Mistakes(size-numberMistakes):") << bad;
    cout << QString("Total Avegare error: %1").arg(std/inputToSol.size());
    cout << QString("Total time: %1 seconds").arg((double(timerTotal.elapsed()) / 1000));
}

void runGenetic(QList<QPair<QString,QString>> inputToSol, uint _populationSize, uint _generationsNumber, uint _debugLevel, bool specialGenes, int attempts,const float& _mutationPercentage, const float& _genesMutationPercentage, const uint& _fitnessFunctionIndex){
    QMap<int,int> good;
    QMap<int,int> bad;
    double std(0);
    QTime timerTotal; timerTotal.start();
    for (int i = 0; i < inputToSol.size(); ++i) {

        cout << QString("--------------------START %1 from %2--------------------------------").arg(i+1).arg(inputToSol.size()); QTime timer; timer.start();
        QPair<QString,QString> inputToSolPair = inputToSol.at(i);
        cout << QString("%1) %2").arg(i+1).arg(inputToSolPair.first.split("/").back());
        double tf(0); int numberOfMachines(0); double globalLower(0);
        const QList<uint> allJobs = getInputFromFile(inputToSolPair, tf, numberOfMachines, globalLower); //getting jobs from input file, printing data from files(input and sol file) and taking the upperBound as targer function(tf)
        tf-=numberOfMachines; //remove num machines

        Gene bestGeneFound;
        //attempt1
        Genetic g(_populationSize, _generationsNumber, numberOfMachines, allJobs,_debugLevel,specialGenes, _mutationPercentage, _genesMutationPercentage, _fitnessFunctionIndex);
        bestGeneFound = g.bestGeneFound;
        cout << QString("Attempt %1(populationSize=%2, genSize=%3): TF=%4; BenchMark=%5").arg(1).arg(_populationSize).arg(_generationsNumber).arg(g.bestGeneFound.targetFunctionValue).arg(tf);

        //attempt2
        if(attempts>=2 && bestGeneFound.targetFunctionValue > tf){
            Genetic g2(_populationSize/2, _generationsNumber*2, numberOfMachines, allJobs,_debugLevel,specialGenes, _mutationPercentage, _genesMutationPercentage, _fitnessFunctionIndex);
            if(g2.bestGeneFound.targetFunctionValue < bestGeneFound.targetFunctionValue){
                bestGeneFound = g2.bestGeneFound;
            }
            cout << QString("Attempt %1(populationSize=%2, genSize=%3): TF=%4; BenchMark=%5").arg(2).arg(_populationSize/2).arg(_generationsNumber*2).arg(g2.bestGeneFound.targetFunctionValue).arg(tf);
        }

        if(attempts>=3 && bestGeneFound.targetFunctionValue > tf){
            //attempt3
            Genetic g3(_populationSize*2, _generationsNumber/2, numberOfMachines, allJobs,_debugLevel,specialGenes, _mutationPercentage, _genesMutationPercentage, _fitnessFunctionIndex);
            if(g3.bestGeneFound.targetFunctionValue < bestGeneFound.targetFunctionValue){
                bestGeneFound = g3.bestGeneFound;
            }
            cout << QString("Attempt %1(populationSize=%2, genSize=%3): TF=%4; BenchMark=%5").arg(3).arg(_populationSize*2).arg(_generationsNumber/2).arg(g3.bestGeneFound.targetFunctionValue).arg(tf);
        }



        cout << QString("----Comparison for the %1 example----").arg(i);
        cout << QString("***BenchmarkTf=%1 ourGeneticTf=%2").arg(tf).arg(bestGeneFound.targetFunctionValue);
        if(tf == bestGeneFound.targetFunctionValue){
            good[allJobs.size()]++;
            cout << "***RESULT IS THE SAME";
        }
        else{
            bad[allJobs.size()]++;
            std += bestGeneFound.targetFunctionValue - tf;
            cout << "***RESULT IS Different" << bestGeneFound.targetFunctionValue - tf;
        }
        cout << "Run time: " << (double(timer.elapsed()) / 1000) << "seconds";
        cout << QString("Correct  (size-numberCorrect):") << good; cout << QString("Mistakes(size-numberMistakes):") << bad; cout << QString("Avegare error: %1").arg(std/i);
        cout << QString("#mutations so far=%1").arg(Genetic::globalSumMutation);//TODO - temp - delete
        cout << QString("-----------END %1 from %2-----------------------------------------").arg(i+1).arg(inputToSol.size());
    }
    cout << QString("Correct  (size-numberCorrect):") << good;
    cout << QString("Mistakes(size-numberMistakes):") << bad;
    cout << QString("Total Avegare error: %1").arg(std/inputToSol.size());
    cout << QString("Total time: %1 seconds").arg((double(timerTotal.elapsed()) / 1000));
    cout << "oldest Gen" << Genetic::globalBestReplace;
}

const uint& _populationSize(100);
const uint& _generationsNumber(100);
const uint& _machinesNumber(5);
QList<uint> allJobs = getInput(38);
const uint& _debugLevel(3);


QList<uint> parseFiles(QPair<QString,QString> inputToSol,double& tf, int& numberOfMachines,double& globalLowerBound){
    QList<uint> retVal;
    QFile file(inputToSol.first);
    QFileInfo fileInfo(file.fileName());
    QString fileName(fileInfo.fileName());
    if(fileName.startsWith("NU") || fileName.startsWith("U")){
        QStringList fileNameList = fileName.split("_");
        QString solFileNameFormat=QString("%1_%2_%3_%4_%5").arg(fileNameList.at(0)).arg(fileNameList.at(1)).arg(fileNameList.at(3)).arg(fileNameList.at(2)).arg(fileNameList.at(4));
        QString fullPathToSolFile = QString("%1%2%3").arg(fileInfo.dir().absolutePath()).arg("/SOL_").arg(solFileNameFormat);
        QFileInfo check_file(fullPathToSolFile);
        if (check_file.exists() && check_file.isFile()) {
            QFile fileSol(fullPathToSolFile);
            QFileInfo fileInfoSol(fileSol.fileName());
            QString fileNameSol(fileInfoSol.fileName());

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                return retVal;
            if (!fileSol.open(QIODevice::ReadOnly | QIODevice::Text))
                return retVal;
            int lineIndex(0);
            int machinesNum(0);
            int jobsNum(0);
            QList<uint> allJobs;

            while (!file.atEnd()) {
                QByteArray line = file.readLine();
                if(lineIndex == 0)  machinesNum = line.trimmed().toInt();
                else if(lineIndex == 1) jobsNum = line.trimmed().toInt();
                if(lineIndex == 2){
                    QString lineStr = line.trimmed();
                    QStringList jobsAsStrings = lineStr.split("  ");
                    for(QString s : jobsAsStrings) { allJobs << s.toUInt(); }
                }
                lineIndex++;
            }
            file.close();

            retVal = allJobs;

            lineIndex = 0;
            int machinesNumSol(0);
            int jobsNumSol(0);
            int lowerBound(0); int upperBound(0); int isOptimal(0);
            QList<uint> summedMachines;
            QList<QList<uint>> machines;
            int machinesIndex(0);
            while (!fileSol.atEnd()) {
                QByteArray line = fileSol.readLine();
                if(lineIndex == 0)  machinesNumSol = line.trimmed().toInt();
                else if(lineIndex == 1) jobsNumSol = line.trimmed().toInt();
                else if(lineIndex == 2){
                    QString lineStr = line.trimmed();
                    QStringList jobsAsStrings = lineStr.split(" ");
                    for (int i = 0; i < jobsAsStrings.size(); ++i) {
                        QString s = jobsAsStrings[i];
                        if(i==0) lowerBound = s.toInt();
                        if(i==1) upperBound = s.toInt();
                        if(i==2) isOptimal = s.toInt();
                    }
                    if(isOptimal == 0) cout << "NOT OPTIMAL:" << fileNameSol;
                }
                if(lineIndex > 2){//each 3 is a machine info (first row indices of jobs, second row values of jobs, third row sum of the machine
                    if((lineIndex-3)%3 == 0){//dont need the jobs indices
                    }
                    else if((lineIndex-3)%3 == 1){//machine values
                        QList<uint> newMachine;
                        QString lineStr = line.trimmed();
                        QStringList jobsAsStrings = lineStr.split(" ");
                        for(QString s : jobsAsStrings) { newMachine << s.toUInt(); }
                        machines.push_back(newMachine);
                    }
                    else if((lineIndex-3)%3 == 2){//machine sum
                        summedMachines.push_back(line.trimmed().toInt());
                        machinesIndex++;//only here we go to next machine
                    }
                }
                lineIndex++;
            }
            Q_UNUSED(jobsNumSol);
            cout << qPrintable(QString("*Data: machinesNum=%2 jobsNum=%3 BenchMarkTf=%4").arg(machinesNum).arg(jobsNum).arg(lowerBound));//<< "allJobs" << allJobs;
//            QString format("***SOLUTION Data from file %1: machinesNum=%2 jobsNum=%3 lowerBound=%4 upperBound=%5 isOptimal=%6");
//            cout << QString(format).arg(fileNameSol).arg(machinesNumSol).arg(jobsNumSol).arg(lowerBound).arg(upperBound).arg(isOptimal);
            globalLowerBound = upperBound;
            tf = upperBound + machinesNumSol;
            numberOfMachines = machinesNum;
            cout << "*Content of machines summed" << summedMachines;
//            cout << "\tContent of machines" << machines;
            fileSol.close();

        }
    }
    return retVal;
}


const QList<uint> getInputFromFile(QPair<QString,QString> inputToSol, double& tf, int& numberOfMachines, double& globalLowerBound)
{
    QList<uint> inputReturn = parseFiles(inputToSol, tf, numberOfMachines, globalLowerBound);


    if(inputReturn.isEmpty()) return inputReturn;
    //sorting from high to low
    std::sort(inputReturn.rbegin(), inputReturn.rend());

    double sumAll(0);
    for(const uint& job :inputReturn) sumAll+= job;
    if(inputReturn.size()<=10){
        cout << "input selected:"<<inputReturn << "size" << inputReturn.size() << "sum" << sumAll;
    }
    else{
        cout << "input selected:" << "size" << inputReturn.size() << "sum" << sumAll;
    }
    return inputReturn;
}

QList<QPair<QString,QString>> getAllTestsNames(){
//    QMap<int,int> numberOfJobs;
    QList<QPair<QString,QString>> inputToSol;
    QDirIterator it("../h/docs/benchMark/all", QDirIterator::Subdirectories); //for pc
//    QDirIterator it("h/docs/benchMark/all", QDirIterator::Subdirectories); //for gilad laptop - if cant read folder - configure project one above h
    while (it.hasNext()) {
        QFile file(it.next());
        QFileInfo fileInfo(file.fileName());
        QString fileName(fileInfo.fileName());
        if(fileName.startsWith("NU") || fileName.startsWith("U")){
            QStringList fileNameList = fileName.split("_");
            QString solFileNameFormat=QString("%1_%2_%3_%4_%5").arg(fileNameList.at(0)).arg(fileNameList.at(1)).arg(fileNameList.at(3)).arg(fileNameList.at(2)).arg(fileNameList.at(4));
            QString fullPathToSolFile = QString("%1%2%3").arg(fileInfo.dir().absolutePath()).arg("/SOL_").arg(solFileNameFormat);
            QFileInfo check_file(fullPathToSolFile);
            if (check_file.exists() && check_file.isFile()) {
//                QString s = fileNameList.at(2); numberOfJobs[s.toInt()]++; //just to count
                QFile fileSol(fullPathToSolFile);
                QFileInfo fileInfoSol(fileSol.fileName());
                QString fileNameSol(fileInfoSol.fileName());
                QPair<QString,QString> p;
                p.first = fileInfo.absoluteFilePath();
                p.second = fullPathToSolFile;
                inputToSol.push_back(p);
            }

        }
    }
//    cout << "number of jobs to how many files have this jobs. for example (10,12) means 12 files have 10 jobs tasks\n" <<numberOfJobs;
    return inputToSol;
}


void runOnAllFolder(){
    QDirIterator it("../h/docs/benchMark/all", QDirIterator::Subdirectories);
    while (it.hasNext()) {

    }
}

void initInputs(){
    inputsMap[1] = (QList<uint>() << 7 << 9 << 3);
    inputsMap[2] = (QList<uint>() << 9 << 7 << 2);
    inputsMap[3] = (QList<uint>() << 2 << 9 << 7);
    inputsMap[4] = (QList<uint>() << 9 << 7 << 7 << 11 << 2 << 31 << 27 << 35 << 4 << 19);
    inputsMap[5] = (QList<uint>() << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 30<<10);
    inputsMap[6] = (QList<uint>() << 30 << 30 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10);
    inputsMap[7] = (QList<uint>() << 1 << 1 << 1 << 1 << 1 << 2 << 3 << 4 << 5 << 6);
    inputsMap[8] = (QList<uint>() << 1 << 2 << 3 << 5 << 6 << 9);
    inputsMap[9] = (QList<uint>() << 1 << 1 << 1 << 1 << 1 << 1 << 2 << 2 << 2 << 2 << 2 << 2);
    inputsMap[10] = (QList<uint>() << 1 << 2 << 2 << 2 << 2 << 4 << 4 << 4 << 4 );
    inputsMap[11] = (QList<uint>() << 2 << 2 << 2 << 2 << 3 << 3 << 3 << 4 << 4 << 5 << 6 );
    inputsMap[12] = (QList<uint>() << 6 << 3 << 3 << 2 << 2 << 2);
    inputsMap[13] = (QList<uint>() << 9 << 7 << 7 << 11 << 2 << 31 << 27 << 35 << 4 << 19 << 23 << 29 << 17 << 32 << 33);
    inputsMap[14] =(QList<uint>() << 9 << 7 << 7 << 11 << 2 << 31 << 27 << 35 << 4 << 19 << 23 << 29 << 17 << 32 << 33 << 25 << 32 << 48);
    inputsMap[15] = (QList<uint>() << 9 << 7 << 7 << 11 << 2 << 31 << 27 << 35 << 4 << 19 << 23 << 17<< 21<< 22<< 29 << 17 << 32 << 33 << 25 << 32 << 48 << 22 <<11 <<34 << 9 <<1 <<22 <<11 <<34);
    inputsMap[152] = (QList<uint>() << 9 << 7 << 7 << 11 << 2 << 31 << 27 << 35 << 4 << 19 << 23 << 17<< 21<<  33 << 25 << 32 << 48 << 22 <<11 <<34 << 9 <<1 <<22 <<11 <<34);
    inputsMap[16] = (QList<uint>() << 9 << 7 << 7 << 11 << 2<<23<<15 << 31  << 35 << 4 << 19 << 23 << 17<< 21<< 22<< 29 << 17 << 32 << 33 << 25 << 32 << 48 << 22 <<11 <<34 << 9 <<1 <<22 <<11 <<34);
    inputsMap[17] = (QList<uint>() << 290 << 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10);
    inputsMap[18] = (QList<uint>() << 37 << 20<< 42<< 25<< 33<< 8<< 18<< 32<< 22<< 39<< 2 << 30<< 18<< 44<< 25<< 7 << 31<< 19<< 10<< 12<< 5 << 9<< 48<< 10<< 17<< 6<< 1<< 37<< 47<< 42);
    inputsMap[19] = (QList<uint>() << 290<< 310<< 110<< 150<< 270<< 140<< 425<< 525<< 600<< 415<< 320<< 90<< 90<< 90<< 330<< 10<< 325<< 80<< 210<< 310<< 170<< 50<< 190<< 730<< 490<< 510<< 190<< 335<< 620<< 120);
    inputsMap[20] = (QList<uint>()<< 2 <<2 <<2 <<2<< 2 <<2 <<2 <<2<< 2 <<2 <<2 <<2<< 2 <<2 <<2 <<2<< 2 <<2 <<2 <<2<< 2 <<2 <<2 <<2<< 2 <<2 <<2 <<2<< 2 <<2 <<2 <<2);
    inputsMap[21] = (QList<uint>()<< 1 <<1 <<1 <<1 <<1 << 1<< 1 <<1 <<1 <<1 <<1 << 1<< 1 <<1 <<1 <<1 <<1 << 1<< 1 <<1 <<1 <<1 <<1 << 1<< 1 <<1 <<1 <<1 <<1 << 1<< 1 <<1 <<1 <<1 <<1 << 1);
    inputsMap[22] = (QList<uint>()<< 2 <<2 <<2 <<2<< 2 <<2 <<2 <<2<< 2 <<2 <<2 <<2<< 2 <<2 <<2 <<2<< 2 <<2 <<2 <<2<< 2 <<2 <<2 <<2<< 1 <<1 <<1 <<1 <<1 << 1 << 1 << 1<< 1 <<1 <<1 <<1 <<1 << 1 << 1 << 1);
    inputsMap[23] = (QList<uint>()<< 1 << 2 << 3<< 1 << 2 << 3<< 1 << 2 << 3<< 1 << 2 << 3<< 1 << 2 << 3<< 1 << 2 << 3);
    inputsMap[24] = (QList<uint>()<< 1 << 2 << 3 << 3<< 1 << 2 << 3 << 3<< 1 << 2 << 3 << 3<< 1 << 2 << 3 << 3<< 1 << 2 << 3 << 3<< 1 << 2 << 3 << 3<< 1 << 2 << 3 << 3<< 1 << 2 << 3 << 3<< 1 << 2 << 3 << 3);
    inputsMap[25] = (QList<uint>()<< 30 << 20 << 5 << 5 << 5 << 3 << 3 << 3 << 2<< 2 << 2 << 1 << 1 << 1 << 1 << 1);
    inputsMap[26] = (QList<uint>()<< 1 << 1 << 2 << 2 << 3 << 3<< 4 << 4 << 5 << 5 << 6 << 6<< 7 << 7 << 8 << 8 << 9 << 9);
    inputsMap[27] = (QList<uint>()<< 5 << 5 << 5 << 5 << 5<< 5 << 5 << 5 << 5 << 5<< 5 << 5 << 5 << 5 << 5<< 5 << 5 << 5 << 5 << 5);
    inputsMap[28] = (QList<uint>()<< 3 << 3 << 3 << 3<< 3 << 3 << 3 << 3<< 3 << 3 << 3 << 3<< 3 << 3 << 3 << 3<< 3 << 3 << 3 << 3<< 3 << 3 << 3 << 3<< 3 << 3 << 3 << 3);
    inputsMap[29] = (QList<uint>()<< 100 << 90 << 80 << 70<< 60 << 50 << 40 << 30<< 20 << 10<< 1 << 2 << 3 << 4 << 5<< 1 << 2 << 3 << 4 << 5<< 1 << 2 << 3 << 4 << 5<< 1 << 2 << 3 << 4 << 5<< 1 << 2 << 3 << 4 << 5);
    inputsMap[30] = (QList<uint>()<< 1<<2<<3<<4<<100 << 90 << 80 << 70<< 60 << 50 << 40 << 30<< 20 << 10<< 1 << 2 << 3 << 4 << 5<< 1 << 2 << 3 << 4 << 5<< 1 << 2 << 3 << 4 << 5<< 1 << 2 << 3 << 4 << 5 << 5);
//    inputsMap[31] = (QList<uint>()
//                     <<32<<91<<82<<38<<97<<53<<12<<68<<24<<83<<39<<98<<89<<44<<4<<60<<16<<75<<31<<90<<46<<36<<96<<51<<11<<67<<23<<82<<38<<97<<87<<43<<2<<58<<14<<74<<30<<99<<54<<48<<4<<60<<19<<75<<34<<90<<46<<5<<96<<55<<11
//                     <<67<<26<<82<<38<<97<<53<<47<<3<<59<<18<<74<<33<<89<<45<<4<<95<<54<<10<<66<<25<<81<<37<<96<<52<<46<<2<<58<<17<<73<<32<<88<<44<<3<<59<<49<<9<<65<<24<<80<<36<<95<<51<<10<<100<<56<<16<<71<<31<<87<<43
//                     <<2<<58<<48<<7<<63<<22<<78<<34<<94<<50<<76<<40<<67<<97<<23<<50<<76<<3<<30<<56<<86<<50<<77<<3<<76<<31<<87<<47<<3<<62<<52<<11<<67<<23<<83<<38<<98<<54<<10<<69<<59<<15<<74<<30<<89<<45<<1<<61<<17<<10
//                     <<66<<22<<81<<37<<93<<52<<8<<68<<58<<14<<73<<29<<88<<44<<100<<59<<15<<6<<65<<21<<80<<36<<92<<51<<7<<66<<57<<13<<72<<28<<87<<43<<99<<58<<14<<8<<64<<20<<79<<35<<94<<50<<6<<65<<55<<15<<71<<27<<86<<42
//                     <<98<<57<<13<<72<<62<<18<<78<<34<<93<<49<<5<<64<<20<<10<<69<<25<<85<<40<<96<<56<<12<<71<<61<<17<<76<<32<<92<<47<<3<<63<<19<<12<<68<<24<<83<<39<<95<<54<<10<<70<<60<<16<<75<<2<<28<<55<<81<<11<<54<<13
//                     <<4<<60<<19<<75<<34<<90<<46<<5<<61<<55<<11<<67<<26<<82<<41<<97<<53<<12<<2<<62<<18<<74<<33<<89<<45<<4<<60<<53<<9<<65<<25<<80<<40<<96<<52<<11<<1<<60<<16<<72<<31<<87<<43<<3<<59<<52<<8<<64<<23<<79
//                     <<38<<94<<50<<10<<66<<56<<15<<71<<30<<86<<42<<1<<57<<17<<7<<63<<22<<78<<34<<93<<49<<8<<64<<55<<14<<70<<29<<85<<41<<100<<56<<15<<6<<62<<21<<77<<33<<92<<48<<7<<63<<53<<13<<69<<28<<84<<40<<99<<55<<11
//                     <<4<<60<<20<<76<<31<<91<<47<<6<<62<<52<<11<<67<<27<<83<<38<<98<<54<<10<<69<<59<<18<<74<<30<<89<<45<<5<<61<<17<<10<<66<<22<<81<<37<<96<<52<<8<<68<<58<<17<<73<<29<<88<<44<<100<<59<<15<<9<<65<<21<<80
//                     <<36<<95<<51<<7<<66<<57<<16<<72<<28<<87<<43<<99<<58<<14<<8<<64<<20<<79<<35<<94<<50<<6<<65<<55<<11<<71<<27<<86<<42<<98<<57<<13<<72<<62<<18<<78<<34<<89<<49<<5<<64<<20<<10<<69<<25<<85<<40<<96<<56<<12
//                     <<71<<61<<17<<76<<32<<88<<47<<3<<63<<19<<9<<68<<24<<83<<39<<95<<54<<10<<66<<60<<16<<75<<31<<87<<46<<2<<61<<17<<8<<67<<23<<82<<38<<94<<53<<9<<65<<59<<15<<74<<30<<86<<32<<88<<47<<74<<100<<64<<91<<21
//                     <<47<<20<<76<<35<<91<<47<<40<<96<<56<<11<<67<<27<<83<<42<<98<<88<<47<<3<<59<<18<<74<<34<<90<<45<<39<<95<<54<<10<<66<<25<<81<<37<<97<<52<<46<<2<<58<<17<<73<<32<<88<<44<<100<<94<<50<<6<<65<<21<<77<<36
//                     <<92<<51<<42<<98<<57<<13<<72<<28<<84<<43<<99<<93<<49<<4<<64<<20<<79<<35<<91<<50<<40<<100<<56<<11<<71<<27<<86<<42<<98<<91<<47<<3<<62<<18<<78<<34<<90<<49<<39<<98<<54<<10<<69<<25<<81<<41<<97<<56<<46<<2
//                     <<61<<17<<76<<32<<88<<48<<3<<97<<53<<9<<68<<24<<80<<39<<95<<54<<45<<1<<60<<16<<75<<31<<87<<46<<2<<96<<52<<8<<67<<23<<82<<38<<94<<53<<44<<100<<59<<15<<74<<30<<89<<45<<1<<95<<51<<10<<66<<22<<81<<37
//                     <<93<<52<<42<<2<<58<<13<<73<<29<<88<<44<<100<<59<<49<<9<<65<<20<<80<<36<<92<<51<<7<<100<<56<<16<<71<<27<<87<<43<<99<<58<<48<<7<<63<<19<<78<<34<<94<<50<<5<<99<<55<<11<<70<<26<<85<<41<<97<<57<<47<<6
//                     <<62<<18<<77<<33<<92<<48<<4<<98<<54<<10<<69<<25<<84<<40<<96<<55<<46<<5<<61<<17<<76<<32<<91<<47<<3<<62<<53<<9<<68<<24<<83<<39<<98<<54<<10<<4<<60<<16<<75<<31<<90<<46<<2<<61<<51<<11<<67<<22<<82<<38
//                     <<94<<53<<9<<2<<58<<14<<74<<29<<89<<45<<94<<50<<44<<100<<59<<15<<71<<30<<86<<42<<1<<91<<51<<7<<62<<22<<78<<37<<93<<49<<42<<98<<58<<13<<69<<29<<85<<41<<100<<56<<49<<5<<61<<20<<76<<36<<92<<48<<7<<97
//                     <<53<<12<<68<<27<<83<<39<<99<<54<<48<<4<<60<<19<<75<<31<<90<<46<<5<<96<<52<<11<<67<<26<<82<<38<<97<<53<<47<<3<<59<<18<<74<<30<<89<<45<<4<<95<<51<<10<<66<<25<<81<<37<<96<<52<<42<<2<<58<<17<<73<<29
//                     <<88<<44<<3<<59<<49<<9<<65<<20<<80<<36<<95<<51<<7<<100<<56<<16<<71<<27<<87<<43<<2<<58<<48<<7<<63<<19<<78<<34<<94<<50<<5<<99<<55<<14<<70<<26<<85<<41<<97<<57<<47<<6<<62<<18<<77<<33<<92<<48<<4<<98
//                     <<54<<13<<69<<25<<84<<40<<96<<55<<46<<5<<61<<17<<76<<32<<91<<47<<3<<62<<53<<9<<68<<24<<83<<39<<95<<54<<10<<4<<60<<16<<75<<31<<87<<46<<2<<61<<51<<7<<67<<22<<82<<38<<94<<53<<9<<2<<58<<14<<74<<29
//                     <<5);
    inputsMap[32] = (QList<uint>()<< 99 << 90 << 96 << 98 << 96 << 95 << 98 << 97 << 95 << 1 );
    inputsMap[33] = (QList<uint>()<< 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2<< 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2<< 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2<< 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2);
        inputsMap[34] = (QList<uint>() << 273);
        for(int i = 11; i <= 60; ++i)
            inputsMap[34] << i;
        inputsMap[35] = (QList<uint>());
        for(int i = 77; i <= 77 + 50 * 2; i+=2)
            inputsMap[35] << i;
        inputsMap[36] = (QList<uint>());
        for(int i = 0; i <= 30; ++i)
            inputsMap[36] << 2;
        for(int i = 0; i <= 20; ++i)
            inputsMap[36] << 3;
        inputsMap[37] = (QList<uint>());
        for(int i = 0; i <= 25; ++i)
            inputsMap[37] << 6 << 8;
    inputsMap[38] = (QList<uint>()<< 1 << 2 << 5 << 6);
}

const QList<uint> getInput(int inputBatch)
{
    QList<uint> inputReturn = inputsMap.contains(inputBatch) ? inputsMap[inputBatch] : QList<uint>();
    if(inputReturn.isEmpty()) return inputReturn;
    std::sort(inputReturn.rbegin(), inputReturn.rend());
    double sumAll(0);
    for(const uint& job :inputReturn) sumAll+= job;
    cout << "input selected:"<<inputReturn << "size" << inputReturn.size() << "sum" << sumAll;
//    cout << "input selected:" << "size" << inputReturn.size() << "sum" << sumAll;
    return inputReturn;
}

QList<uint> shuffleMyQLIST(QList<uint> &input3rand)
{
    Sleep(1000);//for rand to have different time
    qsrand(QDateTime::currentDateTime().toTime_t());
    std::random_shuffle(input3rand.begin(), input3rand.end());
    return input3rand;
}
