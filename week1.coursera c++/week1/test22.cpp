#include<iostream>
#include<string>
#include<cstdio>
using namespace std;

class Student
{
public:
	void Input()
	{
		cin.getline(name, 100, ',');
		cin >> age;
		a = getchar();
		cin.getline(nums, 100, ',');
		cin >> g1 >> a >> g2 >> a >> g3 >> a >> g4;
	}
	void average()
	{
		aver = (g1 + g2 + g3 + g4) / 4;
	}
	void Output()
	{
		cout << name << "," << age << "," << nums << "," << aver;
		cout << endl;
	}
private:
	char name[100], nums[100], a;
	unsigned int age;
	int g1, g2, g3, g4;
	int aver;
};
int main()
{
	Student stu;
	stu.Input();
	stu.average();
	stu.Output();

	return 0;
}