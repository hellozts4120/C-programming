#include <iostream>
#include <string>
using namespace std;

template<class T>
class CMyistream_iterator{
private:
	istream &is;
	T value;
public:
	CMyistream_iterator(istream &i):is(i){
		is >> value;
	}
	void operator++(int){
		is >> value;
	}
	T & operator*(){
		return value;
	}
};

int main()
{
    CMyistream_iterator<int> inputInt(cin);
    int n1,n2,n3;
    n1 = * inputInt; //∂¡»Î n1
    int tmp = * inputInt;
    cout << tmp << endl;
    inputInt ++;
    n2 = * inputInt; //∂¡»Î n2
    inputInt ++;
    n3 = * inputInt; //∂¡»Î n3
    cout << n1 << "," << n2<< "," << n3 << endl;
    CMyistream_iterator<string> inputStr(cin);
    string s1,s2;
    s1 = * inputStr;
    inputStr ++;
    s2 = * inputStr;
    cout << s1 << "," << s2 << endl;
    return 0;
}