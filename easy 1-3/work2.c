#include <iostream>
using namespace std;
class Sample{
public:
    int v;
    Sample(int n):v(n) { }
	Sample(Sample & x) {v = x.v * 2;}
};
int main() {
    Sample a(5);
    Sample b = a;
    cout << b.v;
    return 0;
}