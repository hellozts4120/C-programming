#include <iostream>
#include <set>
#include <iterator>
#include <algorithm>
using namespace std;

class A{
public:
	int age;
	char s;
	A(int a):age(a),s('A'){};
	A(int a,char ss):age(a),s(ss){};
};
class B:public A{
public:
	B(int a):A(a,'B'){};
};

class Comp{
public:
	bool operator()(const A * a, const A * b){
		return a->age < b->age;
	}
};

void Print(const A * a){
	cout << a->s << " " << a->age << endl;
}

int main()
{

        int t;
        cin >> t;
        set<A*,Comp> ct;
        while( t -- ) {
                int n;
                cin >> n;
                ct.clear();
                for( int i = 0;i < n; ++i)	{
                        char c; int k;
                        cin >> c >> k;

                        if( c == 'A')
                                ct.insert(new A(k));
                        else
                                ct.insert(new B(k));
                }
                for_each(ct.begin(),ct.end(),Print);
                cout << "****" << endl;
        }
}