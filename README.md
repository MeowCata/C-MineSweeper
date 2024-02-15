# C-MineE
A minesweeper game based on C++

**My CSDN Blog:[Click Here](https://blog.csdn.net/weixin_45122104/article/details/128066651)**

Hello，大家好，我是余同学。这两个月真是太忙了，无暇给大家更新文章...

**暑假不是写了个扫雷小游戏吗([Link](https://blog.csdn.net/weixin_45122104/article/details/125664857))？考虑到很多同学对代码没有透彻的理解，那么，这篇文章，我们来详细分析一下代码.**

我们分为三个部分来讲：**`生成雷区`,`生成雷区数字`和`刷新与判断`**
***
### Part.1 生成雷区
#### 随机数
首先，我们的雷区不能是定义好的矩阵，肯定得用随机数生成。
用随机数的话，就出现了一个问题：

* **C++有一个生成随机数的奇妙特性(也就是在不写`srand(time(NULL));`的情况下)**

这个特性是什么样的呢？
我们来看下示例：
```cpp
#include <bits/stdc++.h>
using namespace std;

int main ()
{
	//srand(time(NULL));
	int random=rand()%10;
	cout<<random;
    return 0;
}
```
运行结果：
**第一次：**
![1.1](https://img-blog.csdnimg.cn/d9abc09f194046f4ad902c468b6f19b2.png)

**第二次：**
![1.2](https://img-blog.csdnimg.cn/ff2b6a5aaa1d49b884dce2fb2b0d15f2.png)

**可以看到， 虽然是"`随机数`"，但是，程序每次输出的数却是一样的，这是怎么回事？**

**程序每次生成的数来源于一个随机数种子，如果我们不改变它的话，那么程序就会一直使用这个种子，从而导致每次生成的随机数都一样**

因此，我们要使用随机数种子
**其具体原理即是利用`每次运行的时间互不相同，生成的随机数也不同`**
**`srand(time(NULL));`**

**可以使用`rand()%x`生成随机数，也可以使用宏定义，即使用`define`**
```cpp
#define random(x) rand()%(x)
```
那么，我们看看效果吧
**第一次输出：**
![2.1](https://img-blog.csdnimg.cn/901db166ec3144e1b71ca29ccc4113af.png)
**第二次输出：**
![2.2](https://img-blog.csdnimg.cn/dc0ca8b69ee34a809b425a0cf591d5e8.png)
**也就是说，如果我们要生成一个从`a`到`b`区间的随机数，可以用以下指令：**
```cpp
int number=a+rand()%b; //随机数生成区间为：a ~ a+b-1
int number=rand()%b;   //随机数生成区间为：1 ~ b-1
```

**随机数搞定了，接下来，就是考虑生成扫雷矩阵的事情了**

#### 生成雷区

现在，我们面临的最大问题就是：**在不使用字符串的情况下，如何用0~9这十个数字实现数字与雷的分别**

其实，实现很简单，我使用的方法是：**用`1~8`作为数字，`0`作为附近9格无雷标志(其实还是数字)，`9`作为雷**

那么，我们的矩阵是 **`10×10`** 的，只要通过循环生成 **`100`** 个随机数就行了

**看代码吧：**
```cpp
#include<bits/stdc++.h>
/*
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <iomanip>
*/
#define random(x) 1+rand()%(x)
using namespace std;
int ui[12][12],b[12][12]; //多开一些没有坏处，原因后面会讲

int main(){
	srand(time(NULL)); //random seed, srand(time(0));
	//system("color 1B");
	system("title MineSweeper");
	int cnt=10; //地雷个数
	
	while(cnt){
		int x=random(10);//a+rand()%b = [a, a+b-1]
		int y=random(10);
		if(!ui[x][y]){//if(ui[x][y]==0){
			ui[x][y]=9;
			cnt--;
		}
	}
	return 0;
}
```
**忘记说了，我设了两个二维数组，其中，`ui`数组的功能是：`存储整个雷区；`数组`b`的功能则是：`存储每个方格的状态`（详见`Part.3`）**

至此，我们的第一部分结束。

**如果有的同学疑惑上面的代码没有输出时，那是因为我们只写了生成雷区的代码，并没有写生成雷区旁数字的代码，详细请看`Part.2 生成雷区数字`**
***
### Part. 2 生成雷区数字
要完成这一部分，我们首先得了解地雷旁数字的生成规律：

**每个数字代表周围`8`格内的地雷数量**

还是上图片：
![3.1](https://img-blog.csdnimg.cn/00792f3d1e174aae9d1009025b18c47f.png#pic_center)
**这样，我们的逻辑就清晰了，只要用双重循环遍历二维数组`b`的每一项，判断这个点是否为地雷，如果是，就跳过这个点，继续向下遍历；否之，计算出该点周围`8`个方格的总雷数（我们暂且定义为`sum`），`sum`就是这个点的数值**

来把这一部分的代码实现吧
```cpp
for(int i=1;i<=10;i++){
	for(int j=1;j<=10;j++){
		int sum=0; //统计周围地雷数量
		if(ui[i][j]!=9){ //该点不为雷
			/*
			下面的这些坐标具体位置详见上图
			*/
			if(ui[i-1][j-1]==9) sum++;//sum=!(a[i-1][j-1]-9)+!(a[i-1][j]-9)...
			if(ui[i-1][j]==9) sum++;
			if(ui[i-1][j+1]==9) sum++;
			if(ui[i][j-1]==9) sum++;
			if(ui[i][j+1]==9) sum++;
			if(ui[i+1][j-1]==9) sum++;
			if(ui[i+1][j]==9) sum++;
			if(ui[i+1][j+1]==9) sum++;
			ui[i][j]=sum;
		}
	}
}
```
现在，我们就知道了为什么前面的两个二维数组都要开大一点了：

**因为二维数组的索引是从`0`开始的，而我们使用`1`作为起始索引（具体见上方代码两个`for`循环中），就是为了避免在边缘上的方块无法获取到周围地雷数，从而导致程序出bug的原因**

然后，代码中的8个 **`if`** 语句都是统计周围地雷数的，如果你想简单==亿==点的话，把`if`语句替换成这一行：
```cpp
sum=!(ui[i-1][j-1]-9)+!(ui[i-1][j]-9)+!(ui[i-1][j+1]-9)+
!(ui[i][j-1]-9)+!(ui[i][j+1]-9)+!(ui[i+1][j-1]-9)+!(ui[i+1][j]-9)+!(ui[i+1][j+1]-9);
```
这里我分做两行写，是怕有的同学看不清，大家在实际替换中删掉换行符就可以了

**这一部分完整代码：**
```cpp
#include<bits/stdc++.h>
#include "windows.h"
/*
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <iomanip>
*/
#define random(x) 1+rand()%(x)
using namespace std;

int ui[12][12],b[12][12]; 
int main(){
	srand(time(NULL)); //random seed
	//system("color 1B");
	system("title MineSweeper");
	int cnt=10;//cout<<"Booms:";cin>>cnt;
	while(cnt){
		int x=random(10);//a+rand()%b = [a, a+b-1]
		int y=random(10);
		if(!ui[x][y]){//if(ui[x][y]==0){
			ui[x][y]=9;
			cnt--;
		}
	}
	
	for(int i=1;i<=10;i++){
		for(int j=1;j<=10;j++){
			int sum=0;
			if(ui[i][j]!=9){
				if(ui[i-1][j-1]==9) sum++;//sum=!(a[i-1][j-1]-9)+!(a[i-1][j]-9)...
				if(ui[i-1][j]==9) sum++;
				if(ui[i-1][j+1]==9) sum++;
				if(ui[i][j-1]==9) sum++;
				if(ui[i][j+1]==9) sum++;
				if(ui[i+1][j-1]==9) sum++;
				if(ui[i+1][j]==9) sum++;
				if(ui[i+1][j+1]==9) sum++;
				/*sum=!(ui[i-1][j-1]-9)+!(ui[i-1][j]-9)+!(ui[i-1][j+1]-9)+
				!(ui[i][j-1]-9)+!(ui[i][j+1]-9)+!(ui[i+1][j-1]-9)+
				!(ui[i+1][j]-9)+!(ui[i+1][j+1]-9);*/
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
	return 0;
}
```
大家把最后一段的注释符删掉，就可以看到我们的矩阵了
![test1](https://img-blog.csdnimg.cn/97473d1158ab455f9fbd07f761353aa1.png)

**但是，我们到现在才做完整个项目的一半，我们还没有写判断操作符的代码，这就要留到`Part 3`来讲了** 
***
### Part. 3 状态更新与判断输入
#### 刷新屏幕
我们先来讲刷新屏幕

**为什不先写判断输入的代码呢？**
**因为我们得让程序先把扫雷的矩阵输出出来**

我们看看这张图片
![minesweeper](https://img-blog.csdnimg.cn/cfefd39ae50a41589cb8d9c7db05668f.png#pic_center)
这是原版的扫雷界面，通过观察每个方格，可以发现，每个方格都会出现三种状态：**`已翻开(包括空格和数字，用1表示)、未翻开（包括数字和地雷）和旗子（用户标雷处）`**

那么，我们可以**用"`#`"表示未翻开的区域，用"`P`"表示旗子(🚩)，用`0~9`这十个数字表示已翻开区域**

**但是，井号(**`#`**)和旗子(**`P`**)都属于字符串类型，我们开的是数组类型`int`整型，存储不了字符串类型的字符，所以，我们不能把`#`和`P`赋值给二维数组`ui`里的某一项，而是得直接输出**

**逻辑：双重`for`循环遍历每个方格，如果该方块状态为未翻开，输出`#`；如果此方块状态为已翻开，直接输出二维数组`ui`里的这一项；如果此方块已被插旗，则输出`P`**

看看代码实现吧：
```cpp
for(int i=1;i<=10;i++){ 
	for(int j=1;j<=10;j++){ //遍历每个方格
		if(b[i][j]==0){ //如果此方块状态为未翻开
				cout<<"#"<<' '; //则输出#
			}else if(b[i][j]==1){ //如果此方块状态为已翻开
				cout<<ui[i][j]<<' '; //直接输出就行
			}else if(b[i][j]==2){ //如果此方块已被插旗
				cout<<"P"<<' '; //输出P（已插旗）
			}
		}cout<<endl;
```

#### 判断输入
先来定义一下这个游戏的规则：

**游戏一开始首先会打印一个``10×10``的井号(`#`)方阵，表示扫雷区，接下来让用户输入一个操作命令，再输入横纵坐标，表示要操作的方格。**

将雷全部扫完即为胜利，否之，踩到雷即为失败
***
操作符：

* **`q`代表翻开坐标处的井号，再输入`x`与`y`坐标，表示翻开该方格**
* **`p`代表在坐标处插旗，再输入`x`与`y`坐标，表示在这个方格上插旗(也就是标雷)**
* **`c`代表取消坐标位的旗，再输入`x`与`y`坐标，表示取消在该位置插旗**
***
坐标判断依据：

**上方所述的`x`,`y`坐标的具体位置：第`x`行第`y`个(从左往右数)**
比如这个矩阵(**`3*3`**):
* **``5 9 8``**
* **``2 0 3``**
* **``8 6 7``**

当 **`x=2`**，**`y=3`** 时，表示的数字为**3**
***

理解了上面这些逻辑，我们就有了一个大体的思路：
**设置状态变量`op`(`char`类型)与`x`，`y`（存储坐标）**
* **如果输入为`q`，如果该方块为地雷，直接输出`You died!`；如果为`0~8`的正常数字，将该方块状态设置为`已翻开`**
```cpp
if(op=='q'){
	if(ui[x][y]==9){
		cout<<"You died !";
		scanf("%d");
		return 0;
	}
	else{
		b[x][y]=1;
	}
}
```
**然后，再设置变量`k`与`k1`，`k`为地雷正确位置(客观)，`k1`为用户标雷位置(主观)；作用：存储总地雷数，如果`k==cnt(总雷数)`且`k==k1`的话，说明所有地雷已经扫完，且位置正确(没有把正常方格当成地雷)**

* **如果输入为`p`，将该方格设为`P`，状态设为`2`（标雷），`k1`自增；如果当前位置有雷，则说明用户判断正确，`k`自增**
```cpp
else if(op=='p'){
	k1++;
	if(ui[x][y]==9){
		k++;
	}	
	b[x][y]=2;
}
```
* **如果输入为`c`，如果该位置状态为`2`(插旗)，`k1`自减，如果该位置有地雷，`k`自减，然后将该格状态设置为`0`(未翻开)；如果该位置没有插旗，用户想撤销(也就是作弊；当然，你也可以删掉这个`if`，享受当赢家的快乐)，直接跳过(`continue`)**
```cpp
else if(op=='c'){
	if(b[x][y]==2){
		k1--;
		if(ui[x][y]==9){
			k--;
		}
		b[x][y]=0;
	}else{
		continue;
		}
	}
```
* **如果所有地雷已标记(扫完)，且位置正确，输出`You win!`**
```cpp
if(k==cnt && k==k1){
	cout<<"You win !";
	scanf("%d");
	return 0;
}
```
#### 最后，这个判断是要循环实现的。贴上我修改过的完整代码：
```cpp
#include<bits/stdc++.h>
#include "windows.h"
/*
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <iomanip>
*/
#define random(x) 1+rand()%(x)

using namespace std;

int ui[12][12],b[12][12]; 

int main(){
	srand(time(NULL)); //random seed
	//system("color 1B");
	system("title MineSweeper");
	int cnt=10;
	while(cnt){
		int x=random(10);//a+rand()%b = [a, a+b-1]
		int y=random(10);
		if(!ui[x][y]){//if(ui[x][y]==0){
			ui[x][y]=9;
			cnt--;
		}
	}
	cnt=10;
	for(int i=1;i<=cnt;i++){
		for(int j=1;j<=cnt;j++){
			int sum=0;
			if(ui[i][j]!=9){
				if(ui[i-1][j-1]==9) sum++;//sum=!(a[i-1][j-1]-9)+!(a[i-1][j]-9)...
				if(ui[i-1][j]==9) sum++;
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
	
	int k=0,k1=0;
	
	while(true){
		/*for(int i=1;i<=cnt1;i++){
			for(int j=1;j<=cnt1;j++){
			cout<<ui[i][j]<<' '; //output the numbers，cheat code(作弊代码)
		}
		cout<<endl;
	}cout<<endl; */
	
		for(int i=1;i<=cnt;i++){
			for(int j=1;j<=cnt;j++){
				if(b[i][j]==0){
					cout<<"#"<<' ';
				}else if(b[i][j]==1){
					cout<<ui[i][j]<<' ';
				}else if(b[i][j]==2){
					cout<<"P"<<' ';
				}
			}
			cout<<endl;
		}
		char op;
		cin>>op;
		int x,y;
		cin>>x>>y;
		system("cls");
		if(op=='q'){
			if(ui[x][y]==9){
				cout<<"You died !";
				scanf("%d");
				return 0;
			}
			else{
				b[x][y]=1;
			}
		}
		else if(op=='p'){
			k1++;
			if(ui[x][y]==9){
				k++;
			}	
			b[x][y]=2;
		}
		else if(op=='c'){
			if(b[x][y]==2){
				k1--;
				if(ui[x][y]==9){
					k--;
				}
				b[x][y]=0;
			}else{
				continue;
			}
		}
		if(k==cnt && k==k1){
			cout<<"You win !";
			scanf("%d");
			return 0;
		}
	}
    return 0;
} 
```
![MineSweeper](https://img-blog.csdnimg.cn/e3809886724847d9a6b2b751f3383274.png)
**但是，我们现在还有一个问题没有解决：*当翻开的数为`0`，且周围`8`格有为`0`的格子(也就是扫雷中的空格)时，我们应该自动显示出来***
下次再解决吧
***
**Update on 13/2/2024**
鸽了一个学期，终于更新了

这次增加了一些功能，解决了上次没完成的一个功能（上方斜体字）
但是还有一个功能（第一次点击不会是雷）没实现，试了很多方案，还问了一个编程很厉害的朋友，这是他给出的解决方案：
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/533eb565e8014f619e999864120b7140.png)
我也试过，还是没写出来，**在此求大佬指点**

ok，开始正题

### Part. 4 一些更新

#### 一、自动显示空白
这个比较简单，**只需判断当前点击的方格是否为`0`，是，判断周围`8`格是否有空白格(`0`)，是，将其翻开**
```cpp
if(b[x][y]==1&&ui[x][y]==0){//当前格已翻开且为0
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
```
加在操作符为“翻开”，也就是 **`q`** 的判断普通方块（**不为地雷`9`**）分支中，大家不用自己添加，后面会放出完整代码

#### 二、功能性更新
##### 1、生命值
这个功能的增加改善了可玩性
**在程序前面加一个lives的变量，每次踩到雷减一，并判断生命值是否为`0`，为`0`，停止游戏即可**
**每次生成雷阵后输出生命值**
简单代码：
```cpp
int lives=3;
/*-----中间部分-----*/
if(ui[x][y]==9){
	lives--;
	if(lives==0){
		return 0;
	}
}
```
##### 2、踩到旗子不掉血&踩到雷自动插旗
用户插下一个旗子，再次踩到时不会掉血，这个在上个版本没有考虑到
自动插旗让游戏更加方便，不用用户再次在雷的位置插旗
```cpp
if(b[x][y]==2) continue;//anti-blood on a flag
if(ui[x][y]==9){
	b[x][y]=2;
	lives--;
	if(lives==0){
		return 0;
	}
}
```
##### 3、上方未完成的功能的替代&非法操作符的判断
**设置一个bool类型的`firstClick`变量，开始为`true`，当为首次点击并踩到雷时，先将其设为`false`，再自动插旗，直接`continue`（不扣血）**

下方代码为功能性更新中的1，2，3板块的实现：
```cpp
bool firstClick=true;
/*-----中间部分-----*/
if(op=='q'){
	if(ui[x][y]==9){//losing
		if(firstClick){
			firstClick=false;
			b[x][y]=2;
			continue;
		}
		if(b[x][y]==2) continue;//anti-blood on a flag
		lives--;
		cout<<"oops! you just clicked a mine"<<endl<<endl;
		if(lives==0){
			cout<<"You Lose!";
			return 0;
		}
		b[x][y]=2;//auto-flag after one death
		continue;
		return 0;
	}
	else{
		firstClick=false;
		/*.....*/
	}
}
else if(op=='p'){//flagging
	firstClick=false;
	/*.....*/
}
else{
	cout<<"invalid operator\n";
	continue;
}
```
##### 4、自定义地图大小、生命值、雷数
**定义`row` `col` `mine_sum`变量，然后输入，判断数据合法性**
**再把后面`for`循环里面的10，10（上版本预设的地图大小）更改为`row`，`col`即可**
放个判断数据合法性的代码：
```cpp
int row,col,lives,mine_sum;
cin>>row>>col>>lives>>mine_sum;
while(row>=30||row<=0||col>=30||col<=0||
	lives>=(row*col)||lives==0||
	mine_sum>=(row*col)||mine_sum<=0){ //judge input data
		cout<<"failed to process data, please reset map size/health/mine_sum\n";
		cin>>row>>col>>lives>>mine_sum;
}
```
这里预设了地图大小不能超过30×30，且行数，列数均大于等于0，生命值必须小于 **`行数×列数`**（不然可以试出来），雷数必须小于 **`行数×列数`**
##### 5、Ai功能
看着像个重头戏，实际上实现很简单
**我将Ai功能的操作符设为`a`，后面接x,y坐标，表示从(1,1)开始到(x,y)结束的矩阵，Ai会在这个区间内查找 `ui[x][y]==9` 的点并自动标旗，询问是否输出矩阵，是，就输出从(1,1)开始到(x,y)结束的矩阵（后面实现了用户自定义矩阵）**

另外，由于程序原因，这里的x,y坐标并不是坐标系中的坐标：
![coord](https://img-blog.csdnimg.cn/direct/899bc02062d245639fc235477886e354.png#pic_center)
我加了一个变量用以存储Ai找出的雷（新增雷数），并且做了一个用户自定义矩阵：

**新增变量x1，y1，用以存储结束点的坐标，这样，第一次输入的坐标（如输入`a 1 1`）将成为起点坐标，接下来判断数据，x1，y1不能小于等于0，也不能大于行数列数**
```cpp
int x1,y1;
cin>>x1>>y1;
while(x1<=0||y1<=0||x1>row||y1>col){
	cout<<"invalid input\n";
	cin>>x1>>y1;
}
```
并将搜索范围设置为**从x到x1，从y到y1**

这部分的代码如下：
```cpp
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
			}
		}
	}
	cout<<new_mine<<" NEW mine(s) was(were) found\n";
	cout<<"output array?(y/n)"<<' ';
	char ans;
	cin>>ans;
	if(ans=='y'){
		cout<<"outputting..."<<'\n';
		for(int i=x;i<=x1;i++){
			for(int j=y;j<=y1;j++){
				cout<<ui[i][j]<<' ';
			}cout<<endl;
		}cout<<endl;
				
	}
	cout<<endl;
}
```
**游戏的完整代码如下：**
```cpp
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
```
**如果有任何bug，请在评论区指出，感谢各位！**

**Github开源地址：[这里](https://github.com/MeowCata/C-MineE/blob/main/MineE.cpp)，上不去的同学试试`FastGithub`**

这个md文件是我从CSDN复制过来的，Github对代码的排版有些问题，各位去CSDN看吧
 
**非常欢迎各位大佬指点**
