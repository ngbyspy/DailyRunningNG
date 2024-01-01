/*
*
*		用VS打开该代码一定要修改属性！！！
*		用VS打开该代码一定要修改属性！！！
*		用VS打开该代码一定要修改属性！！！
*		用VS打开该代码一定要修改属性！！！
*		用VS打开该代码一定要修改属性！！！
*		用VS打开该代码一定要修改属性！！！
*		用VS打开该代码一定要修改属性！！！
*
*	修改方式：右键项目→属性→高级→字符集→使用多字节字符集
*							↘C/C++→SDL检查→否
*/
/*
	类天天酷跑项目日志
	1.创建项目
	2.安装easyX图形库
	3.创建游戏窗口
	4.实现游戏背景
		a.多重背景同时移动，速度不同
		b.两倍长度循环滚动
		c.渲染
		遇到问题：背景图片png显示黑色
		解决：使用自定义的更新函数
	5.实现奔跑
	6.实现跳跃
		遇到问题：无法跳跃，原因未知
		解决：修改判断接收输入的方式,不要使用
		a.更改跳跃机制为重力
		b.添加二段跳失败，该游戏也不适合二段跳
		遇到问题：空中的动作图片不显示，原因未知
		解决：图片加载的名称不对
	7.修改微调跳跃的高度等属性
	8.实现障碍物1的出现
	9.调整障碍物的模式，用结构体和vector进行封装
	10.实现封装后的障碍物出现
	11.实现控制台窗口的隐藏
		a.实现按键检测触发事件
	12.实现下蹲
	13.实现所有障碍物的出现
		a.优化障碍物出现的频率
	14.使用时间函数优化随机性
	15.实现碰撞检测
		a.角色身体为一个矩形，以此实现碰撞
		b.角色跳跃时候的身体矩形和奔跑是不同的，要另外判定
		c.角色的腿是三角形，需要比较详细的数据，有些困难
		*d.添加碰撞特效//暂时没加，主打一个懒
	16.实现游戏的音效添加
		a.等待优化：播放碰撞音效的时候会卡顿
		b.优化完成
	17.实现界面初始化，检查游戏资源完整性
	18.实现结束页面的音画同步
	19.显示当前分数
*/
#include<stdio.h>
#include<graphics.h>
#include"tools.h"
#include"Check.h"
#include"checkfiles.h"
#include<conio.h>
#include<vector>
#include<Windows.h>
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址
/*以下头文件为时间函数所需*/
#include <sys/timeb.h>
#include <cstdio>
#include <time.h>
#include <string>
/*以上头文件为时间函数所需*/

using namespace std;//申请命名空间	

#define WIN_WIDTH 1012
#define WIN_HEIGHT 396
#define OBSTACLE_COUNT 10
#define RandomNumbersMax 100+1

IMAGE imgBackgroud[4];//全局变量储存背景图
int bgX[4];//背景的坐标
int bgflySpeed[3] = { 2,3,7 };//角色正常时背景移动速度
//int bgflySpeed[3] = { 2,3,8 };//角色为芙宁娜是背景移动速度

IMAGE imgHeros[100];//储存角色奔跑图片
int heroX, heroY, heroFuY0, heroY0;//角色坐标
int heroIndex;//角色奔跑图片序号
bool heroLeft;//角色往左移动
bool heroRight;//角色往右移动

/*IMAGE imgHerojump[20];//储存角色跳跃图片
int herojumpIndex;*///角色跳跃图片序号

bool isHeroJumping;//角色的跳跃状态
bool isHeroGrounded; // 角色是否在地面上
double jumpSpeed;    // 角色当前的跳跃速度
#define GRAVITY 2.0 // 重力加速度
#define JUMP_INIT_SPEED 25 // 初始跳跃的速度
int fuNumAddfutNum = 43;//芙宁娜奔跑和跳跃的总照片数
/*在这里更新腿部三角形的坐标，因为fly函数里计算的基本都是index
*  0  1  2  3  4  5
*  x1 y1 x2 y2 x3 y3
*/
int heroLegsL[43][6] = {
{131,103,154,129,96,160},
{133,89,156,117,74,157},
{141,59,147,148,63,138},
{138,64,158,158,60,130},
{139,63,162,150,64,119},
{138,64,167,152,64,115},
{113,83,160,160,76,103},
{133,77,158,160,70,108},
{108,79,148,119,130,159},
{115,85,146,127,110,153},
{127,93,153,120,90,154},
{137,64,164,119,68,150},
{144,51,159,148,62,126},
{144,55,160,155,61,122},
{147,51,158,159,62,115},
{140,58,155,158,74,107},
{140,72,147,168,83,120},
{144,91,126,165,93,125},
{131,117,144,138,98,166},
{131,116,143,139,101,168},
{124,100,141,124,87,172},
{111,106,135,127,58,170},
{111,106,131,128,58,170},
{117,106,140,124,61,167},
{144,115,112,161,63,164},
{152,109,114,161,61,165},
{151,105,122,166,66,158},
{154,105,124,164,66,158},
{149,109,134,165,68,155},
{148,110,133,166,68,156},
{14,109,150,164,78,148},
{149,108,152,164,82,142},
{147,110,157,167,89,142},
{143,114,157,167,90,142},
{140,106,155,166,93,138},
{139,108,156,161,93,139},
{128,100,152,171,95,145},
{123,105,148,178,98,155},
{123,106,152,177,97,153},
{112,97,135,116,116,179},
{144,124,142,167,85,184},
{142,124,121,158,86,159},
{128,113,144,136,92,156}
};//角色腿部的相对位置

IMAGE imgHeroSquat[100];//角色下蹲图片
int squatImgNumbers = 8;
int squatX, squatY;//最终蹲下时芙宁娜的X和Y
bool ifHeroSquating;
bool keySProcessed;// 新增变量，判断S键是否已经处理过

bool ifupdate;//是否需要立即刷新页面
bool ifPause;//是否需要暂停
bool keyPProcessed;  // 新增变量，判断P键是否已经处理过

int random[RandomNumbersMax];//随机数集
int randomIndex;

int HP;
int SCORE;
int maxScore;
int level;
bool ifAdded;
IMAGE imgFailures[100];
int EnemyFre0 = 50;
int EnemyFre1 = 51;
int ReadMax;

typedef enum
{
	TORTOISE,//乌龟，0
	LION,//狮子，1
	ROLLINGTORT,//旋转的乌龟，2
	HOOK1,//3
	HOOK2,//4
	//HOOK3,//5
	//HOOK4,//6
	OBSTACLE_TYPE_COUNT //这是一种边界判断，放在最后一个，它对应的数刚好是障碍物的类型个数
}obstacle_type;

//存放所有障碍物的图片
vector<vector<IMAGE>> obstacleImgs; //相当于C语言的 IMAGE obstacleImgs[][];优点是行列可变

typedef struct obstacle
{
	obstacle_type type;//障碍物的类型，几号障碍物
	int imgIndex;//当前显示的图片序号
	int x, y;//障碍物的坐标
	int speed;//障碍物的速度
	int power;//碰一下障碍物会造成多少伤害
	int score;
	bool ifExist;//是否存在
	bool ifhited;//是都碰撞
	bool ifpassed;//是否通过
}obstacle_t;

obstacle_t obstacles[OBSTACLE_COUNT];

//以上是定义部分-----------------------------------------------------------------------------------------------------------------------------------
//以下是逻辑部分-----------------------------------------------------------------------------------------------------------------------------------

//时间函数，获取时间的最后两位，毫秒级，大大增加了随机性
int time_interval()
{
	struct timeb tv {};
	struct tm* t;

	ftime(&tv);

	time_t now = tv.time;
	t = localtime(&now);
	if (t == nullptr) {
		perror("localtime");
		return -1;
	}

	//printf("time_now: %d-%d-%d %d:%d:%d.%d\n", 1900+t->tm_year, 1+t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tv.millitm);
	return tv.millitm % 100;
}

//隐藏控制台的函数
void HideConsole()
{
	HWND hwnd = GetForegroundWindow();
	if (hwnd)
	{
		ShowWindow(hwnd, SW_HIDE);
	}
}
//隐藏窗口
HWND HideWindow()
{
	HWND hwnd = GetForegroundWindow();
	if (hwnd)
	{
		ShowWindow(hwnd, SW_HIDE);
	}
	return hwnd;
}
//显示窗口
void ShowWindow(HWND hwnd)
{
	if (hwnd)
	{
		ShowWindow(hwnd, SW_RESTORE);
		SetForegroundWindow(hwnd);
	}
}

//生成障碍物，写这个函数的时候对整个代码都已经比较熟悉了，没怎么写注释
void createObstacle()
{
	int i;
	for (i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].ifExist == false)
		{
			break;
		}
	}
	if (i >= OBSTACLE_COUNT) return;

	obstacles[i].ifExist = true;
	obstacles[i].ifhited = false;
	obstacles[i].ifpassed = false;
	obstacles[i].imgIndex = 0;
	if (randomIndex >= RandomNumbersMax) randomIndex = 0;
	random[randomIndex] = time_interval();
	obstacles[i].type = (obstacle_type)(random[randomIndex++] % (OBSTACLE_TYPE_COUNT - 0));//把多个柱子放到一起，不然柱子的出现概率会过大
	obstacles[i].x = WIN_WIDTH;
	obstacles[i].y = 360 - obstacleImgs[obstacles[i].type][0].getheight();
	if (obstacles[i].type == TORTOISE)
	{
		obstacles[i].speed = 1;
		obstacles[i].power = 5 + level * 5;
		obstacles[i].score = 5;
	}
	else if (obstacles[i].type == LION)
	{
		obstacles[i].speed = 4;
		obstacles[i].power = 15 + level * 5;
		obstacles[i].score = 11;
	}
	else if (obstacles[i].type == ROLLINGTORT)
	{
		obstacles[i].speed = 2;
		obstacles[i].power = 6 + level * 5;
		obstacles[i].score = 6;
	}
	else if (obstacles[i].type >= HOOK1 && obstacles[i].type <= HOOK2)
	{
		obstacles[i].speed = 0;
		obstacles[i].power = 21 + level * 5;
		obstacles[i].y = 0;
		obstacles[i].score = 12;
	}
}

//碰撞检测
void checkHit()
{
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].ifExist && !obstacles[i].ifhited)
		{
			MyTriangle HeroLegs;
			MyRectangle HeroBody, ObstacleBody;
			int a1x, a1y, a2x, a2y;//角色的身体矩形坐标信息
			if (!ifHeroSquating && !isHeroJumping)//奔跑的检测
			{
				HeroBody.x = heroX + 119;
				HeroBody.y = heroY + 38;
				a2x = heroX + imgHeros[i].getwidth() - 50;
				a2y = heroY + imgHeros[i].getheight() - 75;
				HeroBody.width = a2x - HeroBody.x;
				HeroBody.height = a2y - HeroBody.y;
				//以上是身体部分的矩形计算
				//由于三角的坐标可以直接使用heroX和heroY的相对位置，我们直接用就好

				HeroLegs.x1 = heroX + heroLegsL[heroIndex][0];
				HeroLegs.y1 = heroY + heroLegsL[heroIndex][1];
				HeroLegs.x2 = heroX + heroLegsL[heroIndex][2];
				HeroLegs.y2 = heroY + heroLegsL[heroIndex][3];
				HeroLegs.x3 = heroX + heroLegsL[heroIndex][4];
				HeroLegs.y3 = heroY + heroLegsL[heroIndex][5];

			}
			if ((isHeroJumping || !isHeroGrounded) && !ifHeroSquating)//在跳
			{
				HeroBody.x = heroX + 91;
				HeroBody.y = heroY + 26;
				a2x = heroX + imgHeros[i].getwidth() - 41;
				a2y = heroY + imgHeros[i].getheight() - 74;
				HeroBody.width = a2x - HeroBody.x;
				HeroBody.height = a2y - HeroBody.y;

				HeroLegs.x1 = heroX + heroLegsL[heroIndex][0];
				HeroLegs.y1 = heroY + heroLegsL[heroIndex][1];
				HeroLegs.x2 = heroX + heroLegsL[heroIndex][2];
				HeroLegs.y2 = heroY + heroLegsL[heroIndex][3];
				HeroLegs.x3 = heroX + heroLegsL[heroIndex][4];
				HeroLegs.y3 = heroY + heroLegsL[heroIndex][5];
			}
			if (ifHeroSquating)//按了S键
			{
				HeroBody.x = squatX + 5;
				HeroBody.y = squatY + 3;
				a2x = squatX + imgHeroSquat[squatImgNumbers - 1].getwidth() - 10;
				a2y = squatY + imgHeroSquat[squatImgNumbers - 1].getheight() - 6;
				HeroBody.width = a2x - HeroBody.x;
				HeroBody.height = a2y - HeroBody.y;
			}
			//障碍物的矩形坐标信息
			IMAGE img = obstacleImgs[obstacles[i].type][obstacles[i].imgIndex];
			int b1x = obstacles[i].x + 5;
			int b1y = obstacles[i].y + 5;
			int b2x = obstacles[i].x + img.getwidth() - 5;
			int b2y = obstacles[i].y + img.getheight();
			ObstacleBody.x = b1x;
			ObstacleBody.y = b1y;
			ObstacleBody.width = b2x - b1x;
			ObstacleBody.height = b2y - b1y;

			if (checkRectangleCollision(HeroBody, ObstacleBody))//记得添加“或者腿部碰撞”
			{
				HP -= obstacles[i].power;
				obstacles[i].ifhited = true;
				if (HP > 0)
				{
					mciSendString("play res/hit.mp3", 0, 0, 0);
				}
			}
			if (!ifHeroSquating && checkTriangleRectangleCollision(HeroLegs, ObstacleBody))
			{
				HP -= obstacles[i].power;
				obstacles[i].ifhited = true;
				if (HP > 0)
				{
					mciSendString("play res/hit.mp3", 0, 0, 0);
				}
			}
		}
		/*if (obstacles[i].ifExist && obstacles[i].ifhited)
		{
			putimagePNG2()
		}*/
	}
}

//加分检测
void checkScore()
{
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].ifExist && !obstacles[i].ifpassed && !obstacles[i].ifhited)
		{
			if (obstacles[i].x + obstacleImgs[obstacles[i].type][0].getwidth() < heroX)
			{
				SCORE += obstacles[i].score;
				maxScore += obstacles[i].score;
				obstacles[i].ifpassed = true;
				//printf("%d\n", SCORE);
			}
		}
		if (SCORE >= 100)
		{
			SCORE = 0;
			level++;
			ifAdded = false;
		}
		if (level >= 0 && !ifAdded)
		{
			for (int j = 0; j < 3; j++)
			{
				EnemyFre0 -= (level * 3);
				if (EnemyFre0 <= 25) EnemyFre0 = 25;
				EnemyFre1 -= (level * 2);
				if (EnemyFre1 < 30) EnemyFre1 = 30;
				bgflySpeed[j] += level;
			}
			ifAdded = true;
		}
	}
}

//渲染游戏背景
void updateBackGroud()
{
	putimagePNG2(bgX[0], 0, &imgBackgroud[0]);
	putimagePNG2(bgX[1], 119, &imgBackgroud[1]);
	putimagePNG2(bgX[2], 330, &imgBackgroud[2]);
	putimagePNG2(bgX[3], 0, &imgBackgroud[3]);
}

//玩家一的跳跃
void hero1jump()
{
	if (isHeroGrounded)
	{
		isHeroJumping = true;
		isHeroGrounded = false;        // 从地面脱离
		jumpSpeed = JUMP_INIT_SPEED;   // 设置初始跳跃速度
		heroIndex = 18;//芙宁娜的初始化
		//heroIndex = 9;//正常角色的初始化
	}
	/*if (isHeroGrounded || (!isHeroJumping && canDoubleJump)) // 允许二段跳
	{
		isHeroJumping = true;
		isHeroGrounded = false;
		jumpSpeed = JUMP_INIT_SPEED; // 设置初始跳跃速度

		// 如果在空中执行jump，则表示进行了二段跳
		if (!isHeroGrounded && !canDoubleJump)
			canDoubleJump = false; // 使用二段跳后将其设置为不可用
	}*///二段跳测试
}

//下蹲
void squat()
{
	ifHeroSquating = true;
	heroIndex = 0;
}

//检测玩家是否需要中途退出
void ifWannaExit()
{
	HWND hwnd = HideWindow();
	if (HP > 0)
	{
		if (IDYES == MessageBoxA(hwnd, "游戏未保存，是否退出？", "注意", MB_YESNO))
		{
			exit(0);
		}
	}
	else
	{
		if (IDYES == MessageBoxA(hwnd, "数据已保存，是否立即退出？", "DailyRunningNG", MB_YESNO))
		{
			exit(0);
		}
	}
	ShowWindow(hwnd);
}

//接收输入
void keyEvent()
{
	//玩家一的按键事件
	bool keyWPressed = (GetAsyncKeyState('W') & 0x8000) != 0;  // 玩家一上移
	bool keyAPressed = (GetAsyncKeyState('A') & 0x8000) != 0;  // 玩家一左移
	bool keySPressed = (GetAsyncKeyState('S') & 0x8000) != 0;  // 玩家一下移
	bool keyDPressed = (GetAsyncKeyState('D') & 0x8000) != 0;  // 玩家一右移

	//为玩家二测试做准备
	//bool keyUpPressed = (GetAsyncKeyState(VK_UP) & 0x8000) != 0;     // 玩家二上移
	//bool keyLeftPressed = (GetAsyncKeyState(VK_LEFT) & 0x8000) != 0;   // 玩家二左移
	//bool keyDownPressed = (GetAsyncKeyState(VK_DOWN) & 0x8000) != 0;   // 玩家二下移
	//bool keyRightPressed = (GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0;  // 玩家二右移

	//其他按键
	bool keyEscPressed = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;//Esc退出键
	bool keyPPressed = (GetAsyncKeyState('P') & 0x8000) != 0;

	// 判断输入
	if (keyWPressed && !isHeroJumping && !keySProcessed)
	{
		hero1jump();
		ifupdate = true;//可以刷新了
	}
	if (keyEscPressed)
	{
		ifWannaExit();
	}
	if (keyAPressed)
	{
		heroLeft = true;
	}
	if (keyDPressed)
	{
		heroRight = true;
	}

	//下面是检测P键的按下，由于GetAsyncKeyState函数仅检测单次按键状态，所以按下和弹起都是true，所以要加一个keyProcessed来判断按下和弹起
	if (keyPPressed && !keyPProcessed)
	{
		// 当P键被按下且之前未处理时，切换暂停状态
		ifPause = !ifPause;
		keyPProcessed = true;  // 标记为已处理
	}
	else if (!keyPPressed && keyPProcessed)
	{
		// 当P键被释放且之前已处理时，重置处理标志
		keyPProcessed = false;
	}

	//下面是检测S键的按下，理由与P键一样
	if (keySPressed && !keySProcessed)
	{
		// 当P键被按下且之前未处理时，切换暂停状态
		squat();
		keySProcessed = true;  // 标记为已处理
	}
	else if (!keySPressed && keySProcessed)
	{
		// 当P键被释放且之前已处理时，重置处理标志
		ifHeroSquating = !ifHeroSquating;
		keySProcessed = false;
	}
}

//更新敌方
void updateEnemy()
{
	//更新障碍物
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].ifExist)
		{
			putimagePNG2(obstacles[i].x, obstacles[i].y, WIN_WIDTH,
				&obstacleImgs[obstacles[i].type][obstacles[i].imgIndex]);
		}
	}
}

//更新角色
void updateHeros()
{
	if (!ifHeroSquating)
	{
		putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);//输出角色的奔跑和跳跃
	}
	if (ifHeroSquating)//复杂的坐标计算，唉，不过蹲下就不需要计算三角形了
	{
		if (heroIndex < squatImgNumbers - 6)
		{
			putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);
			putimagePNG2(heroX, heroY, &imgHeroSquat[heroIndex]);
		}
		else if (heroIndex < squatImgNumbers - 4)
		{
			putimagePNG2(heroX, heroY, &imgHeroSquat[heroIndex]);
		}
		else if (heroIndex < squatImgNumbers - 1)
		{
			int y = heroY + imgHeros[heroIndex].getheight() - imgHeroSquat[heroIndex].getheight();
			int x = heroX + imgHeros[0].getwidth() * 0.5 - imgHeroSquat[squatImgNumbers - 1].getwidth() * 0.5;
			int xChannel = heroX + imgHeros[0].getwidth() * 0.5 - imgHeroSquat[heroIndex].getwidth() * 0.5;
			int yChannel = heroY + imgHeros[heroIndex].getheight() - imgHeroSquat[heroIndex].getheight();
			putimagePNG2(x, y, &imgHeroSquat[squatImgNumbers - 1]);
			putimagePNG2(xChannel, yChannel, &imgHeroSquat[heroIndex]);
		}
		else
		{
			int y = heroY + imgHeros[heroIndex].getheight() - imgHeroSquat[heroIndex].getheight();
			int x = heroX + imgHeros[0].getwidth() * 0.5 - imgHeroSquat[heroIndex].getwidth() * 0.5;
			putimagePNG2(x, y, &imgHeroSquat[heroIndex]);
			squatX = x;
			squatY = y;
		}
	}
}

//更新血条和分数条
void updateBloodBar()
{
	drawBloodBar(10, 10, 200, 10, 2, BLUE, DARKGRAY, RED, HP / 100.0);//血条
	drawBloodBar(10, 30, 200, 10, 2, BLUE, DARKGRAY, YELLOW, SCORE / 100.0);//分数条

	//更新分数显示
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	//当前总分
	TCHAR strPresentScore[] = _T("当前总分：");
	outtextxy(WIN_WIDTH+10, 120, strPresentScore);
	TCHAR strScore[500];
	_stprintf(strScore, _T("%d"), maxScore);
	outtextxy(WIN_WIDTH + 90, 121, strScore);

	//历史最高分
	TCHAR strHistoryMaxScore[] = _T("历史最高分：");
	outtextxy(WIN_WIDTH + 10, 139, strHistoryMaxScore);
	TCHAR strScoreRead[500];
	_stprintf(strScoreRead, _T("%d"), ReadMax);
	outtextxy(WIN_WIDTH + 102, 140, strScoreRead);

}

//死亡检测
void checkHP()
{
	if (HP <= 0)
	{
		int grade = (maxScore >= ReadMax ? maxScore : ReadMax);
		WriteGrade(grade);
		mciSendString("stop res/traveling.mp3", 0, 0, 0);

		char name[64] = { 0 };
		int i = 0, j = 0;
		int timers = 0;
		int Tmax = 15;
		mciSendString("play res/failure.mp3", 0, 0, 0);
		while (i < 5446)
		{
			keyEvent();//接收输入函数
			timers += getDelay();//加上底层逻辑基础单位时间
			if (timers >= Tmax)
			{
				timers = 0;
				sprintf(name, "res/failure/%04d.jpg", i + 1);
				j = i % 10;//j从0到9
				loadimage(&imgFailures[j], name);
				putimage(0, 0, &imgFailures[j]);
				i++;
			}
		}

		mciSendString("stop res/failure.mp3", 0, 0, 0);
		//putimage(0, 0, &imgFailures[0]);
		ifWannaExit();
	}
}

//计算下一帧图片的数据
void fly()
{
	//背景移动
	for (int i = 0; i < 3; i++)
	{
		bgX[i] -= bgflySpeed[i];
		if (bgX[i] < -WIN_WIDTH)
			bgX[i] = 0;
	}

	//碰撞检测
	checkHit();

	//芙宁娜角色变化
	if (isHeroJumping)//芙宁娜角色跳跃变化
	{
		heroIndex++;
		if (heroIndex > 42) // 这里检查跳跃索引是否超出跳跃动画的范围
		{
			heroIndex = 0; // 如果是，则重置为奔跑动画的开始
		}
		heroY -= jumpSpeed;                     // 应用跳跃速度
		jumpSpeed -= GRAVITY;                   // 应用重力加速度

		// 如果英雄触地，则将其状态设置为地面状态
		if (heroY >= heroFuY0)
		{
			heroY = heroFuY0;
			isHeroJumping = false;
			isHeroGrounded = true;
			jumpSpeed = 0;// 重置跳跃速度
		}//芙宁娜的判定
		if (heroY >= heroY0)//正常角色判定
		{
			heroY = heroY0;
			isHeroJumping = false;
			isHeroGrounded = true;
			jumpSpeed = 0;// 重置跳跃速度
		}
	}
	if (ifHeroSquating)//下蹲
	{
		heroIndex++;
		if (heroIndex >= squatImgNumbers - 1)
		{
			heroIndex = squatImgNumbers - 1;
		}
	}
	if (!isHeroJumping && !ifHeroSquating)//没跳没蹲就是在跑
	{
		//角色奔跑变化
		if (heroIndex == fuNumAddfutNum - 1) heroIndex = 0;
		heroIndex = (heroIndex + 1) % 18;
	}
	if (heroLeft && !heroRight && heroX > 0)
	{
		heroX -= 5;
		if (heroX < 0)
		{
			heroX += 5;
		}
		heroLeft = false;
	}
	if (!heroLeft && heroRight)
	{
		heroX += 5;
		if (heroX >= WIN_WIDTH - imgHeros[heroIndex].getwidth())
		{
			heroX = WIN_WIDTH - imgHeros[heroIndex].getwidth();
		}
		heroRight = false;
	}
	if (heroLeft && heroRight)
	{
		heroLeft = false;
		heroRight = false;
	}
	//正常角色跳跃变化，版本较老，记得更新
	/*if (isHeroJumping)
	{
		heroY -= jumpSpeed;                     // 应用跳跃速度
		jumpSpeed -= GRAVITY;                   // 应用重力加速度

		// 如果角色触地，则将其状态设置为地面状态
		if (heroY >= heroFuY0)
		{
			heroY = heroFuY0;
			isHeroJumping = false;
			isHeroGrounded = true;
			jumpSpeed = 0;// 重置跳跃速度
		}
		if (heroY >= heroY0)//正常角色判定
		{
			heroY = heroY0;
			isHeroJumping = false;
			isHeroGrounded = true;
			jumpSpeed = 0;// 重置跳跃速度
		}
	}
	else
	{
		//角色奔跑变化
		if (heroIndex == 11) heroIndex = 0;
		heroIndex = (heroIndex + 1) % 12;
	}*/

	//显示障碍物的变化
	static int frameCount = 0;//定义静态变量扩大障碍物的域
	static int EnemyFre = 70;//每隔多少帧

	frameCount++;//数一下这是第几帧
	if (frameCount > EnemyFre)
	{
		frameCount = 0;
		if (randomIndex >= RandomNumbersMax) randomIndex = 0;
		random[randomIndex] = time_interval();
		EnemyFre = EnemyFre0 + random[randomIndex++] % EnemyFre1;
		createObstacle();
	}
	//更新障碍物的坐标
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].ifExist)
		{
			obstacles[i].x -= (bgflySpeed[2] + obstacles[i].speed);
			if (obstacles[i].x < -obstacleImgs[obstacles[i].type][obstacles[i].imgIndex].getwidth() * 2)
			{
				obstacles[i].ifExist = false;
			}
			int len = obstacleImgs[obstacles[i].type].size();
			obstacles[i].imgIndex = (obstacles[i].imgIndex + 1) % len;
		}
	}
}

//游戏初始化
void init()
{
	//隐藏控制台
	HideConsole();

	MessageBoxA(0, "不可沉迷游戏！该游戏是由一个新手写的，多次运行可能会出现异常！", "忠告", MB_OK);

	//创建游戏窗口
	initgraph(WIN_WIDTH + 196, WIN_HEIGHT);

	//初始音乐
	preLoadSound("res/beginning.mp3");
	mciSendString("play res/beginning.mp3", 0, 0, 0);

	//检查资源完整性
	IMAGE TEMP;
	IMAGE imgCHECKING;
	char tempName[64] = { 0 };
	char tempName2[64] = { 0 };
	sprintf(tempName, "res/checking.png");
	loadimage(&imgCHECKING, tempName);

	//int timers2 = 0;
	//int k = 0;
	//while (k < 5446)
	//{
	//	keyEvent();
	//	timers2 += getDelay();
	//	if (timers2 >= 11)
	//	{
	//		for (int l = 0; l < 23; l++)
	//		{
	//			tempName[l] = 0;
	//		}
	//		timers2 = 0;
	//		sprintf(tempName, "res/failure/%04d.jpg", k + 1);
	//		sprintf(tempName2, "%04d.jpg", k + 1);
	//		loadimage(&TEMP, tempName);
	//		char relativePath[] = ".\\res\\failure";  // 相对路径
	//		char filename[64] = { 0 };  // 要检查的文件名
	//		sprintf(filename, "%04d.jpg", k + 1);
	//		if (!isJpgFileExists(relativePath, filename))
	//		{
	//			HWND hwnd = HideWindow();
	//			MessageBoxA(hwnd, "资源加载错误！请联系管理员检查资源完整性！错误码：0x1110Failure", "警告", MB_OK);
	//			exit(0);
	//			ShowWindow(hwnd);
	//		}
	//		if (k == 11)puts(tempName);
	//		BeginBatchDraw();//开始渲染
	//		putimage((WIN_WIDTH + 196) / 2.0 - imgCHECKING.getwidth() / 2.0, WIN_HEIGHT - 100, &imgCHECKING);
	//		drawBloodBar((WIN_WIDTH + 196) / 2.0 - 200, WIN_HEIGHT - 70, 400, 5, 2, BLACK, DARKGRAY, WHITE, k / 5446.0);
	//		EndBatchDraw();//结束本次渲染
	//		k++;
	//	}
	//}

	mciSendString("stop res/beginning.mp3", 0, 0, 0);

	//加载背景资源
	char name[64];
	for (int i = 0; i < 3; i++)
	{
		sprintf(name, "res/bg%03d.png", i + 1);
		loadimage(&imgBackgroud[i], name);
		bgX[i] = 0;
		if (name == NULL)
		{
			HWND hwnd = HideWindow();
			MessageBoxA(hwnd, "资源加载错误！请联系管理员检查资源完整性！错误码：0x1101B_G_", "警告", MB_OK);
			exit(0);
			ShowWindow(hwnd);
		}
	}
	sprintf(name, "res/caozuo.png");
	loadimage(&imgBackgroud[3], name);
	bgX[3] = WIN_WIDTH;
	if (name == NULL)
	{
		HWND hwnd = HideWindow();
		MessageBoxA(hwnd, "资源加载错误！请联系管理员检查资源完整性！错误码：0x1100B_G_", "警告", MB_OK);
		exit(0);
		ShowWindow(hwnd);
	}


	//加载芙宁娜角色奔跑
	for (int i = 0; i < 18; i++)
	{
		sprintf(name, "res/fu%d.png", i + 1);
		loadimage(&imgHeros[i], name);
		if (name == NULL)
		{
			HWND hwnd = HideWindow();
			MessageBoxA(hwnd, "资源加载错误！请联系管理员检查资源完整性！错误码：0x1011RunningError", "警告", MB_OK);
			exit(0);
			ShowWindow(hwnd);
		}
	}
	//加载芙宁娜色跳跃
	for (int i = 18; i < fuNumAddfutNum; i++)
	{
		sprintf(name, "res/fut%d.png", i - 17);
		loadimage(&imgHeros[i], name);
		if (name == NULL)
		{
			HWND hwnd = HideWindow();
			MessageBoxA(hwnd, "资源加载错误！请联系管理员检查资源完整性！错误码：0x1010JumpingError", "警告", MB_OK);
			exit(0);
			ShowWindow(hwnd);
		}
	}
	//加载芙宁娜下蹲
	for (int i = 0; i < squatImgNumbers; i++)
	{
		sprintf(name, "res/fud%d.png", i + 1);
		loadimage(&imgHeroSquat[i], name);
		if (name == NULL)
		{
			HWND hwnd = HideWindow();
			MessageBoxA(hwnd, "资源加载错误！请联系管理员检查资源完整性！错误码：0x1000SquatingError", "警告", MB_OK);
			exit(0);
			ShowWindow(hwnd);
		}
	}

	//加载正常角色奔跑
	/*for (int i = 0; i < 12; i++)
	{
		sprintf(name, "res/hero%d.png", i + 1);
		loadimage(&imgHeros[i], name);
	}
	//加载正常角色跳跃
	sprintf(name, "res/hero%d.png", 10);
	loadimage(&imgHeros[12], name);*/

	//初始化角色位置信息
	heroX = WIN_WIDTH * 0.5 - imgHeros[0].getwidth() * 0.5;
	heroFuY0 = 370 - imgHeros[0].getheight();
	heroY0 = 360 - imgHeros[0].getheight();
	heroY = heroFuY0;//角色为芙宁娜的初始化
	//heroY = heroY0;//正常角色的初始化
	//初始图形第几帧
	heroIndex = 0;

	//加载角色跳跃数据
	isHeroJumping = false;//角色一开始没有在跳
	isHeroGrounded = true;   //初始时在地面上
	jumpSpeed = 0;//初速度为零
	ifHeroSquating = false;//刚开始肯定是不蹲的嘛

	ifupdate = true;//一开始是需要刷新的

	//封装加载障碍物
	//加载障碍物1-乌龟1
	IMAGE imgTort;
	loadimage(&imgTort, "res/t1.png");
	vector<IMAGE> imgTortArray;
	imgTortArray.push_back(imgTort);
	obstacleImgs.push_back(imgTortArray);
	//加载障碍物2-狮子
	IMAGE imgLion;
	vector<IMAGE> imgLionArray;
	for (int i = 0; i < 6; i++)
	{
		sprintf(name, "res/p%d.png", i + 1);
		loadimage(&imgLion, name);
		imgLionArray.push_back(imgLion);
	}
	obstacleImgs.push_back(imgLionArray);
	//加载障碍物3-旋转的乌龟
	IMAGE imgRollingTort;
	vector<IMAGE> imgRollingTortArray;
	for (int i = 0; i < 6; i++)
	{
		sprintf(name, "res/t%d.png", i + 2);
		loadimage(&imgRollingTort, name);
		imgRollingTortArray.push_back(imgRollingTort);
	}
	obstacleImgs.push_back(imgRollingTortArray);
	//加载障碍物4-柱子
	IMAGE imgHOOK;
	for (int i = 0; i < 4; i++)
	{
		vector<IMAGE> imgHookArray;
		sprintf(name, "res/h%d.png", i + 1);
		loadimage(&imgHOOK, name, 63, 260, true);
		imgHookArray.push_back(imgHOOK);
		obstacleImgs.push_back(imgHookArray);
	}
	//初始化障碍物池
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		obstacles[i].ifExist = false;
		obstacles[i].ifhited = false;
	}

	ifPause = false;
	keyPProcessed = false;
	keySProcessed = false;

	for (int i = 0; i < 100; i++) random[i] = 0;
	randomIndex = 0;

	HP = 100;
	SCORE = 0;
	level = 0;
	maxScore = 0;
	heroLeft = false;
	heroRight = false;
	ifAdded = true;
	ReadMax = ReadGrade();


	//预加载音效
	preLoadSound("res/hit.mp3");
	preLoadSound("res/failure.mp3");
	cleardevice(); // 清空屏幕内容
	//开始游戏
	sprintf(tempName, "res/start.png");
	loadimage(&imgCHECKING, tempName);
	for (int i = 0; i < 200; i++)
	{
		BeginBatchDraw();//开始渲染
		putimagePNG2((WIN_WIDTH + 196) / 2.0 - imgCHECKING.getwidth() / 2.0, WIN_HEIGHT - 100, &imgCHECKING);
		drawBloodBar((WIN_WIDTH + 196) / 2.0 - 200, WIN_HEIGHT - 70, 400, 5, 2, BLACK, DARKGRAY, WHITE, 1);
		EndBatchDraw();//结束本次渲染
		Sleep(10);
	}
	preLoadSound("res/traveling.mp3");
	mciSendString("play res/traveling.mp3 repeat", 0, 0, 0);
}

//主函数
int main(void)
{
	init();//启动主窗口

	int timer = 0;//使用计时器优化帧
	while (1)//每次循环是一帧
	{
		keyEvent();//接收输入函数
		timer += getDelay();//加上底层逻辑基础单位时间
		if ((timer > 30 || ifupdate) && !ifPause)
		{
			timer = 0;
			ifupdate = false;

			BeginBatchDraw();//绘画背景
			updateBackGroud();//更新背景
			//system("pause");//用来调试
			updateHeros();
			updateEnemy();
			updateBloodBar();
			//printf("%d\n", heroIndex);//用来调试
			EndBatchDraw();//结束本次渲染
			checkHP();
			checkScore();
			fly();//坐标移动，为下一次跳跃做准备；
		}
	}
	return 0;
}