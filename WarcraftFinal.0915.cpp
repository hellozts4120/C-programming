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
			virtual int TellRemain() = 0;
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
		public:  
            Headquarter * pHeadquarter;  
            int kindNo; //武士的种类编号 0 dragon 1 ninja 2 iceman 3 lion 4 wolf  
            int no;  
			int Life;
			int Atk;
			int dead;
			Weapon *wepn[3];
        public:  
            static string names[WARRIOR_NUM];  
            static int initialLifeValue [WARRIOR_NUM];  
			static int initialAttackValue [WARRIOR_NUM];
            Warrior( Headquarter * p,int no_,int kindNo_ );  
            void PrintResult(int nTime); 
			int AttackEnemy(Warrior *enemy,int time,int city);
			int FightBack(Warrior *enemy,int time,int city);
			int CanUseWeapon(int num);
			void TellWeapon(int time);
			int ShootArrow(int time,City* city,Warrior *enemy);		//need done.
			void UseBomb(Warrior *enemy,int time);
			int WillBeKilled(Warrior *enemy,int first);
			~Warrior();

			virtual int Attack(Warrior *enemy,int time,int city) = 0;
			virtual int FightBack0(Warrior *enemy,int time,int city) = 0;
			//virtual int Escape(int time)
    };  

	class dragon:public Warrior
	{
		public:
			int wepnnum;
			double morale;
			dragon(Headquarter * p,int n,int l,double mor):Warrior(p,n,l){
				wepnnum = n % 3;
				switch (wepnnum)
				{
					case 0:wepn[0] = new Sword(Warrior::initialAttackValue[0]);break;
					case 1:wepn[1] = new Bomb();break;
					case 2:wepn[2] = new Arrow();break;
				}
				morale = mor;
				PrintResult(nTime);
			}
				int Attack(Warrior *enemy,int time,int city){
					AttackEnemy(enemy,time,city);
					if(enemy -> dead){
						morale += 0.2;
					}
					else
						morale -= 0.2;
					return enemy -> dead;
				}
				int FightBack0(Warrior *enemy,int time,int city){
					FightBack(enemy,time,city);
					if(enemy -> dead){
						morale += 0.2;
					}
					else
						morale -= 0.2;
					return enemy -> dead;
				}
				double GetMorale(){
					return morale;
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
			}
			int Attack(Warrior *enemy,int time,int city){
				return AttackEnemy(enemy,time,city);
			}
			int FightBack0(Warrior *enemy,int time,int city){
				return 0;
			}

	};

	class iceman:public Warrior
	{
		public:
			int wepnnum;
			int step;
			iceman(Headquarter * p,int n,int l):Warrior(p,n,l){
				step = 0;
				wepnnum = n % 3;
				switch (wepnnum)
				{
					case 0:wepn[0] = new Sword(Warrior::initialAttackValue[2]);break;
					case 1:wepn[1] = new Bomb();break;
					case 2:wepn[2] = new Arrow();break;
				}
				PrintResult(nTime);
			}
			int Attack(Warrior *enemy,int time,int city){
				return AttackEnemy(enemy,time,city);
			}
			int FightBack0(Warrior *enemy,int time,int city){
				return FightBack(enemy,time,city);
			}
			int Move(){
				step++;
				if(step == 2){
					Life -= 9;
					if(Life <= 0){
						Life = 1;
					}
					Atk += 20;
					step = 0;
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
			}
			int Attack(Warrior *enemy,int time,int city){
				AttackEnemy(enemy,time,city);
				if(!enemy -> dead){
					loyalty -= LionK;
				}
				return (enemy -> dead);
			}
			int FightBack0(Warrior *enemy,int time,int city){
				FightBack(enemy,time,city);
				if(!enemy -> dead){
					loyalty -= LionK;
				}
				return (enemy -> dead);
			}

			//逃跑函数need done!
	};

	class wolf:public Warrior
	{
		public:
			wolf(Headquarter * p,int n,int l):Warrior(p,n,l){
				PrintResult(nTime);
			}
			int Attack(Warrior *enemy,int time,int city){
				AttackEnemy(enemy,time,city);
				if(enemy -> dead){
					GetWeapon(enemy);
				}
				return (enemy -> dead);
			}
			int FightBack0(Warrior *enemy,int time,int city){
				FightBack(enemy,time,city);
				if(enemy -> dead){
					GetWeapon(enemy);
				}
				return (enemy -> dead);
			}
			void GetWeapon(Warrior *enemy){
				if(enemy -> CanUseWeapon(0) && !CanUseWeapon(0)){
					wepn[0] = new Sword(enemy -> wepn[0] ->atkValue);
				}
				if(enemy -> CanUseWeapon(1) && !CanUseWeapon(1)){
					wepn[1] = new Bomb();
				}
				if(enemy -> CanUseWeapon(2) && !CanUseWeapon(2)){
					wepn[2] = new Arrow(enemy -> wepn[2] -> TellRemain());
				}
			}
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
		Life = initialLifeValue[kindNo];
		Atk = initialAttackValue[kindNo];
    }  
     
	int Warrior::CanUseWeapon(int num){
		return (wepn[num] && wepn[num] -> useFlag);
	}

	int Warrior::AttackEnemy(Warrior *enemy,int time,int city){
		string color =  pHeadquarter -> GetColor(); 
		string color1 = enemy -> pHeadquarter -> GetColor();
		printf("%03d:40 %s %s %d attacked %s %s %d in city %d with %d elements and force %dn",time/60,color.c_str(),names[kindNo].c_str(),no,color1.c_str(),names[enemy -> kindNo].c_str(),enemy -> no,city,Life,Atk);	
		int enemyLife = enemy -> Life;
		enemy -> Life -= (Atk + (CanUseWeapon(0) ? wepn[0] -> atkValue : 0));
		if(CanUseWeapon(0)){
			wepn[0] -> updateWeapon();
		}
		enemy -> dead = (enemy -> Life <= 0);
		if(enemy -> kindNo == 3 && enemy -> dead){	//杀狮子加生命
			Life += enemyLife;
		}
		return (enemy -> dead);
	}

	int Warrior::FightBack(Warrior *enemy,int time,int city){
		string color =  pHeadquarter -> GetColor(); 
		string color1 = enemy -> pHeadquarter -> GetColor();
		printf("%03d:40 %s %s %d fought back against %s %s %d in city %dn",time/60,color.c_str(),names[kindNo].c_str(),no,color1.c_str(),names[enemy -> kindNo].c_str(),enemy -> no,city);
		int enemyLife = enemy -> Life;
		enemy -> Life -= (Atk + (CanUseWeapon(0) ? wepn[0] -> atkValue : 0));
		if(CanUseWeapon(0)){
			wepn[0] -> updateWeapon();
		}
		enemy -> dead = (enemy -> Life <= 0);
		if(enemy -> kindNo == 3 && enemy -> dead){	//杀狮子加生命
			Life += enemyLife;
		}
		return (enemy -> dead);
	}

	void Warrior::TellWeapon(int time){
		string color =  pHeadquarter -> GetColor(); 
		int wepnnum = 0;
		printf("%03d:55 %s %s %d has ",time/60,color.c_str(),names[kindNo].c_str(),no);
		if(CanUseWeapon(2)){
			printf("arrow(%d)",wepn[2] -> TellRemain());
		}
		if(CanUseWeapon(1)){
			if(wepnnum++ > 0){
				printf(",bomb");
			}
		}
		if(CanUseWeapon(0)){
			if(wepnnum++ > 0){
				printf(",sword(%d)",wepn[0] -> TellRemain());
			}
		}
		if(wepnnum == 0){
			printf("no weapon");
		}
		printf("\n");
	}

	void Warrior::UseBomb(Warrior *enemy,int time){
		string color =  pHeadquarter -> GetColor(); 
		string color1 = enemy -> pHeadquarter -> GetColor();
		dead = 1;
		enemy -> dead = 1;
		printf("%03d:38 %s %s %d used a bomb and killed %s %s %dn",time/60,color.c_str(),names[kindNo].c_str(),no,color1.c_str(),names[enemy -> kindNo].c_str(),enemy -> no);
	}

	int Warrior::WillBeKilled(Warrior *enemy,int first){
		int AtkAll = Atk + (CanUseWeapon(0) ? wepn[0] -> atkValue : 0);
		if(first){
			return(enemy -> Life > AtkAll && enemy -> kindNo != 1 && Life <= (enemy -> Atk / 2+(enemy -> CanUseWeapon(0) ? enemy -> wepn[0] -> atkValue : 0)));
		}
		else
			return(Life <= (enemy -> Atk +(enemy -> CanUseWeapon(0) ? enemy -> wepn[0] -> atkValue : 0)));
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
			pWarriors[totalWarriorNum] = new dragon( this,totalWarriorNum+1,kindNo,(double(totalLifeValue) / (Warrior::initialLifeValue[kindNo])));  
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