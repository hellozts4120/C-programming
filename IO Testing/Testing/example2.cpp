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
	ifstream inFile("students.dat",ios::in|ios::binary);
	if(!inFile){
		cout << "error!" << endl;
		return 0;
	}
	while(inFile.read((char*)&s,sizeof(s))){
		int nReadedBytes = inFile.gcount();		//看读了多少字节
		cout << s.szNames << " " << s.nScore << endl;
		cout << nReadedBytes << endl;
	}
	inFile.close();
	return 0;
}