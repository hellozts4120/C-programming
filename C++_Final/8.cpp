#include<iostream>
using namespace std;

int main(){
	int n,i,j,t;
	cin >> t;
	while(t--){
		cin >> n >> i >> j;
		int result = 0,vi,vj;
		vi = (n >> i) & 1;
		vj = (n >> j) & 1;
		result |= vi << i;
		if (vj == 0) result |= 1 << j;
		for(int k = i+1; k < j;k++){
			result |= 1 << k;
		}
		cout << hex << result << endl;
	}
	return 0;
}