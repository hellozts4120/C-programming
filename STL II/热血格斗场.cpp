#include<iostream>
#include<map>
using namespace std;

typedef map<int,int> MAP;

int main(){
	MAP member;
	member.insert(make_pair(1000000000,1));		//ʹ��strength��key�ؼ���,��Ϊ��Ŀ�Ѿ������� ʵ��ֵ��������� ������
	int strength,id;							//Ҳ��ʹ��id��key�ؼ���,������ÿһ�ζ���Ҫ��������һ��map,�������TLE
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