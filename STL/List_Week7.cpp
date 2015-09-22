#include<iostream>
#include<list>
#include<string>
#include<vector>
#include<cstdio>
using namespace std;

class ListClass{
	public:
		int num;
		ListClass(int a):num(a){};
		list<int>saver;
};

ListClass & FindList(vector<ListClass>&seq,int i){
	int j = seq.size();
	if(j > 0){
		vector<ListClass>::iterator point;
		point = seq.begin();
		return *(point + (i - 1));
	}
}

void PrintAll(const list<int>&saver){
	int j = saver.size();
	if(j > 0){
		list<int>::const_iterator point;
		point = saver.begin();
		for(point;point != saver.end();point++){
			cout << *point << " ";
		}
	}
}


int main(){
	//freopen("in.txt","r",stdin);
	//freopen("fuck.txt","w",stdout);
	int command;
	string cases;
	vector<ListClass>seqSave;
	cin >> command;
	for(int i = 0;i < command;i++){
		cin >> cases;
		if(cases == "new"){
			int id;
			cin >> id;
			seqSave.push_back(ListClass(id));
		}
		else if(cases == "add"){
			int id,num;
			cin >> id;
			cin >> num;
			ListClass & list = FindList(seqSave,id);
			list.saver.push_back(num);
			list.saver.sort();
		}
		else if(cases == "merge"){
			int id1,id2;
			cin >> id1;
			cin >> id2;
			ListClass & list1 = FindList(seqSave,id1);
			ListClass & list2 = FindList(seqSave,id2);
			list1.saver.merge(list2.saver);
			//list1.saver.sort();
		}
		else if(cases == "unique"){
			int id;
			cin >> id;
			ListClass & list = FindList(seqSave,id);
			list.saver.unique();
			//list.saver.sort();
		}
		else if(cases == "out"){
			int id;
			cin >> id;
			ListClass & list = FindList(seqSave,id);
			PrintAll(list.saver);
			cout << endl;
		}
	}
	return 0;
}