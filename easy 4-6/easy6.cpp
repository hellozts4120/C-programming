#include <iostream>
#include <cstring>
using namespace std;

class Array2{
private:
	int *p;
	int r,c;
public:
	Array2(){p = NULL;}
	Array2(int r_, int c_){
		r = r_;
		c = c_;
		p = new int[r * c];
	}
	Array2(const Array2 & arr2){
		r = arr2.r;
		c = arr2.c;
		p = new int[r * c];
		memcpy(p,arr2.p,sizeof(int) * (r * c));
	}			//复制构造函数可写可不写
	Array2 & operator=(const Array2 & arr2){
		if(p){
			delete []p;
		}
		r = arr2.r;
		c = arr2.c;
		p = new int[r * c];
		memcpy(p,arr2.p,sizeof(int) * (r * c));
		return *this;
	}
	~Array2(){
		if(p)
			delete []p;
	}
	int * operator[](int i){
		return p + i * c;	//此处重点
	}
	int & operator()(int i,int j){
		return p[i * c + j];
	}
};
	

	int main() {
    Array2 a(3,4);
    int i,j;
    for(  i = 0;i < 3; ++i )
        for(  j = 0; j < 4; j ++ )
            a[i][j] = i * 4 + j;	//只有[i]是跟着a的，故只有[i]中的[]需要重载，[j]中的[]默认为 return p+j
    for(  i = 0;i < 3; ++i ) {
        for(  j = 0; j < 4; j ++ ) {
            cout << a(i,j) << ",";
        }
        cout << endl;
    }
    cout << "next" << endl;
    Array2 b;     b = a;
    for(  i = 0;i < 3; ++i ) {
        for(  j = 0; j < 4; j ++ ) {
            cout << b[i][j] << ",";
        }
        cout << endl;
    }
    return 0;
}