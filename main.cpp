#include "mainwindow.h"
#include <windows.h>
#include <QApplication>
#include "BandB.h"
#include <algorithm>

#define TESTS_NUM 6

static QMap<uint, QList<uint>> inputsMap;
void initInputs();
const QList<uint> getInput(int inputBatch, bool shouldShuffle, int sortOrder);
QList<uint> shuffleMyQLIST(QList<uint> &input3rand);
uint getRandNumber(uint low, uint high );

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    initInputs();
//    const QList<uint> input = getInput(4,false);
//    QList<uint> myinput = input;
//    for(int i=11; i<=16; ++i){
//        uint extraJob = getRandNumber(10,50);
//        myinput << extraJob;
////        myinput = shuffleMyQLIST(myinput);
//        cout << "Input selected:"<<myinput << ". Size:" << myinput.size() << ". New element:" << extraJob;
//        new BandB(myinput);
//    }
//    QList<uint> myinput = getInput(8,false,1);
    new BandB(getInput(15,false,1));
    new BandB(getInput(16,false,1));

    return 0;
}
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();

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

    /*testing longer inputs - goal to reach 30 jobs*/
    inputsMap[13] = (QList<uint>() << 9 << 7 << 7 << 11 << 2 << 31 << 27 << 35 << 4 << 19 << 23 << 29 << 17 << 32 << 33);
    inputsMap[14] =(QList<uint>() << 9 << 7 << 7 << 11 << 2 << 31 << 27 << 35 << 4 << 19 << 23 << 29 << 17 << 32 << 33 << 25 << 32 << 48);
    inputsMap[15] = (QList<uint>() << 9 << 7 << 7 << 11 << 2 << 31 << 27 << 35 << 4 << 19 << 23 << 17<< 21<< 22<< 29 << 17 << 32 << 33 << 25 << 32 << 48 << 22 <<11 <<34 << 9 <<1 <<22 <<11 <<34);
    inputsMap[16] = (QList<uint>() << 9 << 7 << 7 << 11 << 2<<23<<15 << 31 << 27 << 35 << 4 << 19 << 23 << 17<< 21<< 22<< 29 << 17 << 32 << 33 << 25 << 32 << 48 << 22 <<11 <<34 << 9 <<1 <<22 <<11 <<34);
}

const QList<uint> getInput(int inputBatch, bool shouldShuffle, int sortOrder)
{
    QList<uint> inputReturn = inputsMap.contains(inputBatch) ? inputsMap[inputBatch] : QList<uint>();
    if(shouldShuffle){
        inputReturn = shuffleMyQLIST(inputReturn);
    }
    //sorting from low to high
    if(sortOrder == -1){
        std::sort(inputReturn.begin(), inputReturn.end());
    }
    //sorting from high to low
    else if(sortOrder == 1){
        std::sort(inputReturn.rbegin(), inputReturn.rend());
    }
    cout << "input selected:"<<inputReturn << "size" << inputReturn.size();
    return inputReturn;
}

QList<uint> shuffleMyQLIST(QList<uint> &input3rand)
{
    Sleep(1000);//for rand to have different time
    qsrand(QDateTime::currentDateTime().toTime_t());
    std::random_shuffle(input3rand.begin(), input3rand.end());
    return input3rand;
}

uint getRandNumber(uint low, uint high )
{
    Sleep(1000);//for rand to have different time
    qsrand(uint(QTime::currentTime().msec()));
    return (qrand() % ((high + 1) - low) + low);
}
