#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class CMy_add{
public:
	int &value;
	CMy_add(int &m):value(m){};
	void operator()(int x){
		while(x >= 8){
			x -= 8;
		}
		value += x;
	}
};

int main(int argc, char* argv[]) {
        int v, my_sum=0;
        vector<int> vec;
        cin>>v;
        while ( v ) {
                vec.push_back(v);
                cin>>v;
        }
        for_each(vec.begin(), vec.end(), CMy_add(my_sum));	//ÊµÖÊÎªCMy_add(iterator);
        cout<<my_sum<<endl;
        return 0;
}