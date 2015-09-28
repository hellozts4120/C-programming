#include<iostream>
#include<map>
using namespace std;

typedef map<int,int> MAP;

int main(){
	MAP member;
	member.insert(make_pair(1000000000,1));		//使用strength做key关键字,因为题目已经给出了 实力值不可能相等 的条件
	int strength,id;							//也可使用id做key关键字,但这样每一次都需要完整遍历一次map,容易造成TLE
	int n;
	int low,high;
	cin >> n;
	while(n--){
		cin >> id >> strength;
		member.insert(make_pair(strength,id));
		MAP::iterator i,j,k;
		i = member.find(strength);
		if(i != member.end()){
			if(i == member.begin()){
				j = i;
				j++;
				cout << i->second << " " << j->second << endl;
			}
			else{
				j = i;
				k = i;
				j++;
				k--;
				low = i->first - k->first;
				high = j->first - i->first;
				if(low <= high){
					cout << i->second << " " << k->second << endl;
				}
				else{
					cout << i->second << " " << j->second << endl;
				}
			}
		}
		else continue;
	}
	return 0;
}