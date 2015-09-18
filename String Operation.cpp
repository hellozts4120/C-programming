#include<iostream>
#include<string>
#include<cstdlib>
using namespace std;
int cases;
bool Loop = true;
enum{copy1,add,find1,rfind,insert,reset,print,printall,over};
string str[21];

string GetString();
int GetNum();
int Select(string x){		//命令的开头单词只可能是insert,reset,print,printall,over;
	if(x == "copy"){return copy1;}
	if(x == "add"){return add;}
	if(x == "find"){return find1;}
	if(x == "rfind"){return rfind;}
	if(x == "insert"){return insert;}
	if(x == "reset"){return reset;}
	if(x == "print"){return print;}
	if(x == "printall"){return printall;}
	if(x == "over"){return over;}
	return -1;
}

int GetNum(){
	string head;
	string S;
	int N;
	cin >> head;
	switch(Select(head))
	{
		case find1:
			S = GetString();
			N = GetNum();
			if(strstr(str[N].c_str(),S.c_str()) != NULL){
				return str[N].find(S);
			}
			else return S.length();
		case rfind:
			S = GetString();
			N = GetNum();
			if(strstr(str[N].c_str(),S.c_str()) != NULL){
				return str[N].rfind(S);
			}
			else return S.length();
		default:
			return atoi(head.c_str());		//atoi:将字符串转换为整数
	}
}

bool IsInt(string & s){
	if(s.length() > 5){
		return false;		//此时即使为数字也已大于99999
	}
	int len = s.length();
	for(int i = 0;i < len;i++){
		if(isdigit(s[i]) == 0){		//isdigit函数:检测字符串某一位是否为数字0-9;
			return false;
		}
	}
	return true;
}

string GetString(){
	string head;
	string S1,S2;
	int N,X,L;
	cin >> head;
	switch (Select(head))
	{
		case copy1:
			N = GetNum();
			X = GetNum();
			L = GetNum();
			return str[N].substr(X,L);
		case add:
			S1 = GetString();
			S2 = GetString();
			if(IsInt(S1)&&IsInt(S2)){
				int sum;
				char b[15];
				_itoa_s((atoi(S1.c_str())+atoi(S2.c_str())),b,10);
				string x;
				x = b;
				return x;
			}
			else return (S1+S2);
		default:
			return head;
	}
}

void Exam(){
	string head;
	int N,X;
	string S;
	cin >> head;
	switch (Select(head))
	{
		case printall:
				for(int i = 1;i <= cases;i++){
					cout << str[i].c_str() << endl;
				}
				break;
		case print:
				N =GetNum();
				cout << str[N].c_str() << endl;
				break;
		case over:
				Loop = false;
				break;
		case insert:
				S = GetString();
				N = GetNum();
				X = GetNum();
				str[N].insert(X,S);
				break;
		case reset:
				S = GetString();
				N = GetNum();
				str[N] = S;
				break;
		default:;
	}
}

int main(){
	cin >> cases;
	for(int i = 1;i <= cases;i++){
		cin >> str[i];
	}
	while(Loop){
		Exam();
	}
	return 0;
}