/*
*
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*		��VS�򿪸ô���һ��Ҫ�޸����ԣ�����
*
*	�޸ķ�ʽ���Ҽ���Ŀ�����ԡ��߼����ַ�����ʹ�ö��ֽ��ַ���
*							�KC/C++��SDL������
*/
/*
	�����������Ŀ��־
	1.������Ŀ
	2.��װeasyXͼ�ο�
	3.������Ϸ����
	4.ʵ����Ϸ����
		a.���ر���ͬʱ�ƶ����ٶȲ�ͬ
		b.��������ѭ������
		c.��Ⱦ
		�������⣺����ͼƬpng��ʾ��ɫ
		�����ʹ���Զ���ĸ��º���
	5.ʵ�ֱ���
	6.ʵ����Ծ
		�������⣺�޷���Ծ��ԭ��δ֪
		������޸��жϽ�������ķ�ʽ,��Ҫʹ��
		a.������Ծ����Ϊ����
		b.��Ӷ�����ʧ�ܣ�����ϷҲ���ʺ϶�����
		�������⣺���еĶ���ͼƬ����ʾ��ԭ��δ֪
		�����ͼƬ���ص����Ʋ���
	7.�޸�΢����Ծ�ĸ߶ȵ�����
	8.ʵ���ϰ���1�ĳ���
	9.�����ϰ����ģʽ���ýṹ���vector���з�װ
	10.ʵ�ַ�װ����ϰ������
	11.ʵ�ֿ���̨���ڵ�����
		a.ʵ�ְ�����ⴥ���¼�
	12.ʵ���¶�
	13.ʵ�������ϰ���ĳ���
		a.�Ż��ϰ�����ֵ�Ƶ��
	14.ʹ��ʱ�亯���Ż������
	15.ʵ����ײ���
		a.��ɫ����Ϊһ�����Σ��Դ�ʵ����ײ
		b.��ɫ��Ծʱ���������κͱ����ǲ�ͬ�ģ�Ҫ�����ж�
		c.��ɫ�����������Σ���Ҫ�Ƚ���ϸ�����ݣ���Щ����
		*d.�����ײ��Ч//��ʱû�ӣ�����һ����
	16.ʵ����Ϸ����Ч���
		a.�ȴ��Ż���������ײ��Ч��ʱ��Ῠ��
		b.�Ż����
	17.ʵ�ֽ����ʼ���������Ϸ��Դ������
	18.ʵ�ֽ���ҳ�������ͬ��
	19.��ʾ��ǰ����
*/
#include<stdio.h>
#include<graphics.h>
#include"tools.h"
#include"Check.h"
#include"checkfiles.h"
#include<conio.h>
#include<vector>
#include<Windows.h>
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // ������ڵ�ַ
/*����ͷ�ļ�Ϊʱ�亯������*/
#include <sys/timeb.h>
#include <cstdio>
#include <time.h>
#include <string>
/*����ͷ�ļ�Ϊʱ�亯������*/

using namespace std;//���������ռ�	

#define WIN_WIDTH 1012
#define WIN_HEIGHT 396
#define OBSTACLE_COUNT 10
#define RandomNumbersMax 100+1

IMAGE imgBackgroud[4];//ȫ�ֱ������汳��ͼ
int bgX[4];//����������
int bgflySpeed[3] = { 2,3,7 };//��ɫ����ʱ�����ƶ��ٶ�
//int bgflySpeed[3] = { 2,3,8 };//��ɫΪܽ�����Ǳ����ƶ��ٶ�

IMAGE imgHeros[100];//�����ɫ����ͼƬ
int heroX, heroY, heroFuY0, heroY0;//��ɫ����
int heroIndex;//��ɫ����ͼƬ���
bool heroLeft;//��ɫ�����ƶ�
bool heroRight;//��ɫ�����ƶ�

/*IMAGE imgHerojump[20];//�����ɫ��ԾͼƬ
int herojumpIndex;*///��ɫ��ԾͼƬ���

bool isHeroJumping;//��ɫ����Ծ״̬
bool isHeroGrounded; // ��ɫ�Ƿ��ڵ�����
double jumpSpeed;    // ��ɫ��ǰ����Ծ�ٶ�
#define GRAVITY 2.0 // �������ٶ�
#define JUMP_INIT_SPEED 25 // ��ʼ��Ծ���ٶ�
int fuNumAddfutNum = 43;//ܽ���ȱ��ܺ���Ծ������Ƭ��
/*����������Ȳ������ε����꣬��Ϊfly���������Ļ�������index
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
};//��ɫ�Ȳ������λ��

IMAGE imgHeroSquat[100];//��ɫ�¶�ͼƬ
int squatImgNumbers = 8;
int squatX, squatY;//���ն���ʱܽ���ȵ�X��Y
bool ifHeroSquating;
bool keySProcessed;// �����������ж�S���Ƿ��Ѿ������

bool ifupdate;//�Ƿ���Ҫ����ˢ��ҳ��
bool ifPause;//�Ƿ���Ҫ��ͣ
bool keyPProcessed;  // �����������ж�P���Ƿ��Ѿ������

int random[RandomNumbersMax];//�������
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
	TORTOISE,//�ڹ꣬0
	LION,//ʨ�ӣ�1
	ROLLINGTORT,//��ת���ڹ꣬2
	HOOK1,//3
	HOOK2,//4
	//HOOK3,//5
	//HOOK4,//6
	OBSTACLE_TYPE_COUNT //����һ�ֱ߽��жϣ��������һ��������Ӧ�����պ����ϰ�������͸���
}obstacle_type;

//��������ϰ����ͼƬ
vector<vector<IMAGE>> obstacleImgs; //�൱��C���Ե� IMAGE obstacleImgs[][];�ŵ������пɱ�

typedef struct obstacle
{
	obstacle_type type;//�ϰ�������ͣ������ϰ���
	int imgIndex;//��ǰ��ʾ��ͼƬ���
	int x, y;//�ϰ��������
	int speed;//�ϰ�����ٶ�
	int power;//��һ���ϰ������ɶ����˺�
	int score;
	bool ifExist;//�Ƿ����
	bool ifhited;//�Ƕ���ײ
	bool ifpassed;//�Ƿ�ͨ��
}obstacle_t;

obstacle_t obstacles[OBSTACLE_COUNT];

//�����Ƕ��岿��-----------------------------------------------------------------------------------------------------------------------------------
//�������߼�����-----------------------------------------------------------------------------------------------------------------------------------

//ʱ�亯������ȡʱ��������λ�����뼶����������������
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

//���ؿ���̨�ĺ���
void HideConsole()
{
	HWND hwnd = GetForegroundWindow();
	if (hwnd)
	{
		ShowWindow(hwnd, SW_HIDE);
	}
}
//���ش���
HWND HideWindow()
{
	HWND hwnd = GetForegroundWindow();
	if (hwnd)
	{
		ShowWindow(hwnd, SW_HIDE);
	}
	return hwnd;
}
//��ʾ����
void ShowWindow(HWND hwnd)
{
	if (hwnd)
	{
		ShowWindow(hwnd, SW_RESTORE);
		SetForegroundWindow(hwnd);
	}
}

//�����ϰ��д���������ʱ����������붼�Ѿ��Ƚ���Ϥ�ˣ�û��ôдע��
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
	obstacles[i].type = (obstacle_type)(random[randomIndex++] % (OBSTACLE_TYPE_COUNT - 0));//�Ѷ�����ӷŵ�һ�𣬲�Ȼ���ӵĳ��ָ��ʻ����
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

//��ײ���
void checkHit()
{
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].ifExist && !obstacles[i].ifhited)
		{
			MyTriangle HeroLegs;
			MyRectangle HeroBody, ObstacleBody;
			int a1x, a1y, a2x, a2y;//��ɫ���������������Ϣ
			if (!ifHeroSquating && !isHeroJumping)//���ܵļ��
			{
				HeroBody.x = heroX + 119;
				HeroBody.y = heroY + 38;
				a2x = heroX + imgHeros[i].getwidth() - 50;
				a2y = heroY + imgHeros[i].getheight() - 75;
				HeroBody.width = a2x - HeroBody.x;
				HeroBody.height = a2y - HeroBody.y;
				//���������岿�ֵľ��μ���
				//�������ǵ��������ֱ��ʹ��heroX��heroY�����λ�ã�����ֱ���þͺ�

				HeroLegs.x1 = heroX + heroLegsL[heroIndex][0];
				HeroLegs.y1 = heroY + heroLegsL[heroIndex][1];
				HeroLegs.x2 = heroX + heroLegsL[heroIndex][2];
				HeroLegs.y2 = heroY + heroLegsL[heroIndex][3];
				HeroLegs.x3 = heroX + heroLegsL[heroIndex][4];
				HeroLegs.y3 = heroY + heroLegsL[heroIndex][5];

			}
			if ((isHeroJumping || !isHeroGrounded) && !ifHeroSquating)//����
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
			if (ifHeroSquating)//����S��
			{
				HeroBody.x = squatX + 5;
				HeroBody.y = squatY + 3;
				a2x = squatX + imgHeroSquat[squatImgNumbers - 1].getwidth() - 10;
				a2y = squatY + imgHeroSquat[squatImgNumbers - 1].getheight() - 6;
				HeroBody.width = a2x - HeroBody.x;
				HeroBody.height = a2y - HeroBody.y;
			}
			//�ϰ���ľ���������Ϣ
			IMAGE img = obstacleImgs[obstacles[i].type][obstacles[i].imgIndex];
			int b1x = obstacles[i].x + 5;
			int b1y = obstacles[i].y + 5;
			int b2x = obstacles[i].x + img.getwidth() - 5;
			int b2y = obstacles[i].y + img.getheight();
			ObstacleBody.x = b1x;
			ObstacleBody.y = b1y;
			ObstacleBody.width = b2x - b1x;
			ObstacleBody.height = b2y - b1y;

			if (checkRectangleCollision(HeroBody, ObstacleBody))//�ǵ���ӡ������Ȳ���ײ��
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

//�ӷּ��
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

//��Ⱦ��Ϸ����
void updateBackGroud()
{
	putimagePNG2(bgX[0], 0, &imgBackgroud[0]);
	putimagePNG2(bgX[1], 119, &imgBackgroud[1]);
	putimagePNG2(bgX[2], 330, &imgBackgroud[2]);
	putimagePNG2(bgX[3], 0, &imgBackgroud[3]);
}

//���һ����Ծ
void hero1jump()
{
	if (isHeroGrounded)
	{
		isHeroJumping = true;
		isHeroGrounded = false;        // �ӵ�������
		jumpSpeed = JUMP_INIT_SPEED;   // ���ó�ʼ��Ծ�ٶ�
		heroIndex = 18;//ܽ���ȵĳ�ʼ��
		//heroIndex = 9;//������ɫ�ĳ�ʼ��
	}
	/*if (isHeroGrounded || (!isHeroJumping && canDoubleJump)) // ���������
	{
		isHeroJumping = true;
		isHeroGrounded = false;
		jumpSpeed = JUMP_INIT_SPEED; // ���ó�ʼ��Ծ�ٶ�

		// ����ڿ���ִ��jump�����ʾ�����˶�����
		if (!isHeroGrounded && !canDoubleJump)
			canDoubleJump = false; // ʹ�ö�������������Ϊ������
	}*///����������
}

//�¶�
void squat()
{
	ifHeroSquating = true;
	heroIndex = 0;
}

//�������Ƿ���Ҫ��;�˳�
void ifWannaExit()
{
	HWND hwnd = HideWindow();
	if (HP > 0)
	{
		if (IDYES == MessageBoxA(hwnd, "��Ϸδ���棬�Ƿ��˳���", "ע��", MB_YESNO))
		{
			exit(0);
		}
	}
	else
	{
		if (IDYES == MessageBoxA(hwnd, "�����ѱ��棬�Ƿ������˳���", "DailyRunningNG", MB_YESNO))
		{
			exit(0);
		}
	}
	ShowWindow(hwnd);
}

//��������
void keyEvent()
{
	//���һ�İ����¼�
	bool keyWPressed = (GetAsyncKeyState('W') & 0x8000) != 0;  // ���һ����
	bool keyAPressed = (GetAsyncKeyState('A') & 0x8000) != 0;  // ���һ����
	bool keySPressed = (GetAsyncKeyState('S') & 0x8000) != 0;  // ���һ����
	bool keyDPressed = (GetAsyncKeyState('D') & 0x8000) != 0;  // ���һ����

	//Ϊ��Ҷ�������׼��
	//bool keyUpPressed = (GetAsyncKeyState(VK_UP) & 0x8000) != 0;     // ��Ҷ�����
	//bool keyLeftPressed = (GetAsyncKeyState(VK_LEFT) & 0x8000) != 0;   // ��Ҷ�����
	//bool keyDownPressed = (GetAsyncKeyState(VK_DOWN) & 0x8000) != 0;   // ��Ҷ�����
	//bool keyRightPressed = (GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0;  // ��Ҷ�����

	//��������
	bool keyEscPressed = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;//Esc�˳���
	bool keyPPressed = (GetAsyncKeyState('P') & 0x8000) != 0;

	// �ж�����
	if (keyWPressed && !isHeroJumping && !keySProcessed)
	{
		hero1jump();
		ifupdate = true;//����ˢ����
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

	//�����Ǽ��P���İ��£�����GetAsyncKeyState��������ⵥ�ΰ���״̬�����԰��º͵�����true������Ҫ��һ��keyProcessed���жϰ��º͵���
	if (keyPPressed && !keyPProcessed)
	{
		// ��P����������֮ǰδ����ʱ���л���ͣ״̬
		ifPause = !ifPause;
		keyPProcessed = true;  // ���Ϊ�Ѵ���
	}
	else if (!keyPPressed && keyPProcessed)
	{
		// ��P�����ͷ���֮ǰ�Ѵ���ʱ�����ô����־
		keyPProcessed = false;
	}

	//�����Ǽ��S���İ��£�������P��һ��
	if (keySPressed && !keySProcessed)
	{
		// ��P����������֮ǰδ����ʱ���л���ͣ״̬
		squat();
		keySProcessed = true;  // ���Ϊ�Ѵ���
	}
	else if (!keySPressed && keySProcessed)
	{
		// ��P�����ͷ���֮ǰ�Ѵ���ʱ�����ô����־
		ifHeroSquating = !ifHeroSquating;
		keySProcessed = false;
	}
}

//���µз�
void updateEnemy()
{
	//�����ϰ���
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].ifExist)
		{
			putimagePNG2(obstacles[i].x, obstacles[i].y, WIN_WIDTH,
				&obstacleImgs[obstacles[i].type][obstacles[i].imgIndex]);
		}
	}
}

//���½�ɫ
void updateHeros()
{
	if (!ifHeroSquating)
	{
		putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);//�����ɫ�ı��ܺ���Ծ
	}
	if (ifHeroSquating)//���ӵ�������㣬�����������¾Ͳ���Ҫ������������
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

//����Ѫ���ͷ�����
void updateBloodBar()
{
	drawBloodBar(10, 10, 200, 10, 2, BLUE, DARKGRAY, RED, HP / 100.0);//Ѫ��
	drawBloodBar(10, 30, 200, 10, 2, BLUE, DARKGRAY, YELLOW, SCORE / 100.0);//������

	//���·�����ʾ
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	//��ǰ�ܷ�
	TCHAR strPresentScore[] = _T("��ǰ�ܷ֣�");
	outtextxy(WIN_WIDTH+10, 120, strPresentScore);
	TCHAR strScore[500];
	_stprintf(strScore, _T("%d"), maxScore);
	outtextxy(WIN_WIDTH + 90, 121, strScore);

	//��ʷ��߷�
	TCHAR strHistoryMaxScore[] = _T("��ʷ��߷֣�");
	outtextxy(WIN_WIDTH + 10, 139, strHistoryMaxScore);
	TCHAR strScoreRead[500];
	_stprintf(strScoreRead, _T("%d"), ReadMax);
	outtextxy(WIN_WIDTH + 102, 140, strScoreRead);

}

//�������
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
			keyEvent();//�������뺯��
			timers += getDelay();//���ϵײ��߼�������λʱ��
			if (timers >= Tmax)
			{
				timers = 0;
				sprintf(name, "res/failure/%04d.jpg", i + 1);
				j = i % 10;//j��0��9
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

//������һ֡ͼƬ������
void fly()
{
	//�����ƶ�
	for (int i = 0; i < 3; i++)
	{
		bgX[i] -= bgflySpeed[i];
		if (bgX[i] < -WIN_WIDTH)
			bgX[i] = 0;
	}

	//��ײ���
	checkHit();

	//ܽ���Ƚ�ɫ�仯
	if (isHeroJumping)//ܽ���Ƚ�ɫ��Ծ�仯
	{
		heroIndex++;
		if (heroIndex > 42) // ��������Ծ�����Ƿ񳬳���Ծ�����ķ�Χ
		{
			heroIndex = 0; // ����ǣ�������Ϊ���ܶ����Ŀ�ʼ
		}
		heroY -= jumpSpeed;                     // Ӧ����Ծ�ٶ�
		jumpSpeed -= GRAVITY;                   // Ӧ���������ٶ�

		// ���Ӣ�۴��أ�����״̬����Ϊ����״̬
		if (heroY >= heroFuY0)
		{
			heroY = heroFuY0;
			isHeroJumping = false;
			isHeroGrounded = true;
			jumpSpeed = 0;// ������Ծ�ٶ�
		}//ܽ���ȵ��ж�
		if (heroY >= heroY0)//������ɫ�ж�
		{
			heroY = heroY0;
			isHeroJumping = false;
			isHeroGrounded = true;
			jumpSpeed = 0;// ������Ծ�ٶ�
		}
	}
	if (ifHeroSquating)//�¶�
	{
		heroIndex++;
		if (heroIndex >= squatImgNumbers - 1)
		{
			heroIndex = squatImgNumbers - 1;
		}
	}
	if (!isHeroJumping && !ifHeroSquating)//û��û�׾�������
	{
		//��ɫ���ܱ仯
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
	//������ɫ��Ծ�仯���汾���ϣ��ǵø���
	/*if (isHeroJumping)
	{
		heroY -= jumpSpeed;                     // Ӧ����Ծ�ٶ�
		jumpSpeed -= GRAVITY;                   // Ӧ���������ٶ�

		// �����ɫ���أ�����״̬����Ϊ����״̬
		if (heroY >= heroFuY0)
		{
			heroY = heroFuY0;
			isHeroJumping = false;
			isHeroGrounded = true;
			jumpSpeed = 0;// ������Ծ�ٶ�
		}
		if (heroY >= heroY0)//������ɫ�ж�
		{
			heroY = heroY0;
			isHeroJumping = false;
			isHeroGrounded = true;
			jumpSpeed = 0;// ������Ծ�ٶ�
		}
	}
	else
	{
		//��ɫ���ܱ仯
		if (heroIndex == 11) heroIndex = 0;
		heroIndex = (heroIndex + 1) % 12;
	}*/

	//��ʾ�ϰ���ı仯
	static int frameCount = 0;//���徲̬���������ϰ������
	static int EnemyFre = 70;//ÿ������֡

	frameCount++;//��һ�����ǵڼ�֡
	if (frameCount > EnemyFre)
	{
		frameCount = 0;
		if (randomIndex >= RandomNumbersMax) randomIndex = 0;
		random[randomIndex] = time_interval();
		EnemyFre = EnemyFre0 + random[randomIndex++] % EnemyFre1;
		createObstacle();
	}
	//�����ϰ��������
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

//��Ϸ��ʼ��
void init()
{
	//���ؿ���̨
	HideConsole();

	MessageBoxA(0, "���ɳ�����Ϸ������Ϸ����һ������д�ģ�������п��ܻ�����쳣��", "�Ҹ�", MB_OK);

	//������Ϸ����
	initgraph(WIN_WIDTH + 196, WIN_HEIGHT);

	//��ʼ����
	preLoadSound("res/beginning.mp3");
	mciSendString("play res/beginning.mp3", 0, 0, 0);

	//�����Դ������
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
	//		char relativePath[] = ".\\res\\failure";  // ���·��
	//		char filename[64] = { 0 };  // Ҫ�����ļ���
	//		sprintf(filename, "%04d.jpg", k + 1);
	//		if (!isJpgFileExists(relativePath, filename))
	//		{
	//			HWND hwnd = HideWindow();
	//			MessageBoxA(hwnd, "��Դ���ش�������ϵ����Ա�����Դ�����ԣ������룺0x1110Failure", "����", MB_OK);
	//			exit(0);
	//			ShowWindow(hwnd);
	//		}
	//		if (k == 11)puts(tempName);
	//		BeginBatchDraw();//��ʼ��Ⱦ
	//		putimage((WIN_WIDTH + 196) / 2.0 - imgCHECKING.getwidth() / 2.0, WIN_HEIGHT - 100, &imgCHECKING);
	//		drawBloodBar((WIN_WIDTH + 196) / 2.0 - 200, WIN_HEIGHT - 70, 400, 5, 2, BLACK, DARKGRAY, WHITE, k / 5446.0);
	//		EndBatchDraw();//����������Ⱦ
	//		k++;
	//	}
	//}

	mciSendString("stop res/beginning.mp3", 0, 0, 0);

	//���ر�����Դ
	char name[64];
	for (int i = 0; i < 3; i++)
	{
		sprintf(name, "res/bg%03d.png", i + 1);
		loadimage(&imgBackgroud[i], name);
		bgX[i] = 0;
		if (name == NULL)
		{
			HWND hwnd = HideWindow();
			MessageBoxA(hwnd, "��Դ���ش�������ϵ����Ա�����Դ�����ԣ������룺0x1101B_G_", "����", MB_OK);
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
		MessageBoxA(hwnd, "��Դ���ش�������ϵ����Ա�����Դ�����ԣ������룺0x1100B_G_", "����", MB_OK);
		exit(0);
		ShowWindow(hwnd);
	}


	//����ܽ���Ƚ�ɫ����
	for (int i = 0; i < 18; i++)
	{
		sprintf(name, "res/fu%d.png", i + 1);
		loadimage(&imgHeros[i], name);
		if (name == NULL)
		{
			HWND hwnd = HideWindow();
			MessageBoxA(hwnd, "��Դ���ش�������ϵ����Ա�����Դ�����ԣ������룺0x1011RunningError", "����", MB_OK);
			exit(0);
			ShowWindow(hwnd);
		}
	}
	//����ܽ����ɫ��Ծ
	for (int i = 18; i < fuNumAddfutNum; i++)
	{
		sprintf(name, "res/fut%d.png", i - 17);
		loadimage(&imgHeros[i], name);
		if (name == NULL)
		{
			HWND hwnd = HideWindow();
			MessageBoxA(hwnd, "��Դ���ش�������ϵ����Ա�����Դ�����ԣ������룺0x1010JumpingError", "����", MB_OK);
			exit(0);
			ShowWindow(hwnd);
		}
	}
	//����ܽ�����¶�
	for (int i = 0; i < squatImgNumbers; i++)
	{
		sprintf(name, "res/fud%d.png", i + 1);
		loadimage(&imgHeroSquat[i], name);
		if (name == NULL)
		{
			HWND hwnd = HideWindow();
			MessageBoxA(hwnd, "��Դ���ش�������ϵ����Ա�����Դ�����ԣ������룺0x1000SquatingError", "����", MB_OK);
			exit(0);
			ShowWindow(hwnd);
		}
	}

	//����������ɫ����
	/*for (int i = 0; i < 12; i++)
	{
		sprintf(name, "res/hero%d.png", i + 1);
		loadimage(&imgHeros[i], name);
	}
	//����������ɫ��Ծ
	sprintf(name, "res/hero%d.png", 10);
	loadimage(&imgHeros[12], name);*/

	//��ʼ����ɫλ����Ϣ
	heroX = WIN_WIDTH * 0.5 - imgHeros[0].getwidth() * 0.5;
	heroFuY0 = 370 - imgHeros[0].getheight();
	heroY0 = 360 - imgHeros[0].getheight();
	heroY = heroFuY0;//��ɫΪܽ���ȵĳ�ʼ��
	//heroY = heroY0;//������ɫ�ĳ�ʼ��
	//��ʼͼ�εڼ�֡
	heroIndex = 0;

	//���ؽ�ɫ��Ծ����
	isHeroJumping = false;//��ɫһ��ʼû������
	isHeroGrounded = true;   //��ʼʱ�ڵ�����
	jumpSpeed = 0;//���ٶ�Ϊ��
	ifHeroSquating = false;//�տ�ʼ�϶��ǲ��׵���

	ifupdate = true;//һ��ʼ����Ҫˢ�µ�

	//��װ�����ϰ���
	//�����ϰ���1-�ڹ�1
	IMAGE imgTort;
	loadimage(&imgTort, "res/t1.png");
	vector<IMAGE> imgTortArray;
	imgTortArray.push_back(imgTort);
	obstacleImgs.push_back(imgTortArray);
	//�����ϰ���2-ʨ��
	IMAGE imgLion;
	vector<IMAGE> imgLionArray;
	for (int i = 0; i < 6; i++)
	{
		sprintf(name, "res/p%d.png", i + 1);
		loadimage(&imgLion, name);
		imgLionArray.push_back(imgLion);
	}
	obstacleImgs.push_back(imgLionArray);
	//�����ϰ���3-��ת���ڹ�
	IMAGE imgRollingTort;
	vector<IMAGE> imgRollingTortArray;
	for (int i = 0; i < 6; i++)
	{
		sprintf(name, "res/t%d.png", i + 2);
		loadimage(&imgRollingTort, name);
		imgRollingTortArray.push_back(imgRollingTort);
	}
	obstacleImgs.push_back(imgRollingTortArray);
	//�����ϰ���4-����
	IMAGE imgHOOK;
	for (int i = 0; i < 4; i++)
	{
		vector<IMAGE> imgHookArray;
		sprintf(name, "res/h%d.png", i + 1);
		loadimage(&imgHOOK, name, 63, 260, true);
		imgHookArray.push_back(imgHOOK);
		obstacleImgs.push_back(imgHookArray);
	}
	//��ʼ���ϰ����
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


	//Ԥ������Ч
	preLoadSound("res/hit.mp3");
	preLoadSound("res/failure.mp3");
	cleardevice(); // �����Ļ����
	//��ʼ��Ϸ
	sprintf(tempName, "res/start.png");
	loadimage(&imgCHECKING, tempName);
	for (int i = 0; i < 200; i++)
	{
		BeginBatchDraw();//��ʼ��Ⱦ
		putimagePNG2((WIN_WIDTH + 196) / 2.0 - imgCHECKING.getwidth() / 2.0, WIN_HEIGHT - 100, &imgCHECKING);
		drawBloodBar((WIN_WIDTH + 196) / 2.0 - 200, WIN_HEIGHT - 70, 400, 5, 2, BLACK, DARKGRAY, WHITE, 1);
		EndBatchDraw();//����������Ⱦ
		Sleep(10);
	}
	preLoadSound("res/traveling.mp3");
	mciSendString("play res/traveling.mp3 repeat", 0, 0, 0);
}

//������
int main(void)
{
	init();//����������

	int timer = 0;//ʹ�ü�ʱ���Ż�֡
	while (1)//ÿ��ѭ����һ֡
	{
		keyEvent();//�������뺯��
		timer += getDelay();//���ϵײ��߼�������λʱ��
		if ((timer > 30 || ifupdate) && !ifPause)
		{
			timer = 0;
			ifupdate = false;

			BeginBatchDraw();//�滭����
			updateBackGroud();//���±���
			//system("pause");//��������
			updateHeros();
			updateEnemy();
			updateBloodBar();
			//printf("%d\n", heroIndex);//��������
			EndBatchDraw();//����������Ⱦ
			checkHP();
			checkScore();
			fly();//�����ƶ���Ϊ��һ����Ծ��׼����
		}
	}
	return 0;
}