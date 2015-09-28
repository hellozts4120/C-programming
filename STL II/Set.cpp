#include<set>
#include<string>
#include<iostream>
using namespace std;

int main(){
	string input;
	int num,n;
	multiset<int>MSet;
	set<int>Set;
	cin >> n;
	while(n--){
		cin >> input >> num;
		if(input == "add"){
			Set.insert(num);
			MSet.insert(num);
			cout << MSet.count(num) << endl;
		}
		else if(input == "del"){
			cout << MSet.count(num) << endl;
			MSet.erase(num);
		}
		else if(input == "ask"){
			if(Set.count(num)){
				cout << "1 ";
			}
			else cout << "0 ";
			cout << MSet.count(num) << endl;
		}
	}
	return 0;
}