#include <iostream>
using namespace std;
template <class T>
class CArray3D{
	template <class T1>
	class CArray2D{
		template <class T2>
		class CArray1D{
			private:
				T2 *p;
				T2 _a;
			public:
				CArray1D():p(NULL){};
				void set(T2 a){
					p = new T2[a];
					_a = a;
				}
				T2 & operator[](int elem) const{
					return p[elem];
				}
				~CArray1D(){
					delete []p;
				}
		};
		private:
			CArray1D <T1> *p;
			T1 _b;
		public:
			CArray2D():p(NULL){};
			void set(T1 a, T1 b){
				p = new CArray1D<T1>[a];
				for(int i = 0;i < a;i++){
					p[i].set(b);
				}
				_b = b;
			}
			CArray1D<T> & operator[](int elem) const{
				return p[elem];
			}
			~CArray2D(){
				delete []p;
			}
	};
	private:
		CArray2D <T> *p;
		T _c;
	public:
		CArray3D(T a,T b,T c){
			p = new CArray2D<T>[a];
			for(int i = 0;i < a;i++){
				p[i].set(b,c);
			}
			_c = c;
		}
		CArray2D<T> & operator[](int elem) const{
			return p[elem];
		}
		~CArray3D(){
			delete []p;
		}
};

int main()
{
    CArray3D<int> a(3,4,5);
    int No = 0;
    for( int i = 0; i < 3; ++ i )
        for( int j = 0; j < 4; ++j )
            for( int k = 0; k < 5; ++k )
                a[i][j][k] = No ++;
    for( int i = 0; i < 3; ++ i )
        for( int j = 0; j < 4; ++j )
            for( int k = 0; k < 5; ++k )
                cout << a[i][j][k] << ",";
    return 0;
}