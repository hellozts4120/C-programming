#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <windows.h>
#include<time.h>
#include <fstream>
#include<Mmsystem.h>  
#include<iostream>
using namespace std;
#pragma comment(lib,"winmm.lib")
int main()
{
HANDLE  hConsole;  
hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
struct player
{	char name[100];
	char password[100];
	int countright;
	int round;
	int score;
	int timecount;}play[50];
char s[1000],input[100],output=0,mima[1000],tempo[100];int n,k,music1,music2,music3,i,up,j,temp,temp1,u=-1,v=0;
ifstream fin("1.txt");
 while(!(fin.eof())){
 fin>>s;                 
 for(j=0;s[j]!='\0';j++)
 {		if(s[j]=='*') printf("\n");
 else
		printf("%c",s[j]);
 }
 }
srand(time(NULL));
time_t timeBegin, timeEnd;
music1=rand()%5;
switch (music1)
{
	case 0:PlaySound(TEXT("sound\\start1.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
	case 1:PlaySound(TEXT("sound\\start2.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
	case 2:PlaySound(TEXT("sound\\start3.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
	case 3:PlaySound(TEXT("sound\\start4.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
	case 4:PlaySound(TEXT("sound\\start5.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
}

for(i=0;i<50;i++)
{
	play[i].score=0;play[i].countright=0;play[i].round=0;
}
printf("��Ϸ����������Ļ������ֵĴ�дӢ����ĸ��Сдֵ����Ϸ����ʱ�κ�ʱ�����롰*����exit������Ϊ�˳���Ϸ����\n");
printf("������������֣������롰*����ʾ�˳���Ϸ��");
gets(play[0].name);
if(play[0].name[0]=='*'&&play[0].name[1]=='\0')
{exit(0);}
else
printf("���ε�¼���û��������������룺");
gets(play[0].password);
printf("��ӭ��%s���޸����������롰$�������������ַ��ɿ�ʼ��Ϸ��",play[0].name);
gets(tempo);
if(tempo[0]=='$'&&tempo[1]=='\0')
{
	printf("������ԭ���룺");
	gets(mima);
	for(k=0,temp=1;play[0].password[k]!='\0';)
	{
		 if(mima[k]!=play[0].password[k]){
			printf("����������������룡\n");
			printf("��������û��������룺");
			gets(mima);
			k=0;}
		 if(play[0].password[temp]=='\0'&&mima[temp]!='\0'){
			printf("����������������룡\n");
			printf("��������û��������룺");
			gets(mima);
			k=0;}

		 else
			 k++;
			 temp++;
	}
	printf("�������µ����룺");
	gets(play[0].password);}
	{
		system("cls");
		printf("������Сд��ĸ�����롰*����.�������˳���Ϸ��\n");
		printf("����������ȷ+2�֣�����-1�֣����������У�ͬ��ʱ����ʱ�䳤�̾����Ⱥ�\n");		//�Դ����ܣ��Ľ�����������
		timeBegin = time(NULL);
	for(i=0,n=0;;play[i].round++)
	{	
		
		printf("The character is:%c\n",output=65+rand()%26);
		gets(input);

	if(input[0]==output+32&&input[1]=='\0')
	{
		printf("Excellent!\n");
		play[i].score+=2;
		play[i].countright++;
				music2=rand()%4;
		switch (music2)
{							//�Դ����ܣ�������������ȷʱ����Ч
	case 0:PlaySound(TEXT("sound\\victory1.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
	case 1:PlaySound(TEXT("sound\\victory2.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
	case 2:PlaySound(TEXT("sound\\victory3.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
	case 3:PlaySound(TEXT("sound\\victory4.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
}
		continue;
	}
	else if((input[0]=='*'||input[0]=='.')&&input[1]=='\0')
	{   timeEnd = time(NULL);
		play[i].timecount=timeEnd-timeBegin;
		music3=rand()%3;
		switch (music3)
		{
		case 0:PlaySound(TEXT("sound\\end1.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
		case 1:PlaySound(TEXT("sound\\end2.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
		case 2:PlaySound(TEXT("sound\\end3.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
		}
		if(i!=0)
{		play[i].round--;
		up=0;
		while(up<i+1)
		{for(k=i+1,j=i;j>up;j--)
				if((play[j].score>play[j-1].score))
				{
					play[50]=play[j];play[j]=play[j-1];play[j-1]=play[50];
		
		k=j;}

				up=k;
		}
				up=0;
		while(up<i+1)
		{for(k=i+1,j=i;j>up;j--)
				if((play[j].score==play[j-1].score)&&(play[j].timecount<play[j-1].timecount))
				{
					play[50]=play[j];play[j]=play[j-1];play[j-1]=play[50];
		
		k=j;}

				up=k;
		}
}
		printf("Game Over!\n");
		printf("\tplayer\tplaynum\twinnum\tscore\ttotaltime\n");
		for(j=0;j<=i;j++)
		{
			printf("\t%s\t%d\t%d\t%d\t%d\n",play[j].name,play[j].round,play[j].countright,play[j].score,play[j].timecount);
		}
		system("pause");
		temp=rand()%256;
		SetConsoleTextAttribute(hConsole, temp);		//�Դ����ܣ��˴�Ϊ�����еı�ɫЧ����
	printf("��Ϸ����������Ļ������ֵĴ�дӢ����ĸ��Сдֵ����Ϸ����ʱ�κ�ʱ�����롰*����exit������Ϊ�˳���Ϸ����\n");
printf("������������֣������롰*����ʾ�˳���Ϸ��");
		n++;
		gets(play[n].name);
		if(play[n].name[0]=='*'&&play[n].name[1]=='\0')
	{
		system("cls");
		printf("�������ַ���ѡ����ܷ�����\n1.�����д�Сд��ĸ���������ֱ�Ϊ���ܷ��ţ�\n2.�����Ϊ����\n�����������Ĭ�ϲ����ܣ�\n");
		gets(tempo);
{
		if(tempo[0]=='1'&&tempo[1]=='\0')
			for(temp=0;temp<=i;temp++)
			{
				for(j=0;play[temp].password[j];j++){
					if(play[temp].password[j]>='0'&&play[temp].password[j]<='9'){play[temp].password[j]-=15;}
					if(play[temp].password[j]>='A'&&play[temp].password[j]<='Z'){play[temp].password[j]+=32;}
					if(play[temp].password[j]>='a'&&play[temp].password[j]<='z'){play[temp].password[j]-=32;}
				}
				printf("%s\n",play[temp].password);
			}
		if(tempo[0]=='2'&&tempo[1]=='\0')
			for(temp=0;temp<=i;temp++)
			{
				for(j=0,temp1=0;play[temp].password[j];temp1++,j++){
					s[j]=play[temp].password[j];}
				s[j]='\0';
				temp1--;
				for(j=0;temp1>=0;j++,temp1--){
					play[temp].password[j]=s[temp1];}
				play[temp].password[j]='\0';
				printf("%s\n",play[temp].password);
			}
}
			printf("���ܳɹ���\n");
			system("pause");
		ofstream fout("1.txt");
		for(j=0;j<=i;j++)
		{	
			fout<<play[j].name<<"*"<<play[j].round<<"*"<<play[j].countright<<"*"<<play[j].score<<"*"<<play[j].timecount<<"*"<<"\n";
		}
		system("pause");
			exit(0);
	}
		else
			u=-1;
			for(temp=0;temp<=i;temp++)
			{	for(k=0;play[temp].name[k]!='\0';k++)
			{	if(play[n].name[k]==play[temp].name[k]) {u=temp;}
				if(play[n].name[k]!=play[temp].name[k])
				{u=-1;
				break;	}
				v=k+1;
				if(play[n].name[v]!='\0'&&play[temp].name[v]=='\0')
				{u=-1;
				break;	}
			}
				if((u==-1)&&(temp==i))
				{
					printf("���ε�¼���û��������������룺");
						gets(play[n].password);
						printf("��ӭ��%s���޸����������롰$�������������ַ��ɿ�ʼ��Ϸ��",play[n].name);
						gets(tempo);
						if(tempo[0]=='$'&&tempo[1]=='\0')
				{	
						printf("������ԭ���룺");
						gets(mima);
	for(k=0,temp=1;play[n].password[k]!='\0';)
	{
		 if(mima[k]!=play[n].password[k]){
			printf("����������������룡\n");
			printf("��������û��������룺");
			gets(mima);
			k=0;}
		 if(play[n].password[temp]=='\0'&&mima[temp]!='\0'){
			printf("����������������룡\n");
			printf("��������û��������룺");
			gets(mima);
			k=0;}

		 else
			 k++;
			 temp++;
	}
					printf("�������µ����룺");
					gets(play[n].password);}
						break;
				}
				if((u==-1)&&(temp!=i))
				{continue;}
				else
					printf("��������û���������%s��",play[u].password);
					gets(mima);
					printf("\n");
				for(k=0,temp=1;play[u].password[k]!='\0';k++,temp++)
				{	if(mima[k]!=play[u].password[k]){
						printf("����������������룡\n");
						printf("��������û��������룺");
						gets(mima);
						k=-1;
						temp=0;
				}
					if(mima[temp]!='\0'&&play[u].password[temp]=='\0'){
						printf("����������������룡\n");
						printf("��������û��������룺");
						gets(mima);
						k=-1;
						temp=0;
				}
				else	
					play[n].password[k]=play[u].password[k];

				}
					play[n].password[k]='\0';
					printf("������ȷ��\n");
					printf("��ӭ��%s���޸����������롰$�������������ַ��ɿ�ʼ��Ϸ��",play[n].name);
						gets(tempo);
						if(tempo[0]=='$'&&tempo[1]=='\0')
				{	
						printf("������ԭ���룺");
						gets(mima);
	for(k=0,temp=1;play[n].password[k]!='\0';)
	{
		 if(mima[k]!=play[n].password[k]){
			printf("����������������룡\n");
			printf("��������û��������룺");
			gets(mima);
			k=0;}
		 if(play[n].password[temp]=='\0'&&mima[temp]!='\0'){
			printf("����������������룡\n");
			printf("��������û��������룺");
			gets(mima);
			k=0;}

		 else
			 k++;
			 temp++;
	}
					printf("�������µ����룺");
					gets(play[n].password);
					strcpy(play[u].password,play[n].password);
						}
					system("pause");
					break;
			}
			printf("%s",play[n].password);
			i++;
		system("cls");
		printf("������Сд��ĸ�����롰*����.�������˳���Ϸ��\n");
		printf("����������ȷ+2�֣�����-1�֣����������У�\n");
				timeBegin = time(NULL);
	}
	else 
{
		music2=rand()%5;
		switch (music2)
{
	case 0:PlaySound(TEXT("sound\\wrong1.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
	case 1:PlaySound(TEXT("sound\\wrong2.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
	case 2:PlaySound(TEXT("sound\\wrong3.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
	case 3:PlaySound(TEXT("sound\\wrong4.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
	case 4:PlaySound(TEXT("sound\\wrong5.wav"),NULL,SND_FILENAME|SND_ASYNC);break;
}
		printf("Wrong input!\n");
		play[i].score--;
}
	}
	}
}