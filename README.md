# C-MineE
A minesweeper game based on C++

made by *DingDang* & **open source**

> [!IMPORTANT]
> **编译/运行环境：`C++11/Windows7+`**

> [!NOTE]
> Dev-Cpp 5.1.1 is supported

> [!IMPORTANT]
> 机房开了网络却没有游戏玩？就玩这个！下载站：[flippedcloud.github.io](https://flippedcloud.github.io)

### 玩法介绍
* 程序会提示输入：`<ROW> <COL> <HEALTH> <MINE_SUM>`，分别代表行数，列数（均小于等于30），生命值（小于行数×列数），雷数（小于行数×列数），有数据判断
* 操作符 **`q p c a`** +`X,Y`分别代表：在 **(x,y)** ²(见注释)  翻开/插旗/撤销插旗/使用Ai（作为起始点）³，以下为详细介绍
* q x y: 翻开位于 **(x,y)** 的方格，如果这是你的首次点击并且踩到雷，程序会自动帮你插旗并不扣血，如果不是，扣一滴血，自动插旗；如果当前格为空白（0），自动帮你翻开周围8格内所有的空白
* p x y: 在 **(x,y)** 插旗，表示你认为这里为雷，旗子没有数量限制，但是**只有插对地方且数量正确才会胜利**
* c x y: 撤销在 **(x,y)** 的插旗，如果该位置没有旗则不会操作
* a x y：将Ai排雷的起始点设置为 **(x,y)** ，再输入 **(x1,y1)** （具体坐标说明见下方补充部分），Ai将在这个范围内**自动排雷**，并为你插旗；你也可以选择输出这个范围内的数字
* NOTICE: 在1.1版本中，只需输入Ai终点，会自动在**(1,1)**到终点坐标排雷，更加便捷

### 一些补充
* ²：![coord](https://img-blog.csdnimg.cn/direct/899bc02062d245639fc235477886e354.png)
* ³：`见上方a x y处`

### My CSDN blog: [点这里](https://blog.csdn.net/weixin_45122104/article/details/128066651)

info：
![GitHub repo size](https://img.shields.io/github/repo-size/MeowCata/C-MineE)
![GitHub commit activity](https://img.shields.io/github/commit-activity/t/MeowCata/C-MineE)
![GitHub last commit](https://img.shields.io/github/last-commit/MeowCata/C-MineE)
![GitHub Repo stars](https://img.shields.io/github/stars/MeowCata/C-MineE?style=flat)
![GitHub forks](https://img.shields.io/github/forks/MeowCata/C-MineE?style=flat)

[![Star History Chart](https://api.star-history.com/svg?repos=MeowCata/C-MineE&type=Date)](https://star-history.com/#MeowCata/C-MineE&Date)
