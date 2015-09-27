#include<iostream>
#include<map>
#include<cstring>
#include<string>
using namespace std;

class CStudent{
	public:
		struct  Cinfo
		{
			int id;
			string name;
		};
		int score;
		Cinfo info;
};
typedef multimap<int,CStudent::Cinfo>MAP_STD;

int main(){
	MAP_STD mp;
	CStudent st;
	string cmd;
	while((cin >> cmd)){
		if(cmd == "Add"){
			cin >> st.info.name >> st.info.id >> st.score;
			mp.insert(MAP_STD::value_type(st.score,st.info));
		}
		else if(cmd == "Query"){
			int score;
			cin >> score;
			MAP_STD::iterator p = mp.lower_bound(score);
			if(p != mp.begin()){
				--p;
				score = p->first;
				MAP_STD::iterator maxp = p;
				int maxid = p->second.id;
				for(;p != mp.begin() && p->first == score;--p){
					if(p->second.id > maxid){
						maxp = p;
						maxid = p->second.id;
					}
				}
				if(p->first == score){
					if(p->second.id > maxid){
						maxp = p;
						maxid = p->second.id;
					}
				}
				cout << maxp->second.name << " " << maxp->second.id << " " << maxp->first << endl;
			}
			else cout << "Nobody" << endl;
		}
		else if(cmd == "End"){
			break;
		}
	}
	return 0;
}
