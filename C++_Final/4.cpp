#include <iostream>
using namespace std;

class CType{
public:
	CType():val(0){};
	int val;
	void setvalue(int n){
		val = n;
	}
	int operator++(){
		return val;
	}
	friend ostream & operator<<(ostream & os, const CType & a){
		os << (a.val)*(a.val);
		return os;
	}
};

int main(int argc, char* argv[]) {
        CType obj;
        int n;
        cin>>n;
        while ( n ) {
                obj.setvalue(n);
                cout<<obj++<<" "<<obj<<endl;
                cin>>n;
        }
        return 0;
}