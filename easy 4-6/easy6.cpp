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
	}			//���ƹ��캯����д�ɲ�д
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
		return p + i * c;	//�˴��ص�
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
            a[i][j] = i * 4 + j;	//ֻ��[i]�Ǹ���a�ģ���ֻ��[i]�е�[]��Ҫ���أ�[j]�е�[]Ĭ��Ϊ return p+j
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