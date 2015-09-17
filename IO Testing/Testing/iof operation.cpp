#include<iostream>
#include<fstream>
#include<cstring>
using namespace std;

class CSstudent{
	public:
		char szNames[20];
		int nScore;
};

int main(){
	CSstudent s;
	ofstream OutFile("students.dat",ios::app|ios::binary);
	while(cin >> s.szNames >> s.nScore){
		if(stricmp(s.szNames,"exit") == 0){
			break;
		}
		OutFile.write((char *)&s,sizeof(s));
	}
	OutFile.close();
	return 0;
}
