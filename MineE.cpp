//Made by DingDang 2024-2 ~ 2024-5
//UTF-8 Coding
//Compiler: C++11 or higher version || System: Win7+ 64x/32x
//#include<bits/stdc++.h>

#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <iomanip>
#include <cctype>
#include <string>
#include <random>
//#include <unistd.h>

#if _WIN32 || WIN32 //thanks to Hi-Kite
    #define platform "w"
#elif __linux__
    #define platform "l"
#endif

#define random(x) 1+rand()%(x)
/*
Operator description:
1, q means to open the grid at the coordinate, continue to enter x and y, means to open the y row x
2, p indicates that the flag is planted at the coordinates (the coordinates of the user are thunder)
3, c means cancel the flag at the coordinate (if any)
4. a indicates Ai automatic demining, and the last two coordinates indicate Ai demining range
*/
using namespace std;

int ui[105][105]={0},uiStatus[105][105]={0}; 
int lives,mine_sum;
bool firstClick=true;
int row,column; //map size
int minesum_correct=0,minesum_user=0;

void print(string s){ //100% created originally
	//getline(cin,s);
	char ch[s.size()];
	for(int i=0;i<s.size();i++) ch[i]=s[i];
	for(int i=0;i<s.size();i++){
		cout<<ch[i];
		Sleep(70);
	} 	
}

bool isNumber(string str){
    for(char c:str) {
        if(!isdigit(c)) { //current symbol is not a number
            return false;
        }
    }
    return true;
}

void generateMap(int rowf,int colf,int mine,int clickX,int clickY,int mineArea[105][105]){
    //srand(time(NULL));
	random_device rd; //thanks to BlackHIG
	mt19937 gen(rd());
	while(mine){
		uniform_int_distribution<> x1(1, rowf);
		uniform_int_distribution<> y1(1, colf);
        int x=x1(gen); //generate x coordinate
		int y=y1(gen); //y coord
        if(x==clickX && y==clickY) continue; //skip if the mine position is the click position
        if(mineArea[x][y]==0) { //place mine if the block is empty
            mineArea[x][y]=9; //number 9 represents mine
            mine--;
        }
    }cout<<"mine|";

    //calculate the number of mines around each block
    for(int i=1;i<=rowf;i++){
        for(int j=1;j<=colf;j++){
            int sum=0; //initialize the sum
            if(mineArea[i][j]!=9){ //if the block is not a mine
                for(int dx=-1;dx<=1;dx++){
                    for(int dy=-1;dy<=1;dy++){
                        if(mineArea[i+dx][j+dy]==9) sum++; //check neighboring blocks for mine
                    }
                }
				mineArea[i][j]=sum; //update the block with the number of mines around it
            }
        }
    }cout<<"array\n";
}

void processAni(int row,int column){
	/*for(int i=0;i<=100;i+=10){
		cout<<i<<"%... ";
		Sleep(random(row*col*7));
	}*/
	for(int i=1;i<=10;i++){
		cout<<"[";
		for(int j=1;j<=i;j++) cout<<"*";
		for(int g=1;g<=(10-i);g++) cout<<' ';
		cout<<"] "<<i*10<<"%"<<'\n';
		Sleep(random(row*column*3)); //random interval
	}
}

void coutArray(int array[105][105],int limitX,int limitY){
	for(int i=1;i<=limitX;i++){
		for(int j=1;j<=limitY;j++){
			cout<<array[i][j]<<' '; //output
		}cout<<endl;
	}cout<<endl;
}

void coutSymbols(int array[105][105],int arrayStatus[105][105],int limitX,int limitY){
	for(int i=1;i<=limitX;i++){
		for(int j=1;j<=limitY;j++){ //statement of b[k][b]: =0 closed | =1 opened | =2 flagged
			if(arrayStatus[i][j]==0){
				cout<<"#"<<' '; //closed
			}else if(arrayStatus[i][j]==1){
				cout<<array[i][j]<<' '; //opened
			}else if(arrayStatus[i][j]==2){
				cout<<"P"<<' '; //flagged
			}
		}cout<<endl;
		//judge the status of the grid (opened, closed, flagged) 
	}
}

int main(){
	cout<<"Working on ";
	if(platform=="w"){
        cout<<"Windows ";
    }
    else if(platform=="l"){
        cout<<"Linux"<<endl;
    }
	if(sizeof(void*)==4){ //doesnt work on Linux
        cout<<"32-bit"<<endl;
    }else if(sizeof(void*)==8) {
        cout<<"64-bit"<<endl;
    }
    
	srand(time(NULL)); //random seed
	//system("color 1B");
	//system("title MineSweeper");//set window title
	//system("mode con cols=50 lines=30");//set window size
	//system("echo [console]variables initialization succeeded");
	cout<<'\n';
	Sleep(200);
	cout<<"inputting|format:<HEIGHT> <WIDTH> <HEALTH> <MINE_SUM>\n";//col width row height
	print("example: 9 9 2 10");cout<<"\n";
	cin>>row>>column>>lives>>mine_sum;
	
	while(row>=30||row<=0||column>=30||column<=0||
		lives>=(row*column)||lives==0||
		mine_sum>=(row*column)||mine_sum<=0||mine_sum==1){ //judge input data
			cout<<"failed to process data, please reset map size/health/mine_sum\n";
			cin>>row>>column>>lives>>mine_sum;
	}
	print("preloading map...");cout<<endl;
	
	cout<<'\n';
	while(true){
		/*if(!firstClick){ //because the map generation is after first click
			coutArray(ui,row,column);
		}*/
	//for debugging

		coutSymbols(ui,uiStatus,row,column);
		cout<<"Lives:"<<lives<<' '<<"Mines:"<<mine_sum<<'\n';
		char op; //input operator
		cin>>op;
		//cout<<"number input\n";
		string x1,y1;
		cin>>x1>>y1;
		int x,y;//input coord
		while(!isNumber(x1) || !isNumber(y1) || 
				(op!='a' && op!='q' && op!='h' && op!='c' && op!='p')||
				stoi(x1)<=0||stoi(y1)<=0||(firstClick&&op!='q')||
				stoi(x1)>row||stoi(y1)>column){ //data judgment
			print("invalid op/number input");cout<<'\n';
			cin>>op>>x1>>y1; 
		}
		x=stoi(x1);y=stoi(y1); //stof -> float | stoi -> int

		if(firstClick){ //map generation
			generateMap(row,column,mine_sum,x,y,ui);
			processAni(row,column); //play loading animation
		}
		if(op=='q'){
			if(ui[x][y]==9){//losing
				if(uiStatus[x][y]==2) continue;//anti-blood on a flag
				lives--;
				//system("color 47");
				print("oops! you just clicked a mine");cout<<endl<<endl;
				Sleep(200);
				//system("color 07");
				if(lives==0){
					print("You Lose!");
					string lose;cin>>lose;
					break;
				}
				//auto-flag after one death
				uiStatus[x][y]=2;
				minesum_correct++;minesum_user++;
			}
			else{
				firstClick=false;
				//system("cls");
				uiStatus[x][y]=1; //current block is opened
				for(int dx=-1;dx<=1;dx++){
					for(int dy=-1;dy<=1;dy++){
						if(ui[x+dx][y+dy]==0 && uiStatus[x+dx][y+dy]!=2) uiStatus[x+dx][y+dy]=1;
						/*[x-1][y-1] [x-1][y] [x-1][y+1]
						  [x][y-1]   [x][y]   [x][y+1]
						  [x+1][y-1] [x+1][y] [x+1][y+1]*/
					}
				}
				//determine if there are any blank squares around, and if there are, open them automatically
			}
		}
		else if(op=='p'){//flagging
			firstClick=false; 
			//system("cls"); 
			minesum_user++; //user thinks this block is mine
		 	if(ui[x][y]==9){
				minesum_correct++; //current block is mine
			}	
			uiStatus[x][y]=2; //flagged
		}
		else if(op=='c'){//unflagging 
			//system("cls");
			if(uiStatus[x][y]==2){ //current block is flagged
				minesum_user--; //same as the content above
				if(ui[x][y]==9){
					minesum_correct--;
				}
			uiStatus[x][y]=0;//unflagged
			}else{ //current block is not flagged
				//system("cls");
				continue;
			}
		}
		else if(op=='a'){
			firstClick=false;
			print("ai-mode enabled");cout<<'\n';
			cout<<"looking for mines in: "<<'('<<x<<','<<y<<')'<<endl; 
			int new_mine=0;
			for(int i=1;i<=x;i++){
				for(int j=1;j<=y;j++){
					if(ui[i][j]==9){ //current block is mine
						if(uiStatus[i][j]!=2){ //current block is not flagged
							minesum_correct++;minesum_user++;
							uiStatus[i][j]=2; //auto flag
							new_mine++; //sum the number of new mines
						}
						cout<<"mine position: "<<i<<' '<<j<<endl; //output mine position
						Sleep(200);
					}
				}
			}
			cout<<new_mine<<" NEW mine(s) was(were) found\n";
			cout<<"output array?(y/n)"<<' ';
			char ans;
			cin>>ans;
			if(ans=='y'){
				cout<<"outputting..."<<'\n';
				Sleep(1000);
				//system("cls");
				coutArray(ui,x,y);
			}
			Sleep(1500);
			cout<<endl;
		}else{
			print("invalid operator");cout<<'\n';continue;
		}

		if(minesum_correct==mine_sum && minesum_correct==minesum_user){ //winning condition
			print("You Win!");
			/*for(int i=1;i<=2;i++){
				system("color 1a");
				Sleep(200);
				system("color 2b");
				Sleep(200);
				system("color 3c");
				Sleep(200);
				system("color 4d");
				Sleep(200);
				system("color 5e");
			}
			system("color 07");*/
			string end;cin>>end;
			break;
		}
	}
    return 0;
}