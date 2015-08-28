#include<cstdio>
#include<iostream>
using namespace std;

class Headquarter;
int M,gametime = 0;		//M����ʼ����Ԫ��Gametime����Ϸʱ��		
char name[5][7] = {"dragon","ninja","iceman","lion","wolf"};	//5����ʿ����
int order[2][5]={{2,3,4,1,0},{3,0,1,2,4}};
char Headname[2][5] = {"red","blue"};
int num[2];		//������ʿ����
int next[2];	//��һ��������
int hpini[5];	//������������ֵ��ʼ
Headquarter* head[2];

class Headquarter{
	public:
		int LifeCoin;	//ʣ������Ԫ
		int WarriorNum[5];	//��ʿ����	
		int Side;	//��Ӫ
		int i;

	Headquarter(int SideColor){
		Side = SideColor;
		LifeCoin = M;
		for(i = 0;i < 5;i++){
			WarriorNum[i] = 0;
		}
	}
	bool BulidWarrior(){
		int sort = order[Side][next[Side] % 5];
		if(LifeCoin - hpini[sort] >= 0){
			LifeCoin -= hpini[sort];
			WarriorNum[sort]++;
			printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",gametime,Headname[Side],name[sort],num[sort],hpini[sort],WarriorNum[sort],name[sort],Headname[Side]);
			next[Side]++;
			num[Side]++;
			return true;
		}
		else if(LifeCoin - hpini[order[Side][(next[Side] + 1) % 5]] >= 0){
			sort = order[Side][(next[Side] + 1) % 5];
			LifeCoin -= hpini[sort];
			WarriorNum[sort]++;
			printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",gametime,Headname[Side],name[sort],num[sort],hpini[sort],WarriorNum[sort],name[sort],Headname[Side]);
			next[Side]+= 2;
			num[Side]++;
			return true;
		}
		else if(LifeCoin - hpini[order[Side][(next[Side] + 2) % 5]] >= 0){
			sort = order[Side][(next[Side] + 2) % 5];
			LifeCoin -= hpini[sort];
			WarriorNum[sort]++;
			printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",gametime,Headname[Side],name[sort],num[sort],hpini[sort],WarriorNum[sort],name[sort],Headname[Side]);
			next[Side]+= 3;
			num[Side]++;
			return true;
		}
		else if(LifeCoin - hpini[order[Side][(next[Side] + 3) % 5]] >= 0){
			sort = order[Side][(next[Side] + 3) % 5];
			LifeCoin -= hpini[sort];
			WarriorNum[sort]++;
			printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",gametime,Headname[Side],name[sort],num[sort],hpini[sort],WarriorNum[sort],name[sort],Headname[Side]);
			next[Side]+= 4;
			num[Side]++;
			return true;
		}
		else if(LifeCoin - hpini[order[Side][(next[Side] + 4) % 5]] >= 0){
			sort = order[Side][(next[Side] + 4) % 5];
			LifeCoin -= hpini[sort];
			WarriorNum[sort]++;
			printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",gametime,Headname[Side],name[sort],num[sort],hpini[sort],WarriorNum[sort],name[sort],Headname[Side]);
			next[Side]+= 5;
			num[Side]++;
			return true;
		}
		else {
			printf("%03d %s headquarter stops making warriors\n",gametime,Headname[Side]);
			return false;
		}
	}

};	//class:Headquater


int main()
{
	int cases,casenum,i,stop[2];
	scanf("%d",&cases);
	for(casenum = 1;casenum <= cases;casenum++){
		num[0] = num[1] = 0;
		next[0] = next[1] = 0;
		stop[0] = stop[1] = 1;
		scanf("%d",&M);
		for(i = 0;i < 5;i++){
			scanf("%d",&hpini[i]);
		}
		head[0] = new Headquarter(0);
		head[1] = new Headquarter(1);
		printf("Case:%d\n",casenum);
		for(gametime = 0;stop[0] || stop[1];gametime++){
			if(head[0]->BulidWarrior() == false){
				stop[0] = 0;
			}
			if(head[1]->BulidWarrior() == false){
				stop[1] = 0;
			}
		}
	}
	return 0;
}



