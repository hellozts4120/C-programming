#include <iostream>
using namespace std;
class Number {
public:
    int num;
    Number(int n): num(n) {
    }

	Number(const Number & a){
		num = a.num;
	}
	int & value(){
		return this->num;
	}
	Number & operator+(const Number & a){
		num += a.num;
		return *this;
	}

};
int main() {
    Number a(2);
    Number b = a;
    cout << a.value() << endl;
    cout << b.value() << endl;
    a.value() = 8;
    cout << a.value() << endl;
    a+b;
    cout << a.value() << endl;
    return 0;
}