    #include <iostream>  
    #include <cstdio>  
    #include <string>  
	#include <iomanip>
    using namespace std;  
    const int  WARRIOR_NUM = 5;  
	const int  CITYMAX = 25;
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
			int Position;	//所处城市位置;
			Weapon *wepn[3];
        public:  
			friend class City;
            static string names[WARRIOR_NUM];  
            static int initialLifeValue [WARRIOR_NUM];  
			static int initialAttackValue [WARRIOR_NUM];
            Warrior( Headquarter * p,int no_,int kindNo_ );  
            void PrintResult(int nTime); 
			int AttackEnemy(Warrior *enemy,int time,int city);
			int FightBack(Warrior *enemy,int time,int city);
			int CanUseWeapon(int num);
			void TellWeapon(int time);
			int ShootArrow(int time,City* city,Warrior *enemy);	
			void UseBomb(Warrior *enemy,int time);
			int WillBeKilled(Warrior *enemy,int first);
			virtual int Move();
			virtual int Escape(int time){return 0;};
			~Warrior();

			virtual int Attack(Warrior *enemy,int time,int city) = 0;
			virtual int FightBack0(Warrior *enemy,int time,int city) = 0;
			//virtual int Escape(int time)
    };  

	    class Headquarter  
    {  
        private:  
            int totalLifeValue;  
            bool stopped;  
            int totalWarriorNum;  
            int color;  
			int EnemyNum;
            int curMakingSeqIdx; //当前要制造的武士是制造序列中的第几个  
            int warriorNum[WARRIOR_NUM]; //存放每种武士的数量  
            Warrior * pWarriors[1000];  
        public:  
            friend class Warrior;  
			friend class City;
            static int makingSeq[2][WARRIOR_NUM]; //武士的制作顺序序列  
            void Init(int color_, int lv);  
            ~Headquarter () ;  
			int Produce(int nTime,City *city);  
            string GetColor();  
			void TellLife(int time);
			int Occupy(int time);
			int EnemyComeIn(int time,Warrior *enemy);
			void TellWar(int time);
      
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
				string color;
				color = pHeadquarter -> GetColor();
				step++;
				if(step == 2){
					Life -= 9;
					if(Life <= 0){
						Life = 1;
					}
					Atk += 20;
					step = 0;
				}
				if(color == "red"){
					Position++;
				}
				if(color == "blue"){
					Position--;
				}
				return Position;
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

			int Escape(int time){
				string color =  pHeadquarter -> GetColor(); 
				dead = ((loyalty <= 0) && ((color == "red" && Position != 0) || (color == "blue" && Position != nCity + 1)));
				if(dead){
					printf("%03d:05 %s lion %d ran away\n",time/60,color.c_str(),no);
				}
				return dead;
			}
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

	class City
	{
		public:
			int CityNum;
			int CityLife;
			int Flag;
			int LastWin;
			int DeadByArrow;
			Warrior *RedWarrior, *BlueWarrior;
			City(int num):CityNum(num),CityLife(10),Flag(2),LastWin(2),RedWarrior(0),BlueWarrior(0),DeadByArrow(2){};
			int WarriorExist(int color);
			void WarriorMove(int time);
			void EarnLife(int time);
			//int ShootArrow(Warrior *enemy,int city,int time);
	};

      
	void City::EarnLife(int time){
		if(WarriorExist(0) && !WarriorExist(1)){
			string color =  RedWarrior -> pHeadquarter -> GetColor();
			printf("%03d:40 %s %s %d earned %d elements for his headquarter\n",time/60,color.c_str(),RedWarrior -> names[RedWarrior -> kindNo].c_str(),RedWarrior -> no,CityLife);
			RedWarrior -> pHeadquarter -> totalLifeValue += CityLife;
			CityLife = 0;
		}
		else if(WarriorExist(1) && !WarriorExist(0)){
			string color =  BlueWarrior -> pHeadquarter -> GetColor();
			printf("%03d:40 %s %s %d earned %d elements for his headquarter\n",time/60,color.c_str(),BlueWarrior -> names[BlueWarrior -> kindNo].c_str(),BlueWarrior -> no,CityLife);
			BlueWarrior -> pHeadquarter -> totalLifeValue += CityLife;
			CityLife = 0;
		}
	}


	int City::WarriorExist(int color){
		if(color == 0){
			return !(RedWarrior == 0 || RedWarrior -> dead);
		}
		else
			return !(BlueWarrior == 0 || BlueWarrior -> dead);
	}

	void City::WarriorMove(int time){
		if(WarriorExist(0)){
			printf("%03d:10 red %s marched to city %d with %d elements and force %d\n",time/60,RedWarrior -> names[RedWarrior -> kindNo].c_str(),CityNum,RedWarrior -> Life,RedWarrior -> Atk);
		}
		if(WarriorExist(1)){
			printf("%03d:10 blue %s marched to city %d with %d elements and force %d\n",time/60,BlueWarrior -> names[BlueWarrior -> kindNo].c_str(),CityNum,BlueWarrior -> Life,BlueWarrior -> Atk);
		}
	}

	int Warrior::ShootArrow(int time,City* city,Warrior *enemy){
		string color =  pHeadquarter -> GetColor(); 
		string color1 = enemy -> pHeadquarter -> GetColor();
		enemy -> Life -= ArrowR;
		wepn[2] -> updateWeapon();
		if(enemy -> Life <= 0){
			enemy -> dead = 1;
			if(color1 == "red") {
				city -> DeadByArrow = 0;
			}
			if(color1 == "blue") {
				city -> DeadByArrow = 1;
			}
			printf("%03d:35 %s %s %d shot and killed %s %s %d\n",time/60,color.c_str(),names[kindNo].c_str(),no,color1.c_str(),enemy -> names[enemy -> kindNo].c_str(),enemy -> no);
			return 1;
		}
		else
		{
			printf("%03d:35 %s %s %d shot\n",time/60,color.c_str(),names[kindNo].c_str(),no);
			return 0;
		}
	}

	int Warrior::Move(){
		if(pHeadquarter -> color == 0){
			return Position++;
		}
		else{
			return Position--;
		}
	}

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
		printf("%03d:40 %s %s %d attacked %s %s %d in city %d with %d elements and force %dn",time/60,color.c_str(),names[kindNo].c_str(),no,color1.c_str(),enemy -> names[enemy -> kindNo].c_str(),enemy -> no,city,Life,Atk);	
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
		printf("%03d:40 %s %s %d fought back against %s %s %d in city %dn",time/60,color.c_str(),names[kindNo].c_str(),no,color1.c_str(),enemy -> names[enemy -> kindNo].c_str(),enemy -> no,city);
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
		printf("%03d:38 %s %s %d used a bomb and killed %s %s %dn",time/60,color.c_str(),names[kindNo].c_str(),no,color1.c_str(),enemy -> names[enemy -> kindNo].c_str(),enemy -> no);
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
		EnemyNum = 0;
        curMakingSeqIdx = 0;  
        for( int i = 0;i < WARRIOR_NUM;i++ )  
            warriorNum[i] = 0;  
    }  
      
    Headquarter::~Headquarter () {  
        for( int i = 0;i < totalWarriorNum;i++ )  
            delete pWarriors[i];  
    }  
      
	void Headquarter::TellWar(int time){
		if(color == 0){
			for(int i = totalWarriorNum;i > 0;i--){
				if(!pWarriors[i] -> dead){
					pWarriors[i] -> TellWeapon(time);
				}
			}
		}
		else{
			for(int i = 1;i <= totalWarriorNum;i++){
				if(!pWarriors[i] -> dead){
					pWarriors[i] -> TellWeapon(time);
				}
			}
		}
	}

	void Headquarter::TellLife(int time){
		string col;
		col = GetColor();
		printf("%03d:50 %d elements in %s headquarter\n",time,totalLifeValue,col.c_str());
	}
	
	int Headquarter::Occupy(int time){
		string col;
		col = GetColor();
		if(EnemyNum >= 2){
			printf("%03d:10 %s headquarter was takenn",time/60,col.c_str());
			return 1;
		}
		else return 0;
	}

	int Headquarter::EnemyComeIn(int time,Warrior *enemy){
		string col;
		col = GetColor();
		string color1 = enemy -> pHeadquarter -> GetColor();
		EnemyNum++;
		printf("%03d %s %s %d reached %s headquarter with %d elements and force %d\n",time/60,color1.c_str(),enemy -> names[enemy -> kindNo],enemy -> no,col.c_str(),enemy -> Life,enemy -> Atk);
		return EnemyNum;
	}


	int Headquarter::Produce(int nTime,City *city)  
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
		if(color == 0){
			city -> RedWarrior = pWarriors[totalWarriorNum];
		}
		else{
			city -> BlueWarrior = pWarriors[totalWarriorNum];
		}
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
        int testcase;  
        int m;  
		int HeadOccupy = 0;
        Headquarter RedHead,BlueHead;  
        cin >> testcase; 
        int nCaseNo = 1;  
        while ( testcase-- ) {  
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
			City *pCity[CITYMAX];
			for(int i = 0;i <= nCity + 1;i++){
				pCity[i] = new City(i);
			}
			while(nTime <= TimeEnd && !HeadOccupy) {  
				switch (nTime % 60)
				{
					case 0:
							RedHead.Produce(nTime,pCity[0]);	//报告生产情况的输出似乎要改！
							BlueHead.Produce(nTime,pCity[nCity + 1]);
							break;
					case 5:
							for(int i = 1;i <= nCity;i++){
								if(pCity[i] -> WarriorExist(0)){
									pCity[i] -> RedWarrior -> Escape(nTime);
								}
								if(pCity[i] -> WarriorExist(1)){
									pCity[i] -> BlueWarrior -> Escape(nTime);
								}
							}
							break;
					case 10:
							//to be continue
					case 20:
							for(int i = 1;i <= nCity;i++){
								pCity[i] -> CityLife += 10;
							}
							break;
					case 30:
							for(int i = 1;i <= nCity;i++){
								pCity[i] -> EarnLife(nTime);
							}
							break;
					case 35:
							for(int i = 1;i <= nCity;i++){
								pCity[i] -> DeadByArrow = 2;
							}
							for(int i = 1;i <= nCity;i++){
								if(i != nCity && (pCity[i] -> WarriorExist(0) || pCity[i] -> DeadByArrow == 0) && (pCity[i] -> RedWarrior -> CanUseWeapon(2)) && pCity[i + 1] ->WarriorExist(1)){
									pCity[i] -> RedWarrior ->ShootArrow(nTime,pCity[i + 1],pCity[i + 1] -> BlueWarrior);
								}
								if(i != 1 && (pCity[i] -> WarriorExist(1) || pCity[i] -> DeadByArrow == 1) && (pCity[i] -> BlueWarrior -> CanUseWeapon(2)) && pCity[i + 1] ->WarriorExist(0)){
									pCity[i] -> BlueWarrior ->ShootArrow(nTime,pCity[i + 1],pCity[i + 1] -> RedWarrior);
								}
							}
							break;
					case 38:
							for(int i = 1;i <= nCity;i++){
								if(pCity[i] -> WarriorExist(0) && pCity[i] -> WarriorExist(1)){
									if((pCity[i] -> RedWarrior -> CanUseWeapon(1)) && (pCity[i] -> RedWarrior -> WillBeKilled(pCity[i] -> BlueWarrior,((pCity[i] -> Flag == 0 || (pCity[i] -> Flag == 2 && i%2 == 1))?1:0)))){
										pCity[i] -> RedWarrior -> UseBomb(pCity[i] -> BlueWarrior,nTime);
									}
									if((pCity[i] -> BlueWarrior -> CanUseWeapon(1)) && (pCity[i] -> BlueWarrior -> WillBeKilled(pCity[i] -> RedWarrior,((pCity[i] -> Flag == 1 || (pCity[i] -> Flag == 2 && i%2 == 0))?1:0)))){
										pCity[i] -> BlueWarrior -> UseBomb(pCity[i] -> RedWarrior,nTime);
									}
								}
							}
							break;
					case 40:
							//for(int i = 1;i <= nCity;i++){
							//to be continue;
					case 50:
							RedHead.TellLife(nTime);
							BlueHead.TellLife(nTime);
							break;
					case 55:
							RedHead.TellWar(nTime);
							BlueHead.TellWar(nTime);
							break;
									

				}

                //int tmp1 = RedHead.Produce(nTime);  
                //int tmp2 = BlueHead.Produce(nTime);  
                //if( tmp1 == 0 && tmp2 == 0)  
                //    break;  
                nTime++;  
			}  
			nTime = 0;
			for(int i = 0;i <= nCity + 1;i++){
				delete pCity[i];
			}
		}  
        return 0;  
    }  