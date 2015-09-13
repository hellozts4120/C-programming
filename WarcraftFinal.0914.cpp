    #include <iostream>  
    #include <cstdio>  
    #include <string>  
	#include <iomanip>
    using namespace std;  
    const int  WARRIOR_NUM = 5;  
	//const string weapon[3] = {"sword","bomb","arrow"};
	int nTime = 0;  
	int ArrowR;
	int LionK;
	int TimeEnd;
	int nCity;
    /* 
    string Warrior::names[WARRIOR_NUM] = { "dragon","ninja","iceman","lion","wolf" }; 
    红方司令部按照 iceman、lion、wolf、ninja、dragon 的顺序制造武士。 
    蓝方司令部按照 lion、dragon、ninja、iceman、wolf 的顺序制造武士。 
    */  
      
    class Headquarter;  
	class City;
	class Weapon		//新增武器类
	{
		public:
			int atkValue;
			int useFlag;
			Weapon(int atk = 1):atkValue(atk),useFlag(atk){};
			virtual int updateWeapon() = 0;
			virtual int TellRemain();
	};

	class Sword:public Weapon
	{
		public:
			Sword(int atk):Weapon(atk * 0.2){};
			int updateWeapon(){
				atkValue *= 0.8;
				useFlag = atkValue;
				return useFlag > 0;
			}
			int TellRemain(){
				return atkValue;
			}
	};

	class Bomb:public Weapon
	{
		public:
			int updateWeapon(){
				return useFlag = 0;
			}
			int TellRemain(){
				return useFlag;
			}
	};

	class Arrow:public Weapon
	{
		public:
			int ArrowNum;
			Arrow(int _Arrow = 3):ArrowNum(_Arrow){};
			int updateWeapon(){
				ArrowNum--;
				useFlag = ArrowNum;
				return useFlag > 0;
			}
			int TellRemain(){
				return ArrowNum;
			}
	};

    class Warrior  
    {  
        private:  
            Headquarter * pHeadquarter;  
            int kindNo; //武士的种类编号 0 dragon 1 ninja 2 iceman 3 lion 4 wolf  
            int no;  
		protected:
			Weapon *wepn[3];
        public:  
            static string names[WARRIOR_NUM];  
            static int initialLifeValue [WARRIOR_NUM];  
			static int initialAttackValue [WARRIOR_NUM];
            Warrior( Headquarter * p,int no_,int kindNo_ );  
            void PrintResult(int nTime); 
			~Warrior();
    };  

	class dragon:public Warrior
	{
		public:
			int wepnnum;
			float morale;
			dragon(Headquarter * p,int n,int l,float mor):Warrior(p,n,l){
				wepnnum = n % 3;
				switch (wepnnum)
				{
					case 0:wepn[0] = new Sword(Warrior::initialAttackValue[0]);break;
					case 1:wepn[1] = new Bomb();break;
					case 2:wepn[2] = new Arrow();break;
				}
				morale = mor;
				PrintResult(nTime);
				show();
			}
			void show(){
				switch (wepnnum)
				{
					case 0:cout << "It has a sword,and it's morale is " << fixed << setprecision(2) << morale << endl;break;
					case 1:cout << "It has a bomb,and it's morale is " << fixed << setprecision(2) << morale << endl;;break;
					case 2:cout << "It has a arrow,and it's morale is " << fixed << setprecision(2) << morale << endl;break;
				}
			}
	};

	class ninja:public Warrior
	{
		public:
			int wepnnum;
			int wepnnum2;
			ninja(Headquarter * p,int n,int l):Warrior(p,n,l){
				wepnnum = n % 3;
				switch (wepnnum)
				{
					case 0:wepn[0] = new Sword(Warrior::initialAttackValue[1]);break;
					case 1:wepn[1] = new Bomb();break;
					case 2:wepn[2] = new Arrow();break;
				}
				wepnnum = n % 3;
				switch (wepnnum2 = wepnnum + 1)
				{
					case 0:wepn[0] = new Sword(Warrior::initialAttackValue[1]);break;
					case 1:wepn[1] = new Bomb();break;
					case 2:wepn[2] = new Arrow();break;
				}
				PrintResult(nTime);
				show();
			}
			void show(){
				switch (wepnnum)
				{
					case 0:cout << "It has a sword";break;
					case 1:cout << "It has a bomb";break;
					case 2:cout << "It has a arrow";break;
				}
				switch (wepnnum2)
				{
					case 0:cout << " and a sword"<< endl;break;
					case 1:cout << " and a bomb" << endl;;break;
					case 2:cout << " and a arrow"<< endl;break;
				}
			}
	};

	class iceman:public Warrior
	{
		public:
			int wepnnum;
			iceman(Headquarter * p,int n,int l):Warrior(p,n,l){
				wepnnum = n % 3;
				switch (wepnnum)
				{
					case 0:wepn[0] = new Sword(Warrior::initialAttackValue[2]);break;
					case 1:wepn[1] = new Bomb();break;
					case 2:wepn[2] = new Arrow();break;
				}
				PrintResult(nTime);
				show();
			}
			void show(){
				switch (wepnnum)
				{
					case 0:cout << "It has a sword"<< endl;break;
					case 1:cout << "It has a bomb" << endl;;break;
					case 2:cout << "It has a arrow"<< endl;break;
				}
			}
	};

	class lion:public Warrior
	{
		public:
			int loyalty;
			lion(Headquarter * p,int n,int l,int loyal):Warrior(p,n,l){
				loyalty = loyal;
				PrintResult(nTime);
				show();
			}
			void show(){
				cout << "It's loyalty is " << loyalty << endl;
			}
	};

	class wolf:public Warrior
	{
		public:
			wolf(Headquarter * p,int n,int l):Warrior(p,n,l){
				PrintResult(nTime);
			}
			void show(){};
	};

    class Headquarter  
    {  
        private:  
            int totalLifeValue;  
            bool stopped;  
            int totalWarriorNum;  
            int color;  
            int curMakingSeqIdx; //当前要制造的武士是制造序列中的第几个  
            int warriorNum[WARRIOR_NUM]; //存放每种武士的数量  
            Warrior * pWarriors[1000];  
        public:  
            friend class Warrior;  
            static int makingSeq[2][WARRIOR_NUM]; //武士的制作顺序序列  
            void Init(int color_, int lv);  
            ~Headquarter () ;  
            int Produce(int nTime);  
            string GetColor();  
      
    };  
      
    
	Warrior::~Warrior(){
		int i;
		for(i = 0;i <= 2;i++){
			if(wepn[i]){
				delete wepn[i];
			}
		}
	};

    Warrior::Warrior( Headquarter * p,int no_,int kindNo_ ) {  
        no = no_;  
        kindNo = kindNo_;  
        pHeadquarter = p;  
    }  
      
    void Warrior::PrintResult(int nTime)  
    {  
        string color =  pHeadquarter->GetColor();  
        printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n"  ,  
            nTime, color.c_str(), names[kindNo].c_str(), no, initialLifeValue[kindNo],  
            pHeadquarter->warriorNum[kindNo],names[kindNo].c_str(),color.c_str());
    }  
      
    void Headquarter::Init(int color_, int lv)  
    {  
        color = color_;  
        totalLifeValue = lv;  
        totalWarriorNum = 0;  
        stopped = false;  
        curMakingSeqIdx = 0;  
        for( int i = 0;i < WARRIOR_NUM;i++ )  
            warriorNum[i] = 0;  
    }  
      
    Headquarter::~Headquarter () {  
        for( int i = 0;i < totalWarriorNum;i++ )  
            delete pWarriors[i];  
    }  
      
    int Headquarter::Produce(int nTime)  
    {  
      
        if( stopped )  
            return 0;  
        int searchingTimes = 0;  
        while( Warrior::initialLifeValue[makingSeq[color][curMakingSeqIdx]] > totalLifeValue &&  
            searchingTimes < WARRIOR_NUM ) {  
            curMakingSeqIdx = ( curMakingSeqIdx + 1 ) % WARRIOR_NUM;  
            searchingTimes++;  
        }  
        int kindNo = makingSeq[color][curMakingSeqIdx];  
        if( Warrior::initialLifeValue[kindNo] > totalLifeValue ) {  
            stopped = true;  
            if( color == 0)  
                printf("%03d red headquarter stops making warriors\n",nTime);  
            else  
                printf("%03d blue headquarter stops making warriors\n",nTime);  
            return 0;  
        }  
        //制作士兵：  
        totalLifeValue -= Warrior::initialLifeValue[kindNo];  
        curMakingSeqIdx = ( curMakingSeqIdx + 1 ) % WARRIOR_NUM;
		warriorNum[kindNo]++;  
		if(kindNo == 0){
			pWarriors[totalWarriorNum] = new dragon( this,totalWarriorNum+1,kindNo,(float(totalLifeValue) / (Warrior::initialLifeValue[kindNo])));  
		}
		else if(kindNo == 1){
			pWarriors[totalWarriorNum] = new ninja( this,totalWarriorNum+1,kindNo);
		}
		else if(kindNo == 2){
			pWarriors[totalWarriorNum] = new iceman( this,totalWarriorNum+1,kindNo);
		}
		else if(kindNo == 3){
			pWarriors[totalWarriorNum] = new lion( this,totalWarriorNum+1,kindNo,totalLifeValue);
		}
		else if(kindNo == 4){
			pWarriors[totalWarriorNum] = new wolf( this,totalWarriorNum+1,kindNo);
		}

        //pWarriors[totalWarriorNum]->PrintResult(nTime);  
        totalWarriorNum++;  
        return 1;  
    }  
      
    string Headquarter::GetColor()  
    {  
        if( color == 0)  
            return "red";  
        else  
            return "blue";  
    }  
      
    string Warrior::names[WARRIOR_NUM] = { "dragon","ninja","iceman","lion","wolf" };  
    int Warrior::initialLifeValue [WARRIOR_NUM];  
	int Warrior::initialAttackValue [WARRIOR_NUM];
    int Headquarter::makingSeq[2][WARRIOR_NUM] = { { 2,3,4,1,0 },{3,0,1,2,4} }; //两个司令部武士的制作顺序序列  
      
    int main()  
    {  
        int t;  
        int m;  
        Headquarter RedHead,BlueHead;  
        cin >> t; 
        int nCaseNo = 1;  
        while ( t-- ) {  
            printf("Case:%d\n",nCaseNo++);  
			cin >> m >> nCity >> ArrowR >> LionK >> TimeEnd; 
            for( int i = 0;i < WARRIOR_NUM;i++ ){
                cin >> Warrior::initialLifeValue[i];
			}
			for( int i = 0;i < WARRIOR_NUM;i++ ){
				cin >> Warrior::initialAttackValue[i];
			}
            RedHead.Init(0,m);  
            BlueHead.Init(1,m);  
            while(true) {  
                int tmp1 = RedHead.Produce(nTime);  
                int tmp2 = BlueHead.Produce(nTime);  
                if( tmp1 == 0 && tmp2 == 0)  
                    break;  
                nTime++;  
            }  
			nTime = 0;
        }  
        return 0;  
    }  