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
	fstream iofile("students.dat",ios::in|ios::out|ios::binary);
	if(!iofile){
		cout << "error!" << endl;
		return 0;
	}
	iofile.seekp(2 * sizeof(s),ios::beg);
	iofile.write("Mike",strlen("Mike") + 1);
	iofile.seekg(0,ios::beg);
	while(iofile.read((char*)&s,sizeof(s))){
		cout << s.szNames << " " << s.nScore << endl;
	}
	iofile.close();
	return 0;
}