#include<stdio.h>
#include<string.h>
#include<iostream>
#include<math.h>
using namespace std;

class StuPresent{
	private:
		char name[100],number[100],age[100],a;int mean,year1,year2,year3,year4;
	public:
		void Input();
		int	MeanCalcu();
		void Output();
};

void StuPresent::Input(){

	a = ',';
	cin.getline(name,100,a);
	cin.getline(age,100,a);
    cin.getline(number,100,a);
    scanf("%d,%d,%d,%d",&year1,&year2,&year3,&year4);
}
int StuPresent::MeanCalcu(){
	float x;
	x = (year1 + year2 + year3 + year4) / 4;
	mean = floor(x);
	return mean;
}
void StuPresent::Output(){
	cout<<name<<","<<age<<","<<number<<","<< mean;
}


void main()
{
	StuPresent SP;
	SP.Input();
	SP.MeanCalcu();
	SP.Output();
}