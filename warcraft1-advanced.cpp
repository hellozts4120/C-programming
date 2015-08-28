#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<string>
using namespace std;
const int WARNUM = 5;

class Headquarter;
class Warrior{
	private:
		Headquarter * head;
		int WarriorNum;
		int num;
	public:
		static string names[WARNUM];
		static int hpini[WARNUM];
		Warrior(Headquarter * p,int num_,int warriornum_);
		void PrintResult(int gametime);
};

class Headquarter{
	private:
		int LifeCoin;
		bool stopped;
		int totalWarrior;
		int color;
		int curMakingSeqIdx;	//当前要制造的武士是武士序列中的第几个;
		int WarriorKind[WARNUM];		//存放每种武士的数量;
		Warrior * warrior[1000];
	public:
		friend class Warrior;
        static int makingSeq[2][WARNUM]; //武士的制作顺序序列  
		void Init(int color_,int lv);
		int Produce(int gametime);
		string GetColor();
		~Headquarter();

};

string Warrior::names[WARNUM] = {"dragon","ninja","iceman","lion","wolf"};
int Warrior::hpini[WARNUM];			//static对象变量要在全局先初始化
int Headquarter::makingSeq[2][WARNUM] = { { 2,3,4,1,0 },{3,0,1,2,4} };

Warrior::Warrior(Headquarter * p, int num_, int warriornum_){
	num = num_;
	WarriorNum = warriornum_;
	head = p;
}
void Warrior::PrintResult(int gametime){
	string color = head->GetColor();
	printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",gametime,color.c_str(),names[WarriorNum].c_str(),num,hpini[WarriorNum],head->WarriorKind[WarriorNum],names[WarriorNum].c_str(),color.c_str());
}


void Headquarter::Init(int color_, int m_){
	int i = 0;
	color = color_;
	LifeCoin = m_;
	totalWarrior = 0;
	stopped = false;
	curMakingSeqIdx = 0;
	for(i = 0;i < WARNUM;i++){
		WarriorKind[i] = 0;
	}
}
int Headquarter::Produce(int gametime){
	if(stopped){
		return 0;
	}
	int SearchTimes = 0;
	while((Warrior::hpini[makingSeq[color][curMakingSeqIdx]] > LifeCoin) && (SearchTimes < WARNUM)){
		SearchTimes++;
		curMakingSeqIdx = (curMakingSeqIdx + 1) % WARNUM;
	}
	int Kind = makingSeq[color][curMakingSeqIdx];
	if(Warrior::hpini[Kind] > LifeCoin){
		stopped = true;
		if(color == 0){
			printf("%03d red headquarter stops making warriors\n",gametime);
		}
		if(color == 1){
			printf("%03d blue headquarter stops making warriors\n",gametime);
			return 0;
		}
	}
		//生产武士
		LifeCoin -= Warrior::hpini[Kind];
		curMakingSeqIdx = (curMakingSeqIdx + 1) % WARNUM;
		warrior[totalWarrior] = new Warrior( this,totalWarrior + 1,Kind);
		WarriorKind[Kind]++;
		warrior[totalWarrior] -> PrintResult(gametime);
		totalWarrior++;
		return 1;
}
string Headquarter::GetColor(){
	if(color == 0){
		return "red";
	}
	else{
		return "blue";
	}
}
Headquarter::~Headquarter(){
	for( int i = 0;i < totalWarrior;i++ )  
        delete warrior[i];  
}  


int main()
{
	int cases;
	int m;
	int i;
	int CurCase = 0;
	int gametime;
	int token1,token2;
	Headquarter BlueH,RedH;
	scanf("%d",&cases);
	while(CurCase < cases){
		CurCase++;
		scanf("%d",&m);
		for(i = 0;i < WARNUM;i++){
			scanf("%d", &Warrior::hpini[i]);
		}
		RedH.Init(0,m);
		BlueH.Init(1,m);
		gametime = 0;
		printf("Case:%d\n",CurCase);
		while(1){
			token1 = RedH.Produce(gametime);
			token2 = BlueH.Produce(gametime);
			if(token1 == 0 && token2 == 0){
				break;
			}
			gametime++;
		}
	}
	system("pause");
}