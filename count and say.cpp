#include<cstdio>
#include<iostream>
#include<string>
using namespace std;

string convert(string str){
	string newStr;
	int count = 0;
	char num = str[0];
	char temp;
	for(size_t i = 0;i <= str.size();i++){
		if(str[i] == num){
			count++;
		}
		else{
			temp = count + '0';
			newStr = newStr + temp;
			newStr = newStr + num;
			count = 1;
			num = str[i];
		}
	}
	return newStr;
}

string CountAndSay(int n){
	if(n == 0){
		string voidstr = "";
		return voidstr;
	}
	else{
		string str = "1";
		for(int i = 1;i < n;i++){
			str = convert(str);
		}
		return str;
	}
}

int main()
{
	int n;
	scanf("%d",&n);
	string StrMain;
	StrMain = CountAndSay(n);
	cout << StrMain << endl;
	return 0;
}