#include <windows.h>
#include <QApplication>
#include <algorithm>

#include "BandBK2To10.h"
#include "LocalK2To10.h"

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
//    new BandBK2To10(getInput(28,false,1));
    new LocalK2To10(getInput(3,false,1));
    return 0;
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
    inputsMap[16] = (QList<uint>() << 9 << 7 << 7 << 11 << 2<<23<<15 << 31  << 35 << 4 << 19 << 23 << 17<< 21<< 22<< 29 << 17 << 32 << 33 << 25 << 32 << 48 << 22 <<11 <<34 << 9 <<1 <<22 <<11 <<34);
    inputsMap[17] = (QList<uint>() << 290 << 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10<< 10);
    inputsMap[18] = (QList<uint>() << 37 << 20<< 42<< 25<< 33<< 8<< 18<< 32<< 22<< 39<< 2 << 30<< 18<< 44<< 25<< 7 << 31<< 19<< 10<< 12<< 5 << 9<< 48<< 10<< 17<< 6<< 1<< 37<< 47<< 42);
    inputsMap[19] = (QList<uint>() << 290<< 310<< 110<< 150<< 270<< 140<< 425<< 525<< 600<< 415<< 320<< 90<< 90<< 90<< 330<< 10<< 325<< 80<< 210<< 310<< 170<< 50<< 190<< 730<< 490<< 510<< 190<< 335<< 620<< 120);
    inputsMap[20] = (QList<uint>()
                     << 2 <<2 <<2 <<2
                     << 2 <<2 <<2 <<2
                     << 2 <<2 <<2 <<2
                     << 2 <<2 <<2 <<2
                     << 2 <<2 <<2 <<2
                     << 2 <<2 <<2 <<2
                     << 2 <<2 <<2 <<2
                     << 2 <<2 <<2 <<2
                     );
    inputsMap[21] = (QList<uint>()
                     << 1 <<1 <<1 <<1 <<1 << 1
                     << 1 <<1 <<1 <<1 <<1 << 1
                     << 1 <<1 <<1 <<1 <<1 << 1
                     << 1 <<1 <<1 <<1 <<1 << 1
                     << 1 <<1 <<1 <<1 <<1 << 1
                     << 1 <<1 <<1 <<1 <<1 << 1
                     );
    inputsMap[22] = (QList<uint>()
                     << 2 <<2 <<2 <<2
                     << 2 <<2 <<2 <<2
                     << 2 <<2 <<2 <<2
                     << 2 <<2 <<2 <<2
                     << 2 <<2 <<2 <<2
                     << 2 <<2 <<2 <<2
                     << 1 <<1 <<1 <<1 <<1 << 1 << 1 << 1
                     << 1 <<1 <<1 <<1 <<1 << 1 << 1 << 1
                     );
    inputsMap[23] = (QList<uint>()
                     << 1 << 2 << 3
                     << 1 << 2 << 3
                     << 1 << 2 << 3
                     << 1 << 2 << 3
                     << 1 << 2 << 3
                     << 1 << 2 << 3
                     );
    inputsMap[24] = (QList<uint>()
                     << 1 << 2 << 3 << 3
                     << 1 << 2 << 3 << 3
                     << 1 << 2 << 3 << 3
                     << 1 << 2 << 3 << 3
                     << 1 << 2 << 3 << 3
                     << 1 << 2 << 3 << 3
                     << 1 << 2 << 3 << 3
                     << 1 << 2 << 3 << 3
                     << 1 << 2 << 3 << 3
                     );
    inputsMap[25] = (QList<uint>()
                     << 30 << 20 << 5 << 5 << 5 << 3 << 3 << 3 << 2
                     << 2 << 2 << 1 << 1 << 1 << 1 << 1
                     );
    inputsMap[26] = (QList<uint>()
                     << 1 << 1 << 2 << 2 << 3 << 3
                     << 4 << 4 << 5 << 5 << 6 << 6
                     << 7 << 7 << 8 << 8 << 9 << 9
                     );
    inputsMap[27] = (QList<uint>()
                     << 5 << 5 << 5 << 5 << 5
                     << 5 << 5 << 5 << 5 << 5
                     << 5 << 5 << 5 << 5 << 5
                     << 5 << 5 << 5 << 5 << 5
                     );
    inputsMap[28] = (QList<uint>()
                     << 3 << 3 << 3 << 3
                     << 3 << 3 << 3 << 3
                     << 3 << 3 << 3 << 3
                     << 3 << 3 << 3 << 3
                     << 3 << 3 << 3 << 3
                     << 3 << 3 << 3 << 3
                     << 3 << 3 << 3 << 3
                     );


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
    double sumAll(0);
    for(const uint& job :inputReturn) sumAll+= job;
    cout << "input selected:"<<inputReturn << "size" << inputReturn.size() << "sum" << sumAll;
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
