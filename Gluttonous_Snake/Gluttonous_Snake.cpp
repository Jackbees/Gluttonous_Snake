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
#define SNAKELEN 1200  //�ߵ���󳤶�





void Drawmap();  //��ͼ
void displaymainmenu();
void gamerun();//������Ϸ
void writescore(); //д�����
void showscores();  //չʾ��������
void createfood();  //ʳ��
void loadingimage(); //����ͼƬ
void createsnake(); //��ʼ����
void drawsnake(); //����
void move();  //�ƶ�
void drawt();//������
void addscore();  //��ʾ����
void HeadDirection(); //�ñ䷽��
void savescore();  //�������
void eat(); //�ж��Ƿ�Ե�ʳ��
void gameover();  //��������
void GameEnd();  //�ж��Ƿ�����


MOUSEMSG main_m;  //�����Ϣ
char main_c='\0';//��¼����
FILE* fp; //�ļ�����
int s=100;//����
int speed = 80; //�����ٶ�
int isdead ;  //��־����


IMAGE snakedown, snakeup, snakeleft, snakeright, snakebody, foodimage;



enum DIRE                     //���Ƶķ���Ľṹ��
{
	up = 72,				//�ϡ��¡����ҡ��ո������Ӧ��ASCII��
	down = 80,
	left = 75,
	right = 77,
	space = 32
};



//�����ṹ��
typedef struct scoreinfo
{
	int score;
}scinfo;




//�ߵ�����
typedef struct posion			//��ʾ����Ľṹ��
{
	int x;
	int y;
}posi; 



struct snakeinfo 				//����ṹ�壬���洢�ߵ������Ϣ
{
	posi crd[SNAKELEN];		//�����ߵ�ÿ�����������꣬�����1000��
	int node;				//�����ߵ�ǰ����
	DIRE dir;				//��ǰ���н��ķ���
}snake;




struct FOOD					//ʳ��Ľṹ��
{
	posi crd;				//ʳ������
	int flag;				//��ʾ��û��ʳ�� 1����û��ʳ�û��ʳ����Ҫ����ʳ��
}food;





//������
int main()
{
	Drawmap();
	displaymainmenu();
	//savescore();
	getchar();
}











void Drawmap()//��ʼ��
{
	int i = 0;
	mciSendString("open C:\\Users\\50402\\Desktop\\̰�����ز�����\\1.mp3 alias bk", NULL, 0, NULL);
	mciSendString("play bk repeat", NULL, 0, NULL);
	initgraph(800, 600);
	setbkcolor(RGB(245, 245, 245));
	cleardevice();
	drawt();
}


void displaymainmenu()
{
	int mark = 0;//��־�ж��Ƿ��������ҳ��
	settextcolor(RGB(255, 192, 203));
	RECT r1 = { 360,210,440,235 }, r2 = { 350,245,450,270 }, r3 = { 350,280,450,305 };
	drawtext("��ʼ��Ϸ", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("���а�", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("��������", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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
				if (main_m.y <= 255 && main_m.y >= 210) {//��ʼ��Ϸ
					setcolor(RGB(160, 32, 240));
					drawtext("��ʼ��Ϸ", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					if (main_m.uMsg == WM_LBUTTONDOWN)    {
					gamerun();
					gameover();
					mark = 1;
				    }
				}
				if (main_m.y <= 270 && main_m.y >= 245) {//��������
					setcolor(RGB(160, 32, 240));
					drawtext("���а�", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					if (main_m.uMsg == WM_LBUTTONDOWN) {
						showscores();
						mark = 1;
					}
				}
				if (main_m.y >= 280 && main_m.y <= 305) {
					setcolor(RGB(160, 32, 240));
					drawtext("��������", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					if (main_m.uMsg == WM_LBUTTONDOWN) { 
						exit(0);
						mark = 1;
					}
				}
			}
			settextcolor(RGB(255, 192, 203));
			if (!(main_m.y <= 255 && main_m.y >= 210)) drawtext("��ʼ��Ϸ", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (!(main_m.y <= 270 && main_m.y >= 245)) drawtext("���а�", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (!(main_m.y >= 280 && main_m.y <= 305)) drawtext("��������", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}
//��ʼ��Ϸ����������,�������а�,��Ϸ˵��

void showscores()
{
	int num= 0,i,j;
	TCHAR  scoreinfo[50];
	scinfo score[NUM];//����ṹ�������¼�ļ��еķ���
	scinfo tmpscore;   //��ʱ
	RECT r4 = { 340,65,440,90 }, r5 = { 350,120,450,145 }, r6 = {40,40,140,65};
	cleardevice();
	drawt();;
	settextcolor(RGB(30, 144, 255));
	drawtext("�س�������",& r6, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("��������", &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	fopen_s(&fp, FilePath, "r");
	getc(fp);
	if (feof(fp)) {
		drawtext("��������", &r5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else {
		rewind(fp);
		while (!feof(fp)) {
			fread(&score[num], sizeof(struct scoreinfo), 1, fp);
			num++;
		}
		fclose(fp);
		for (i = 0; i < num; i++)  //ð���������
			for (j = 0; j < num - 1 - i; j++) {
				if (score[j].score < score[j + 1].score)
				{
					tmpscore = score[j];
					score[j] = score[j + 1];
					score[j + 1] = tmpscore;
				}
			}
		//��ʾ����
		if (num > 10) {//��ʾǰʮ����
			for (i = 0; i < 10; i++)	//ֻ��ʾǰ10������
			{
				sprintf_s(scoreinfo, "%d         %d", i + 1, score[i].score);
				settextcolor(BLACK);
				settextstyle(25, 25, _T("����"));
				outtextxy(350, 90 + i * 20, scoreinfo);//����������ı�����
			}
		}
		else {
			for (i = 0; i < num -1; i++)	//ֻ��num��
			{
				sprintf_s(scoreinfo, "%d         %d", i + 1, score[i].score);
				settextcolor(BLACK);
				outtextxy(350, 90 + i * 20, scoreinfo);//����������ı�����
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

	srand((unsigned)time(NULL));  //����һ��������ӣ�ÿ�����ж��ܱ�֤������Ӳ�ͬ��
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
	switch (snake.dir)//��ȡ�˶����򣬸ı���ͷͼƬ
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
		//����
	//ͨ��һ���ӣ�node-1����1��ѭ������������������ͼ
		for (int i = snake.node - 1; i > 0; i--)
		{
			putimage(snake.crd[i].x, snake.crd[i].y, &snakebody);
		}
}

void loadingimage() 
{
	loadimage(&snakedown, "pic\\��ͷ��.jpg", N, N);
	loadimage(&snakeup, "pic\\��ͷ��.jpg", N, N);
	loadimage(&snakeright, "pic\\��ͷ��.jpg", N, N);
	loadimage(&snakeleft, "pic\\��ͷ��.jpg", N, N);
	loadimage(&foodimage, "pic\\ʳ��.jpg", N, N);
	loadimage(&snakebody, "pic\\̰��������.jpg", N, N);
}


void move() {
	//������һ���������ͼ������ڵ����ڶ���
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
	case 32://�ո���ͣ
		system("pause");
	default:
		break;
	}
}


void addscore()
{
	TCHAR myscore[20];
	sprintf_s(myscore, _T("score:%d"), s);// �����ֵ���Ƚ����ָ�ʽ�����Ϊ�ַ���
	settextcolor(BLACK);
	outtextxy(0, 0, myscore);//����������ı�����
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
	if (snake.crd[0].x == food.crd.x && snake.crd[0].y == food.crd.y)//ʳ����������ͷ����(snake.crd.[0])���غ�
	{
		snake.node++;	//��������
		food.flag = 0;	//����ʳ��ı�־��Ϊ0
		s += 10;
		speed--;
	}
}

void GameEnd()
{
	//ײǽ
	if (snake.crd[0].y < 0 || snake.crd[0].y>600 || snake.crd[0].x < 0 || snake.crd[0].x>800)
	{
		//�˳�
		isdead = 0;
	}
	//ҧ���Լ�
	for (int i = snake.node - 1; i > 0; i--)
	{
		if (snake.crd[0].x == snake.crd[i].x && snake.crd[0].y == snake.crd[i].y)
		{
			//�˳�
			isdead = 0;
		}
	}
}



void gameover() {
	char myscore[20];
	savescore();
	RECT rect1 = { 360,300,480,325 };
	settextcolor(RGB(178, 34, 34));
	drawtext("�س��������˵�", &rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	sprintf_s(myscore, "yourscore:  %d", s);
	outtextxy(380, 240, "game over");
	outtextxy(370, 270, myscore);
	getchar();
	
}


