#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;
class Complex {
private:
    double r,i;
public:
    void Print() {
        cout << r << "+" << i << "i" << endl;
    }
	Complex & operator=(const char t[]){
		if(!t){
			r = 0.0;
			i = 0.0;
		}
		else{
			r = t[0] - '0';
			i = t[2] - '0';
		}
		return *this;
	}
};
int main() {
    Complex a;
    a = "3+4i"; a.Print();
    a = "5+6i"; a.Print();
    return 0;
}