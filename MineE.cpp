//Made by DingDang 2024-2
//UTF-8 Coding
//Compile Mode:C++11 or higher version System: Win7+ 64x
//#include<bits/stdc++.h>

#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <iomanip>
#include <cctype>
#include <string>
//#include <unistd.h>

#if _WIN32 || WIN32 //thanks to FZ
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
4. a indicates ai automatic demining, and the last two coordinates indicate Ai demining range
5, h means hacker-mode, it shows the array behind the # symbols
*/
using namespace std;

void print(string s){ //100% created originally
	//getline(cin,s);
	char ch[s.size()];
	for(int i=0;i<s.size();i++) ch[i]=s[i];
	for(int i=0;i<s.size();i++){
		cout<<ch[i];
		Sleep(70);
	} 	
}

bool isNumber(string str) {
    for(char c:str) {
        if(!isdigit(c)) {
            return false;
        }
    }
    return true;
}

int ui[105][105]={0},b[105][105]={0}; 
int lives,mine_sum;
bool firstClick=true;
int row,col;
int k=0,k1=0;

int main(){
	cout<<"Working on ";
	if(platform=="w"){
        cout<<"Windows"<<endl;
    }
    else if(platform=="l"){
        cout<<"Linux"<<endl;
    }
	if(sizeof(void*)==4) {
        cout<<"32-bit Windows"<<endl;
    }else if(sizeof(void*)==8) {
        cout<<"64-bit Windows"<<endl;
    }
	srand(time(NULL)); //random seed
	//system("color 1B");
	//system("title MineSweeper");//set window title
	//system("mode con cols=50 lines=30");//set window size
	//system("echo [console]variables initialization succeeded");
	cout<<'\n';
	Sleep(200);
	cout<<"inputting|format:<HEIGHT> <WIDTH> <HEALTH> <MINE_SUM>\n";//col width row height
	cin>>row>>col>>lives>>mine_sum;
	
	while(row>=30||row<=0||col>=30||col<=0||
		lives>=(row*col)||lives==0||
		mine_sum>=(row*col)||mine_sum<=0||mine_sum==1){ //judge input data
			cout<<"failed to process data, please reset map size/health/mine_sum\n";
			cin>>row>>col>>lives>>mine_sum;
	}
	print("generating map...");cout<<endl;
	/*for(int i=0;i<=100;i+=10){
		cout<<i<<"%... ";
		Sleep(random(row*col*7));
	}*/
	for(int i=1;i<=10;i++){
		cout<<"[";
		for(int j=1;j<=i;j++) cout<<"*";
		for(int g=1;g<=(10-i);g++) cout<<' ';
		cout<<"] "<<i*10<<"%"<<'\n';
		Sleep(random(row*col*3));
	}
	cout<<'\n'<<'\n';
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

	while(true){
		/*for(int i=1;i<=row;i++){
			for(int j=1;j<=col;j++){
				cout<<ui[i][j]<<' '; //output the numbers
			}
			cout<<endl;
		}cout<<endl; */
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
		char op; // input operator
		cin>>op;
		//cout<<"number input\n";
		string x1,y1;
		cin>>x1;cin>>y1;
		int x,y;//input coord
		while(!isNumber(x1) || !isNumber(y1) || 
				(op != 'a' && op != 'q' && op != 'h' && op != 'c' && op != 'p')||
				stoi(x1)<=0||stoi(y1)<=0){
			print("invalid op/number input");cout<<'\n';
			cin>>op>>x1>>y1; 
		}
		x=stoi(x1);y=stoi(y1); //stof -> float | stoi -> int
		
		if(op=='q'){
			if(ui[x][y]==9){//losing
				//system("cls");
				if(firstClick){
					firstClick=false;
					b[x][y]=2;
					k++;k1++;
					continue;
				}else{
					if(b[x][y]==2) continue;//anti-blood on a flag
					lives--;
					//system("color 47");
					print("oops! you just clicked a mine");cout<<endl<<endl;
					Sleep(200);
					//system("color 07");
					if(lives==0){
						print("You Lose!");
						return 0;
					}
					//auto-flag after one death
					b[x][y]=2;
					k++;k1++;
				}
			}
			else{
				firstClick=false;
				//system("cls");
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
			//system("cls"); 
			k1++;
		 	if(ui[x][y]==9){
				k++;
			}	
			b[x][y]=2;
		}
		else if(op=='c'){//unflagging 
			//system("cls");
			if(b[x][y]==2){
				k1--;
				if(ui[x][y]==9){
					k--;
				}
			b[x][y]=0;
			}else{
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
					if(ui[i][j]==9){
						if(b[i][j]!=2){
							k++;k1++;
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
				//system("cls");
				for(int i=1;i<=x;i++){
					for(int j=1;j<=y;j++){
						cout<<ui[i][j]<<' ';
					}cout<<endl;
				}cout<<endl;
				
			}
			Sleep(1500);
			cout<<endl;
		}else if(op=='h'){
			firstClick=false;
			for(int i=1;i<=x;i++){
				for(int j=1;j<=y;j++){
					cout<<ui[i][j]<<' ';
				}cout<<endl;
			}cout<<'\n';	
		}else{
			print("invalid operator");cout<<'\n';continue;
		}
		if(k==tempCalc && k==k1){//winning
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
