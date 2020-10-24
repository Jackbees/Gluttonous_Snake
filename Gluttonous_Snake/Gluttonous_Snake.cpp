#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include <wchar.h>
#include<stdlib.h>
#include<time.h>






#pragma comment(lib,"winmm.lib")
#define HIGH 600
#define WIDTH 800
#define FilePath "scores.txt"
#define NUM 1024
#define N 20  
#define SNAKELEN 1200  //蛇的最大长度





void Drawmap();  //地图
void displaymainmenu();
void gamerun();//运行游戏
void writescore(); //写入分数
void showscores();  //展示分数排行
void createfood();  //食物
void loadingimage(); //加载图片
void createsnake(); //初始化蛇
void drawsnake(); //画蛇
void move();  //移动
void drawt();//画背景
void addscore();  //显示分数
void HeadDirection(); //该变方向
void savescore();  //保存分数
void eat(); //判断是否吃到食物
void gameover();  //死亡画面
void GameEnd();  //判断是否死亡


MOUSEMSG main_m;  //鼠标信息
char main_c='\0';//记录按键
FILE* fp; //文件操作
int s=100;//分数
int speed = 80; //最慢速度
int isdead ;  //标志死亡


IMAGE snakedown, snakeup, snakeleft, snakeright, snakebody, foodimage;



enum DIRE                     //控制的方向的结构体
{
	up = 72,				//上、下、左、右、空格键所对应的ASCII码
	down = 80,
	left = 75,
	right = 77,
	space = 32
};



//分数结构体
typedef struct scoreinfo
{
	int score;
}scinfo;




//蛇的坐标
typedef struct posion			//表示坐标的结构体
{
	int x;
	int y;
}posi; 



struct snakeinfo 				//定义结构体，来存储蛇的相关信息
{
	posi crd[SNAKELEN];		//保存蛇的每个结数的坐标，最多有1000节
	int node;				//保存蛇当前节数
	DIRE dir;				//当前蛇行进的方向
}snake;




struct FOOD					//食物的结构体
{
	posi crd;				//食物坐标
	int flag;				//表示有没有食物 1代表没有食物，没有食物需要生成食物
}food;





//主函数
int main()
{
	Drawmap();
	displaymainmenu();
	//savescore();
	getchar();
}











void Drawmap()//初始化
{
	int i = 0;
	mciSendString("open C:\\Users\\50402\\Desktop\\贪吃蛇素材资料\\1.mp3 alias bk", NULL, 0, NULL);
	mciSendString("play bk repeat", NULL, 0, NULL);
	initgraph(800, 600);
	setbkcolor(RGB(245, 245, 245));
	cleardevice();
	drawt();
}


void displaymainmenu()
{
	int mark = 0;//标志判断是否进入其他页面
	settextcolor(RGB(255, 192, 203));
	RECT r1 = { 360,210,440,235 }, r2 = { 350,245,450,270 }, r3 = { 350,280,450,305 };
	drawtext("开始游戏", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("排行榜", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("返回桌面", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	while (1) {
		while (MouseHit()) {
			if (mark == 1) {
				cleardevice();
				drawt();
				mark = 0;
			}
			main_m = GetMouseMsg();
			if (main_m.x <= 450 && main_m.x >= 350)
			{
				if (main_m.y <= 255 && main_m.y >= 210) {//开始游戏
					setcolor(RGB(160, 32, 240));
					drawtext("开始游戏", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					if (main_m.uMsg == WM_LBUTTONDOWN)    {
					gamerun();
					gameover();
					mark = 1;
				    }
				}
				if (main_m.y <= 270 && main_m.y >= 245) {//分数排行
					setcolor(RGB(160, 32, 240));
					drawtext("排行榜", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					if (main_m.uMsg == WM_LBUTTONDOWN) {
						showscores();
						mark = 1;
					}
				}
				if (main_m.y >= 280 && main_m.y <= 305) {
					setcolor(RGB(160, 32, 240));
					drawtext("返回桌面", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					if (main_m.uMsg == WM_LBUTTONDOWN) { 
						exit(0);
						mark = 1;
					}
				}
			}
			settextcolor(RGB(255, 192, 203));
			if (!(main_m.y <= 255 && main_m.y >= 210)) drawtext("开始游戏", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (!(main_m.y <= 270 && main_m.y >= 245)) drawtext("排行榜", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (!(main_m.y >= 280 && main_m.y <= 305)) drawtext("返回桌面", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}
//开始游戏，返回桌面,分数排行榜,游戏说明

void showscores()
{
	int num= 0,i,j;
	TCHAR  scoreinfo[50];
	scinfo score[NUM];//定义结构体数组记录文件中的分数
	scinfo tmpscore;   //临时
	RECT r4 = { 340,65,440,90 }, r5 = { 350,120,450,145 }, r6 = {40,40,140,65};
	cleardevice();
	drawt();;
	settextcolor(RGB(30, 144, 255));
	drawtext("回车键返回",& r6, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("分数排行", &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	fopen_s(&fp, FilePath, "r");
	getc(fp);
	if (feof(fp)) {
		drawtext("暂无数据", &r5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else {
		rewind(fp);
		while (!feof(fp)) {
			fread(&score[num], sizeof(struct scoreinfo), 1, fp);
			num++;
		}
		fclose(fp);
		for (i = 0; i < num; i++)  //冒泡排序分数
			for (j = 0; j < num - 1 - i; j++) {
				if (score[j].score < score[j + 1].score)
				{
					tmpscore = score[j];
					score[j] = score[j + 1];
					score[j + 1] = tmpscore;
				}
			}
		//显示分数
		if (num > 10) {//显示前十分数
			for (i = 0; i < 10; i++)	//只显示前10个分数
			{
				sprintf_s(scoreinfo, "%d         %d", i + 1, score[i].score);
				settextcolor(BLACK);
				settextstyle(25, 25, _T("宋体"));
				outtextxy(350, 90 + i * 20, scoreinfo);//输出分数的文本内容
			}
		}
		else {
			for (i = 0; i < num -1; i++)	//只显num个
			{
				sprintf_s(scoreinfo, "%d         %d", i + 1, score[i].score);
				settextcolor(BLACK);
				outtextxy(350, 90 + i * 20, scoreinfo);//输出分数的文本内容
			}
		}
	}
	getchar();
}



void gamerun()
{
	s = 0;
	isdead = 1;
	loadingimage();
	cleardevice();
	Drawmap();   
	createsnake();
	drawsnake();
	createfood();
	while (1) {
		while (!_kbhit()) {
			if (food.flag == 0) {
				createfood();
				putimage(food.crd.x, food.crd.y, &foodimage);
			}
			move();
			Sleep(speed);
			BeginBatchDraw();
			cleardevice();
			drawt();
			addscore();
			putimage(food.crd.x, food.crd.y, &foodimage);
			drawsnake(); 
			Sleep(150);
			EndBatchDraw();
			eat();
			GameEnd();
			if (isdead==0)  break;
		}
		if (isdead == 0) break;
		HeadDirection();
	}
}

void createfood() {

	srand((unsigned)time(NULL));  //设置一个随机种子，每次运行都能保证随机种子不同。
	food.crd.x = rand() % (800/N) * N;
	food.crd.y = rand() % (600/N) * N;
	food.flag = 1;
	//putimage(food.crd.x, food.crd.y,&foodimage);
}


void createsnake() {
	snake.crd[0].x =40;
	snake.crd[0].y =40;
	snake.node = 1;
	snake.dir = down;
	food.flag = 0;
}

void drawsnake() {
	switch (snake.dir)//获取运动方向，改变蛇头图片
	{
	case up:
		putimage(snake.crd[0].x, snake.crd[0].y, &snakeup);
		break;
	case down:
		putimage(snake.crd[0].x, snake.crd[0].y, &snakedown);
		break;
	case left:
		putimage(snake.crd[0].x, snake.crd[0].y, &snakeleft);
		break;
	case right:
		putimage(snake.crd[0].x, snake.crd[0].y, &snakeright);
		break;
	default:
		break;
	}
		//蛇身
	//通过一个从（node-1）到1的循环，生成蛇身，进行贴图
		for (int i = snake.node - 1; i > 0; i--)
		{
			putimage(snake.crd[i].x, snake.crd[i].y, &snakebody);
		}
}

void loadingimage() 
{
	loadimage(&snakedown, "pic\\蛇头下.jpg", N, N);
	loadimage(&snakeup, "pic\\蛇头上.jpg", N, N);
	loadimage(&snakeright, "pic\\蛇头右.jpg", N, N);
	loadimage(&snakeleft, "pic\\蛇头左.jpg", N, N);
	loadimage(&foodimage, "pic\\食物.jpg", N, N);
	loadimage(&snakebody, "pic\\贪吃蛇身体.jpg", N, N);
}


void move() {
	//倒数第一节蛇身的贴图坐标等于倒数第二节
	for (int i = snake.node - 1; i > 0; i--)
	{
		snake.crd[i].x = snake.crd[i - 1].x;
		snake.crd[i].y = snake.crd[i - 1].y;
	}
	switch (snake.dir)
	{
	case up:
		snake.crd[0].y -= N;
		break;
	case down:
		snake.crd[0].y += N;
		break;
	case left:
		snake.crd[0].x -= N;
		break;
	case right:
		snake.crd[0].x += N;
		break;
	default:
		break;
	}
}


void HeadDirection()
{
	switch (_getch())
	{
	case 72:
		if (snake.dir != down)
			snake.dir = up;
		break;
	case 80:
		if (snake.dir != up)
			snake.dir = down;
		break;
	case 75:
		if (snake.dir != right)
			snake.dir = left;
		break;
	case 77:
		if (snake.dir != left)
			snake.dir = right;
		break;
	case 32://空格暂停
		system("pause");
	default:
		break;
	}
}


void addscore()
{
	TCHAR myscore[20];
	sprintf_s(myscore, _T("score:%d"), s);// 输出数值，先将数字格式化输出为字符串
	settextcolor(BLACK);
	outtextxy(0, 0, myscore);//输出分数的文本内容
}

void drawt() {
	int i = 0;
	setcolor(RGB(192, 192, 192));
	for (i = 0; i < WIDTH / 20; i++)
	{
		line(i * 20, 0, i * 20, 600);
		if (i < 30)
			line(0, i * 20, 800, i * 20);
	}
}

void savescore() {
	scinfo tmp;
	tmp.score = s;
	//if((fopen_s(&fp,FilePath,"a")!=0))  fopen_s(&fp,FilePath,"w");
	fopen_s(&fp, FilePath, "a");
	fwrite(&tmp, sizeof(struct scoreinfo), 1, fp);
	fclose(fp);

}


void eat() {
	if (snake.crd[0].x == food.crd.x && snake.crd[0].y == food.crd.y)//食物的坐标和蛇头坐标(snake.crd.[0])的重合
	{
		snake.node++;	//节数增加
		food.flag = 0;	//生成食物的标志变为0
		s += 10;
		speed--;
	}
}

void GameEnd()
{
	//撞墙
	if (snake.crd[0].y < 0 || snake.crd[0].y>600 || snake.crd[0].x < 0 || snake.crd[0].x>800)
	{
		//退出
		isdead = 0;
	}
	//咬到自己
	for (int i = snake.node - 1; i > 0; i--)
	{
		if (snake.crd[0].x == snake.crd[i].x && snake.crd[0].y == snake.crd[i].y)
		{
			//退出
			isdead = 0;
		}
	}
}



void gameover() {
	char myscore[20];
	savescore();
	RECT rect1 = { 360,300,480,325 };
	settextcolor(RGB(178, 34, 34));
	drawtext("回车返回主菜单", &rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	sprintf_s(myscore, "yourscore:  %d", s);
	outtextxy(380, 240, "game over");
	outtextxy(370, 270, myscore);
	getchar();
	
}


