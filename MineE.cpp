//Made by DingDang 2024-2
//AntiCheat: Grim v7~
//UTF-8 Coding
//Compile Mode:C++11 or higher version System: Win7+ 64x
#include<bits/stdc++.h>
#include "windows.h" //for Windows-Client
/*
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <iomanip>
*/
#define random(x) 1+rand()%(x)
/*
Operator description:
1, q means to open the grid at the coordinate, continue to enter x and y, means to open the y row x
2, p indicates that the flag is planted at the coordinates (the coordinates of the user are thunder)
3, c means cancel the flag at the coordinate (if any)
4. a indicates ai automatic demining, and the last two coordinates indicate Ai demining range
*/
using namespace std;

int ui[105][105]={0},b[105][105]={0}; 
int lives,mine_sum;
bool firstClick=true;
int row,col;
int k=0,k1=0;
	
int main(){
	srand(time(NULL)); //random seed
	//system("color 1B");
	system("title MineSweeper");//set window title
	system("mode con cols=50 lines=30");//set window size
	system("echo [console]variate initialization succeeded");
	cout<<"inputting|format:<ROW> <COL> <HEALTH> <MINE_SUM>]\n";
	cout<<"example: 10 10 3 10\n";
	
	cin>>row>>col>>lives>>mine_sum;
	
	while(row>=30||row<=0||col>=30||col<=0||
		lives>=(row*col)||lives==0||
		mine_sum>=(row*col)||mine_sum<=0){ //judge input data
			cout<<"failed to process data, please reset map size/health/mine_sum\n";
			cin>>row>>col>>lives>>mine_sum;
	}
	int tempCalc=mine_sum;
	while(mine_sum){
		int x=random(row);//a+rand()%b = [a, a+b-1]
		int y=random(col);
		if(!ui[x][y]){//if(ui[x][y]==0){
			ui[x][y]=9;
			mine_sum--;
		}
	}
	for(int i=1;i<=row;i++){
		for(int j=1;j<=col;j++){
			int sum=0;//sum is the current block number
			if(ui[i][j]!=9){
				if(ui[i-1][j-1]==9) sum++;
				if(ui[i-1][j]==9) sum++;//generate number 
				if(ui[i-1][j+1]==9) sum++;
				if(ui[i][j-1]==9) sum++;
				if(ui[i][j+1]==9) sum++;
				if(ui[i+1][j-1]==9) sum++;
				if(ui[i+1][j]==9) sum++;
				if(ui[i+1][j+1]==9) sum++;
				//sum=!(ui[i-1][j-1]-9)+!(ui[i-1][j]-9)+!(ui[i-1][j+1]-9)+!(ui[i][j-1]-9)+!(ui[i][j+1]-9)+!(ui[i+1][j-1]-9)+!(ui[i+1][j]-9)+!(ui[i+1][j+1]-9);
				ui[i][j]=sum;
				
			}
		}
	}
	
	/*for(int i=1;i<=10;i++){
		for(int j=1;j<=10;j++){
			cout<<ui[i][j]<<' '; //output the numbers
		}
		cout<<endl;
	}*/
	
	while(true){
		for(int i=1;i<=row;i++){
			for(int j=1;j<=col;j++){
				cout<<ui[i][j]<<' '; //output the numbers
			}
			cout<<endl;
		}cout<<endl; 
	//for debug
	
		for(int i=1;i<=row;i++){
			for(int j=1;j<=col;j++){ //state of b[k][m]: =0 closed | =1 opened | =2 flagged
				if(b[i][j]==0){
					cout<<"#"<<' ';
				}else if(b[i][j]==1){
					cout<<ui[i][j]<<' ';
				}else if(b[i][j]==2){
					cout<<"P"<<' ';
				}
			}cout<<endl;
			//Judge the status of the grid (opened, closed, flag 
		}
		cout<<"Lives: "<<lives<<'\n';
		char op;//input operator
		cin>>op;
		int x,y;//input coord
		cin>>x>>y;
		while(x>row||y>col){
			cout<<"invalid input number"<<endl;
			cin>>op>>x>>y;continue;
		}
		
		if(op=='q'){
			if(ui[x][y]==9){//losing
				system("cls");
				if(firstClick){
					firstClick=false;
					b[x][y]=2;
					continue;
				}
				if(b[x][y]==2) continue;//anti-blood on a flag
				lives--;
				system("color 47");
				cout<<"oops! you just clicked a mine"<<endl<<endl;
				Sleep(200);
				system("color 07");
				if(lives==0){
					cout<<"You Lose!";
					return 0;
				}
				b[x][y]=2;//auto-flag after one death
				continue;
				scanf("%d");
				return 0;
			}
			else{
				firstClick=false;
				system("cls");
				b[x][y]=1;
				if(b[x][y]==1&&ui[x][y]==0){
					if(ui[x-1][y-1]==0) b[x-1][y-1]=1;
					if(ui[x-1][y]==0) b[x-1][y]=1;
					if(ui[x-1][y+1]==0) b[x-1][y+1]=1;
					if(ui[x][y-1]==0) b[x][y-1]=1;
					if(ui[x][y]==0) b[x][y]=1;
					if(ui[x][y+1]==0) b[x][y+1]=1;
					if(ui[x+1][y-1]==0) b[x+1][y-1]=1;
					if(ui[x+1][y]==0) b[x+1][y]=1;
					if(ui[x+1][y+1]==0) b[x+1][y+1]=1;	
				}
				//Determine if there are any blank squares around, and if there are, open them automatically
			}
		}
		else if(op=='p'){//flagging
			firstClick=false; //k stores the correct location of the mine (objective),
			  			      //k1 is the user's marking location (subjective); Function: Store the total number of mines
			system("cls"); 
			k1++;
		 	if(ui[x][y]==9){
				k++;
			}	
			b[x][y]=2;
		}
		else if(op=='c'){//unflagging 
			system("cls");
			if(b[x][y]==2){
				k1--;
				if(ui[x][y]==9){
					k--;
				}
			b[x][y]=0;
			}else{
				system("cls");
				continue;
			}
		}
		else if(op=='a'){
			firstClick=false;
			cout<<"ai-mode enabled"<<endl;
			int x1,y1;
			cout<<"start-point coordinates have been entered\n please enter the end-point coordinates\n";
			cin>>x1>>y1; //input judging
			while(x1<=0||y1<=0||x1>row||y1>col){
				cout<<"invalid input\n";
				cin>>x1>>y1;
			}
			cout<<"looking for mines in: "<<'('<<x<<','<<y<<')'<<'('<<x1<<','<<y1<<')'<<endl; 
			int new_mine=0;
			for(int i=x;i<=x1;i++){
				for(int j=y;j<=y1;j++){
					if(ui[i][j]==9){
						k++;
						k1++;
						if(b[i][j]!=2){
							b[i][j]=2;
							new_mine++;
						}
						cout<<"mine position: "<<i<<' '<<j<<endl;
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
				system("cls");
				for(int i=x;i<=x1;i++){
					for(int j=y;j<=y1;j++){
						cout<<ui[i][j]<<' ';
					}cout<<endl;
				}cout<<endl;
				
			}
			Sleep(1500);
			cout<<endl;
		}else{
			cout<<"invalid operator\n";continue;
		}
		if(k==tempCalc && k==k1){//winning
			cout<<"You Win !";
			for(int i=1;i<=5;i++){
				system("color 1a");
				Sleep(100);
				system("color 2b");
				Sleep(100);
				system("color 3c");
				Sleep(100);
				system("color 4d");
				Sleep(100);
				system("color 5e");
			}
			system("color 07");
			scanf("%d");
		}
	}
    return 0;
} 
