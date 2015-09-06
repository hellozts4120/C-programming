    #include <iostream>  
    #include <cstdio>  
    #include <string>  
	#include <iomanip>
    using namespace std;  
    const int  WARRIOR_NUM = 5;  
	const string weapon[3] = {"sword","bomb","arrow"};
    /* 
    string Warrior::names[WARRIOR_NUM] = { "dragon","ninja","iceman","lion","wolf" }; 
    红方司令部按照 iceman、lion、wolf、ninja、dragon 的顺序制造武士。 
    蓝方司令部按照 lion、dragon、ninja、iceman、wolf 的顺序制造武士。 
    */  
      
    class Headquarter;  
    class Warrior  
    {  
        private:  
            Headquarter * pHeadquarter;  
            int kindNo; //武士的种类编号 0 dragon 1 ninja 2 iceman 3 lion 4 wolf  
            int no;  
        public:  
            static string names[WARRIOR_NUM];  
            static int initialLifeValue [WARRIOR_NUM];  
            Warrior( Headquarter * p,int no_,int kindNo_ );  
            void PrintResult(int nTime);  
    };  

	class dragon:public Warrior
	{
		public:
			int wepn;
			// int HP;
			float morale;
			dragon(int n,int ){
				wepn = n % 3;
				morale = totalLifeValue / value;
				// HP = 1;
				show();
			}
			void show(){
				cout << "It has a " + weapon[wepn]+",and it's morale is " << fixed << setprecision(2) << morale << endl;
			}
	};

	class ninja:public Warrior
	{
		public:
			int wepn[2];
			ninja(int n){
				wepn[0] = n % 3;
				wepn[1] = (n + 1) % 3;
				show();
			}
			void show(){
				cout << "It has a " + weapon[wepn[0]]+" and a " << weapon[wepn[1]] << endl;
			}
	};

	class iceman:public Warrior
	{
		public:
			int wepn;
			iceman(int n){
				wepn = n % 3;
				show();
			}
			void show(){
				cout << "It has a " +weapon[wepn]+" and a " << endl;
			}
	};

	class lion:public Warrior
	{
		public:
			int loyalty;
			lion(int loyal){
				loyalty = loyal;
				show();
			}
			void show(){
				cout << "It's loyalty is " << loyalty << endl;
			}
	};

	class wolf:public Warrior
	{
		public:
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
        pWarriors[totalWarriorNum] = new Warrior( this,totalWarriorNum+1,kindNo);  
        warriorNum[kindNo]++;  
        pWarriors[totalWarriorNum]->PrintResult(nTime);  
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
            cin >> m; 
            for( int i = 0;i < WARRIOR_NUM;i++ )  
                cin >> Warrior::initialLifeValue[i];
            RedHead.Init(0,m);  
            BlueHead.Init(1,m);  
            int nTime = 0;  
            while(true) {  
                int tmp1 = RedHead.Produce(nTime);  
                int tmp2 = BlueHead.Produce(nTime);  
                if( tmp1 == 0 && tmp2 == 0)  
                    break;  
                nTime++;  
            }  
        }  
        return 0;  
    }  