#include "mainwindow.h"
#include <windows.h>
#include <QApplication>
#include "BandB.h"
#define TESTS_NUM 6

static QMap<uint, QList<uint>> inputsMap;
void initInputs();
const QList<uint> getInput(int inputBatch, bool shouldShuffle);
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
//        myinput = shuffleMyQLIST(myinput);
//        cout << "Input selected:"<<myinput << ". Size:" << myinput.size() << ". New element:" << extraJob;
//        new BandB(myinput);
//    }
    new BandB(getInput(11,false));
    return 0;
}
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();

void initInputs(){
    QList<uint> input1 = (QList<uint>() << 7 << 9 << 3);
    QList<uint> input2 = (QList<uint>() << 9 << 7 << 2);
    QList<uint> input3 = (QList<uint>() << 2 << 9 << 7);
    QList<uint> input4 = (QList<uint>() << 9 << 7 << 7 << 11 << 2 << 31 << 27 << 35 << 4 << 19);
    QList<uint> input5 = (QList<uint>() << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 30<<10);
    QList<uint> input6 = (QList<uint>() << 30 << 30 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10);
    inputsMap[1] = input1;
    inputsMap[2] = input2;
    inputsMap[3] = input3;
    inputsMap[4] = input4;
    inputsMap[5] = input5;
    inputsMap[6] = input6;
    QList<uint> input7Lecture = (QList<uint>() << 1 << 1 << 1 << 1 << 1 << 2 << 3 << 4 << 5 << 6);
    QList<uint> input8Lecture = (QList<uint>() << 1 << 2 << 3 << 5 << 6 << 9);
    inputsMap[7] = input7Lecture;
    inputsMap[8] = input8Lecture;
    QList<uint> input9Lecture = (QList<uint>() << 1 << 1 << 1 << 1 << 1 << 1 << 2 << 2 << 2 << 2 << 2 << 2);
    inputsMap[9] = input9Lecture;
    QList<uint> input10Lecture = (QList<uint>() << 1 << 2 << 2 << 2 << 2 << 4 << 4 << 4 << 4 );
    inputsMap[10] = input10Lecture;
    QList<uint> input11Lecture = (QList<uint>() << 2 << 2 << 2 << 2 << 3 << 3 << 3 << 4 << 4 << 5 << 6 );
    inputsMap[11] = input11Lecture;
}

const QList<uint> getInput(int inputBatch, bool shouldShuffle)
{
    QList<uint> inputReturn = inputsMap.contains(inputBatch) ? inputsMap[inputBatch] : QList<uint>();
    if(shouldShuffle){
        inputReturn = shuffleMyQLIST(inputReturn);
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
