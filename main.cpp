#include "mainwindow.h"
#include <QApplication>
#include "BandB.h"
#define TESTS_NUM 1

static QMap<unsigned int, QList<unsigned int>> inputsMap;
void initInputs();
const QList<unsigned int> getInput(int inputBatch, bool shouldShuffle);
QList<unsigned int> shuffleMyQLIST(QList<unsigned int> &input3rand);

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    initInputs();
    for(int i=1; i<=TESTS_NUM; ++i){
        const QList<unsigned int> input = getInput(i,true);
        new BandB(input);
    }
    return 0;
}
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();

void initInputs(){
    QList<unsigned int> input1 = (QList<unsigned int>() << 3 << 9 << 7);
    QList<unsigned int> input2 = (QList<unsigned int>() << 9 << 7 << 2);
    QList<unsigned int> input3 = (QList<unsigned int>() << 2 << 9 << 7);
    QList<unsigned int> input4 = (QList<unsigned int>() << 9 << 7 << 7 << 11 << 2 << 31 << 27 << 35 << 4 << 19);
    QList<unsigned int> input5 = (QList<unsigned int>() << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 30<<10);
    QList<unsigned int> input6 = (QList<unsigned int>() << 30 << 30 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10);
    inputsMap[1] = input1;
    inputsMap[2] = input2;
    inputsMap[3] = input3;
    inputsMap[4] = input4;
    inputsMap[5] = input5;
    inputsMap[6] = input6;
}

const QList<unsigned int> getInput(int inputBatch, bool shouldShuffle)
{
    QList<unsigned int> inputReturn = inputsMap.contains(inputBatch) ? inputsMap[inputBatch] : QList<unsigned int>();
    if(shouldShuffle){
        inputReturn = shuffleMyQLIST(inputReturn);
    }
    if(DEBUGLEVEL >= 1) cout << "input selected:"<<inputReturn;
    return inputReturn;
}

QList<unsigned int> shuffleMyQLIST(QList<unsigned int> &input3rand)
{
    qsrand(QDateTime::currentDateTime().toTime_t());
    std::random_shuffle(input3rand.begin(), input3rand.end());
    return input3rand;
}
