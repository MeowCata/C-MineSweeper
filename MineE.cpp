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
//#include <unistd.h>

#if _WIN32 || WIN32 //thanks to Hi-Kite
#define platform "w"
#elif __linux__
#define platform "l"
#endif

#define random(x) 1 + rand() % (x)
/*
Operator description:
1, q means to open the grid at the coordinate, continue to enter x and y, means to open the y row x
2, p indicates that the flag is planted at the coordinates (the coordinates of the user are thunder)
3, c means cancel the flag at the coordinate (if any)
4. a indicates Ai automatic demining, and the last two coordinates indicate Ai demining range
*/
using namespace std;

int ui[105][105] = {0}, uiStatus[105][105] = {0};
int lives, mine_sum;
bool firstClick = true;
int row, column; //map size
int minesum_correct = 0, minesum_user = 0;
int flag_sum;
bool autoOpenEnabled = false;

void print(string s, bool coutENDL) { //100% created originally
	//getline(cin,s);
	char ch[s.size()];
	for (int i = 0; i < s.size(); i++) ch[i] = s[i];
	for (int i = 0; i < s.size(); i++) {
		cout << ch[i];
		Sleep(70);
	}
	if (coutENDL) cout<<endl;
}

bool isNumber(string str) {
	for (auto c: str) {
		if (!isdigit(c)) { //current symbol is not a number
			return false;
		}
	}
	return true;
}

bool checkOutOfBorders(int checkX, int checkY) {
	if (checkX <= 0 || checkX > row) {
		return true;
	}
	if (checkY <= 0 || checkY > column) {
		return true;
	}
	return false;
}
const int facingsX[9] = {-1, -1, -1, 0, 0, 1, 1, 1}; //thanks to BlackHIG's logic
const int facingsY[9] = {-1, 0, 1, -1, 1, -1, 0, 1};
/*[x-1][y-1] [x-1][y] [x-1][y+1]
  [x][y-1]   [x][y]   [x][y+1]
  [x+1][y-1] [x+1][y] [x+1][y+1]*/

int calcNearbyMines(int checkX, int checkY, int array[105][105], int mineValue) {
	int sum = 0;

	for (int i = 0; i < 8; i++) {
		int targetX = checkX + facingsX[i];
		int targetY = checkY + facingsY[i];

		if (checkOutOfBorders(targetX, targetY)) continue;

		if (array[targetX][targetY] == mineValue) {
			sum ++;
		}
	}

	return sum;
}

void generateMap(int rowf, int colf, int mine, int clickX, int clickY, int mineArea[105][105]) {
	srand(time(NULL));
	while (mine) {
		int x = random(rowf); //generate x coordinate
		int y = random(colf); //y coord
		if (x == clickX && y == clickY) continue; //skip if the mine position is the click position
		if (mineArea[x][y] == 0) { //place mine if the block is empty
			mineArea[x][y] = 9; //number 9 represents mine
			mine--;
		}
	}
	cout << "mine|";

	//calculate the number of mines around each block
	for (int i = 1; i <= rowf; i++) {
		for (int j = 1; j <= colf; j++) {
			if (mineArea[i][j] != 9) { //if the block is not a mine
				mineArea[i][j] = calcNearbyMines(i, j, mineArea, 9); //update the block with the number of mines around it
			}
		}
	}
	cout << "array\n";
}

void processAni(int row, int column) {
	/*for(int i=0;i<=100;i+=10){
	    cout<<i<<"%... ";
	    Sleep(random(row*col*7));
	}*/
	for (int i = 1; i <= 10; i++) {
		cout << "[";
		for (int j = 1; j <= i; j++) cout << "*";
		for (int g = 1; g <= (10 - i); g++) cout << ' ';
		cout << "] " << i * 10 << "%" << '\n';
		Sleep(random(row * column * 3)); //random interval
	}
}

void coutArray(int array[105][105], int limitX, int limitY) {
	cout << endl << "   ";
	for (int k = 1; k <= limitY; k++) cout << k << ' ';
	cout << "y";
	cout << endl;
	for (int i = 1; i <= limitX; i++) {
		cout << endl << i << "  ";
		for (int j = 1; j <= limitY; j++) {
			cout << array[i][j] << ' '; //output
		}
	} //cout << endl;
	cout << endl << "x\n";
}

void coutSymbols(int array[105][105], int arrayStatus[105][105], int limitX, int limitY) {
	cout << endl << "   ";
	for (int k = 1; k <= limitY; k++) cout << k << ' ';
	cout << "y";
	cout << endl;

	for (int i = 1; i <= limitX; i++) {
		cout << endl << i << "  ";
		for (int j = 1; j <= limitY; j++) { //statement of b[k][b]: =0 closed | =1 opened | =2 flagged
			if (arrayStatus[i][j] == 0) {
				cout << "#" << ' '; //closed
			} else if (arrayStatus[i][j] == 1) {
				if (ui[i][j] == 0) cout << ' ' << ' '; //opened
				else cout << array[i][j] << ' '; //opened
			} else if (arrayStatus[i][j] == 2) {
				cout << "P" << ' '; //flagged
			}
		}
		//judge the status of the grid (opened, closed, flagged)
	}
	cout << endl << "x\n";
}

void openEmptyAround(int x, int y) {
	if (checkOutOfBorders(x, y)) return;

	uiStatus[x][y] = 1;

	if (ui[x][y] == 0) {
		for (int i = 0; i < 8; i++) {
			int newX = x + facingsX[i];
			int newY = y + facingsY[i];
			if (!checkOutOfBorders(newX, newY) && uiStatus[newX][newY] == 0) openEmptyAround(newX, newY);
		}
	}
}

bool msWin() {
	int cnt = 0;
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= column; j++) {
			if (uiStatus[i][j] == 0) cnt++;
		}
	}
	return flag_sum == cnt;
}

void autoFlag(int ui[105][105], int uiStatus[105][105]) {
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= column; j++) { //traverse every block
			if (ui[i][j] == 0 || ui[i][j] == 9 || uiStatus[i][j] != 1) continue; //conditions: not blank, not a mine, not opened
			int currentNum = ui[i][j];
			int blankCnt = 0; //stores the number of blank blocks
			int flagTX[8] = {-1}, flagTY[8] = {-1}; //stores flag position

			for (int k = 0; k < 8; k++) { //traverse blocks around
				int targetX = i + facingsX[k];
				int targetY = j + facingsY[k];
				if (checkOutOfBorders(targetX, targetY)) continue;

				if (uiStatus[targetX][targetY] == 2) { //flagged
					currentNum--;
				} else if (uiStatus[targetX][targetY] == 0) { //closed
					flagTX[blankCnt] = targetX; //record number
					flagTY[blankCnt] = targetY;
					blankCnt++; //blankCnt is index!
				}
			}

			if (blankCnt > 0 && currentNum == blankCnt) {
				for (int k = 0; k < blankCnt; k++) {
					if (flagTX[k] != -1 && flagTY[k] != -1) { // && uiStatus[flagTX[k]][flagTY[k]] == 0 (not necessary)
						uiStatus[flagTX[k]][flagTY[k]] = 2; //flag
						flag_sum--;
						minesum_correct++; minesum_user++;
					}
				}
			}
		}
	}
}




int main() {
	cout << "Working on ";
	if (platform == "w") {
		cout << "Windows ";
	} else if (platform == "l") {
		cout << "Linux" << endl;
	}
	if (sizeof(void*) == 4) { //doesnt work on Linux
		cout << "32-bit" << endl;
	} else if (sizeof(void*) == 8) {
		cout << "64-bit" << endl;
	}

	srand(time(NULL)); //random seed
	//system("color 1B");
	//system("title MineSweeper");//set window title
	//system("mode con cols=50 lines=30");//set window size
	//system("echo [console]variables initialization succeeded");
	cout << '\n';
	Sleep(200);
	cout << "inputting|format:<HEIGHT> <WIDTH> <HEALTH> <MINE_SUM>\n";//col width row height
	print("example: 9 9 2 10", true);
	cin >> row >> column >> lives >> mine_sum;

	while (row >= 30 || row <= 0 || column >= 30 || column <= 0 ||
	        lives >= (row * column) || lives == 0 ||
	        mine_sum >= (row * column) || mine_sum <= 0 || mine_sum == 1 ||
	        row * column - 1 == mine_sum) { //judge input data
		cout << "failed to process data, please reset map size/health/mine_sum\n";
		cin >> row >> column >> lives >> mine_sum;
	}
	flag_sum = mine_sum;
	cout << "enable autoFlag?(y/n)";
	char ans;
	cin >> ans;
	if (ans == 'y') autoOpenEnabled = true;
	print("preloading map...", true);

	cout << '\n';
	while (true) {
		/*if(!firstClick) { //because the map generation is after first click
			coutArray(ui,row,column);
		}*/
		//for debugging
		coutSymbols(ui, uiStatus, row, column);

		cout << "HP: " << lives << ' ' << "Mines: " << mine_sum << '\n' << "Flags-Remaining: " << flag_sum << '\n';
		string ops; //input operator
		cin >> ops;
		char op = ops[0];
		//cout<<"number input\n";
		string x1, y1;
		cin >> x1 >> y1;
		int x, y;//input coord

		while (!isNumber(x1) || !isNumber(y1) ||
		        (op != 'q' && op != 'c' && op != 'p') ||
		        stoi(x1) <= 0 || stoi(y1) <= 0 || (firstClick && op != 'q') ||
		        stoi(x1) > row || stoi(y1) > column) { //data judgment
			print("invalid op/number input", true);
			cin >> op;
			cin >> x1 >> y1;
		}
		x = stoi(x1);
		y = stoi(y1); //stof -> float | stoi -> int

		if (firstClick) { //map generation
			generateMap(row, column, mine_sum, x, y, ui);
			processAni(row, column); //play loading animation
		}

		if (op == 'q') {
			firstClick = false;
			if (ui[x][y] == 9) {//losing
				if (uiStatus[x][y] == 2) continue;//anti-blood on a flag
				lives--;
				//system("color 47");
				print("oops! you just clicked a mine", true);
				cout << endl;
				Sleep(200);
				//system("color 07");
				if (lives == 0) {
					print("You Lose!", false);
					string lose;
					cin >> lose;
					break;
				}
				//auto-flag after one death
				uiStatus[x][y] = 2;
				flag_sum--;
				minesum_correct++;
				minesum_user++;
			} else if (uiStatus[x][y] == 2) {
				continue;
			} else {
				//system("cls");
				uiStatus[x][y] = 1; //current block is opened
				if (ui[x][y] == 0) openEmptyAround(x, y);
				//determine if there are any blank squares around, and if there are, open them automatically
				if (autoOpenEnabled) autoFlag(ui, uiStatus);
			}
		} else if (op == 'p') { //flagging
			firstClick = false;
			//system("cls");
			if (uiStatus[x][y] == 2) continue;
			flag_sum--;
			if (flag_sum >= 0) {
				minesum_user++; //user thinks this block is mine
				if (ui[x][y] == 9) {
					minesum_correct++; //current block is mine
				}
				uiStatus[x][y] = 2; //flagged
			}
			if (flag_sum < 0) flag_sum = 0;
		} else if (op == 'c') { //unflagging
			//system("cls");
			if (uiStatus[x][y] == 2) { //current block is flagged
				minesum_user--; //same as the content above
				if (ui[x][y] == 9) {
					minesum_correct--;
				}
				uiStatus[x][y] = 0;//unflagged
				flag_sum++;
			} else { //current block is not flagged
				//system("cls");
				continue;
			}
		} else {
			print("invalid operator", true);
			continue;
		}

		if ((minesum_correct == mine_sum && minesum_correct == minesum_user) || msWin()) { //winning condition
			coutArray(ui, row, column);
			print("You Win!", false);
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
			string end;
			cin >> end;
			break;
		}
	}
	return 0;
}
