#include "bandb.h"

BandB::BandB()
{
    mode = getModelChoice();
    QList<unsigned int> allJobs = getInput();
    Node* root = initializeRoot(allJobs);
    activeNodes.push_back(root);
    runBnb();
}

bool BandB::getModelChoice()
{
    //if true => 2<=k<=10
    //if false => k->(k^3)
    return true;
}

const QList<unsigned int> BandB::getInput()
{
    QList<unsigned int> input;
    input.push_back(3);
    input.push_back(9);
    input.push_back(7);
    return input;
}

Node *BandB::initializeRoot(const QList<unsigned int> &allJobs)
{
    Node* treeHead = new Node(QList<unsigned int>(), allJobs);
    calcLowerBound(treeHead);
    calcUpperBound(treeHead);
    unsigned int job = treeHead->getJob();
    treeHead->state.push_back(job);
    return treeHead;
}

void BandB::calcLowerBound(Node *node) const
{
    node->L = 9;
}

void BandB::calcUpperBound(Node *node) const
{
    node->U = 19;
}

void BandB::runBnb()
{
    while(!activeNodes.isEmpty()){
        Node* node = getActive();
        unsigned int job = node->getJob();
        cout << node->toString();
        cout << "current job:" << job << ". active nodes: " <<activeNodes.size();
        for(int i=0; i<node->state.size() +1; ++i){
            Node* sonI =  new Node(node->state, node->jobsLeft);
            if(node->state.size() -1 >= i){
                sonI->state[i] += job;
            }
            else{
                sonI->state.push_back(job);
            }
            if(!sonI->jobsLeft.isEmpty()){
                calcLowerBound(sonI);
                calcUpperBound(sonI);
                activeNodes.push_back(sonI);
            }
            else{
                cout <<"LEAF## " << sonI->toString();
            }
        }
    }
}

Node *BandB::getActive()
{
    return activeNodes.takeLast();
}
