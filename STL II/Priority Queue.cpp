#include<iostream>
#include<set>
#include<cmath>
using namespace std;

bool isPrime(int num){
	for(int i = 2;i <= sqrt(num);i++){
		if(num % i == 0){
			return false;
		}
	}
	return true;
}

int DivideFactor(int num){
	int sum = 0;
	int temp;
	for(int i = 2;i <= sqrt(num);i++){
		if(num % i == 0){
			temp = num / i;
			if(i != temp && isPrime(temp)){
				sum += 1;
			}
			if(isPrime(i)){
				sum += 1;
			}
		}
	}
	return sum;
}

struct Compare{
	bool operator()(const int &a,const int &b){
		int FactorA,FactorB;
		FactorA = DivideFactor(a);
		FactorB = DivideFactor(b);
		if(FactorA < FactorB){
			return true;
		}
		else if(FactorA > FactorB){
			return false;
		}
		else{
			if(a < b) return true;
			else return false;
		}
	}
};

int main(){
	int n,num;
	set<int,Compare>P_queqe;
	cin >> n;
	while(n--){
		for(int i = 0;i < 10;i++){
			cin >> num;
			P_queqe.insert(num);
		}
		int min = *(P_queqe.begin());
		int max = *(P_queqe.rbegin());
		P_queqe.erase(max);
		P_queqe.erase(min);
		cout << max << " " << min << endl;
	}
	return 0;
}