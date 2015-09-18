#include<iostream>
#include<iomanip>
using namespace std;
int main(){
	double a;
	cin >> a;
	cout <<setiosflags(ios::fixed)<< setprecision(5) << a << endl;
	cout << setprecision(7)<< scientific <<a << endl;
	return 0;
}