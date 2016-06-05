#include<iostream>
#include<stdlib.h>
#include<stack>
#include<queue>
#include<vector>
using namespace std;


int LRU(int pageFrameCount, vector<int> pageVector) {
    vector<int> s;
    int pageFaultCount = 0;
    bool isContain[10] = {false, false, false, false, false, false, false, false, false, false};
    for (int i = 0; i < pageVector.size(); i++) {
        if (s.size() < pageFrameCount && !isContain[pageVector[i]]) {
            pageFaultCount++;
            s.push_back(pageVector[i]);
            isContain[pageVector[i]] = true;
        }
        else if (!isContain[pageVector[i]]) {
            isContain[s[0]] = false;
            s.erase(s.begin());
            pageFaultCount++;
            s.push_back(pageVector[i]);
            isContain[pageVector[i]] = true;
        }
        else if (isContain[pageVector[i]]) {
            for (int j = 0; j < s.size(); j++) {
                if (s[j] == pageVector[i]) {
                    s.erase(s.begin() + j);
                    s.push_back(pageVector[i]);
                }
            }
        }
    }
    return pageFaultCount;
}

int FIFO(int pageFrameCount, vector<int> pageVector) {
    queue<int> q;
    int pageFaultCount = 0;
    bool isContain[10] = {false, false, false, false, false, false, false, false, false, false};
    for (int i = 0; i < pageVector.size(); i++) {
        if (q.size() < pageFrameCount && !isContain[pageVector[i]]) {
            pageFaultCount++;
            q.push(pageVector[i]);
            isContain[pageVector[i]] = true;
        }
        else if (!isContain[pageVector[i]]) {
            isContain[q.front()] = false;
            q.pop();
            pageFaultCount++;
            q.push(pageVector[i]);
            isContain[pageVector[i]] = true;
        }
    }
    return pageFaultCount;
}

int main() {
    cout << "Average page faults of the two algorithms are:" << endl;
    int averageLRU[7], averageFIFO[7];
    for (int i = 1; i <= 7; i++) {
        averageLRU[i - 1] = 0, averageFIFO[i - 1] = 0;
        for (int j = 0; j < 100; j++) {
            vector<int> pageVector;
            for (int k = 0; k < 200; k++) {
                pageVector.push_back(int(rand() % 10));
            }
            averageLRU[i - 1] += LRU(i, pageVector);
            averageFIFO[i - 1] += FIFO(i, pageVector);
        }
        
    }
    
    cout << "pageFrame:\t1\t2\t3\t4\t5\t6\t7" << endl;
    cout << "LRU-Count:\t";
    for (int i = 0; i < 7; i++) {
        cout << averageLRU[i] / 100 << "\t";
    }
    cout << endl;
    cout << "FIFO-Count:\t";
    for (int i = 0; i < 7; i++) {
        cout << averageFIFO[i] / 100 << "\t";
    }
    cout << endl;
}