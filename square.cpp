// 1452214 高昕宇 计算机一班
#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>			//包含多媒体函数库
using namespace std;
#define UP 328
#define DOWN 336
#define LEFT 331
#define RIGHT 333

HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE); //取标准输出设备对应的句柄
bool board[23][13] = {0};
int cur_type = 0;
int cur_change = 0;
int next = 0;
int cur_block[4][2] = {0};
int temp_block[4][2] = {0};
bool gameend = 1;
int score = 0;
int level = 1;
int type[7][4][4][2] = {
    {
        {{0,0},{0,1},{0,2},{0,3}},
        {{0,0},{-1,0},{-2,0},{-3,0}},
        {{0,0},{0,1},{0,2},{0,3}},
        {{0,0},{-1,0},{-2,0},{-3,0}},
    },
    {
        {{0,0},{0,1},{-1,1},{-1,2}},
        {{0,0},{-1,0},{-1,-1},{-2,-1}},
        {{0,0},{0,1},{-1,1},{-1,2}},
        {{0,0},{-1,0},{-1,-1},{-2,-1}},
    },
    {
        {{0,0},{0,1},{1,1},{1,2}},
        {{0,0},{-1,0},{-1,1},{-2,1}},
        {{0,0},{0,1},{1,1},{1,2}},
        {{0,0},{-1,0},{-1,1},{-2,1}},
    },
    {
        {{0,0},{0,1},{0,2},{1,2}},
        {{0,0},{-1,0},{-2,0},{-2,1}},
        {{0,0},{0,-1},{0,-2},{-1,-2}},
        {{0,0},{1,0},{2,0},{2,-1}},
    },
    {
        {{0,0},{0,1},{0,2},{-1,2}},
        {{0,0},{-1,0},{-2,0},{-2,-1}},
        {{0,0},{0,-1},{0,-2},{1,-2}},
        {{0,0},{1,0},{2,0},{2,1}},
    },
    {
        {{0,0},{1,1},{0,1},{1,0}},
        {{0,0},{1,1},{0,1},{1,0}},
        {{0,0},{1,1},{0,1},{1,0}},
        {{0,0},{1,1},{0,1},{1,0}},
    },
    {
        {{0,0},{0,1},{-1,1},{1,1}},
        {{0,0},{-1,0},{-1,1},{-1,-1}},
        {{0,0},{0,-1},{-1,-1},{1,-1}},
        {{0,0},{1,0},{1,1},{1,-1}},
    }

};
void gotoxy(HANDLE hout, const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(hout,coord);
}
/* 设置指定的颜色

   颜色的取值由背景色+前景色组成，各16种
	   fg_color：0-7    黑   蓝   绿   青   红   粉   黄   白
	             8-15 亮黑 亮蓝 亮绿 亮青 亮红 亮粉 亮黄 亮白
	   bg_color：0-7    黑   蓝   绿   青   红   粉   黄   白
		         8-15 亮黑 亮蓝 亮绿 亮青 亮红 亮粉 亮黄 亮白
   最终的颜色为 背景色*16+前景色 */
void setcolor(HANDLE hout, const int bg_color, const int fg_color)
{
	SetConsoleTextAttribute(hout, bg_color*16+fg_color);
}
void printboard()
{
    setcolor(hout,0,12);
    gotoxy(hout,18,0);
    cout <<"制作者：高昕宇";
    setcolor(hout,0,14);
    gotoxy(hout,10,2);
    int i = 0;
    cout << "◎─────────────◎";
    for(i = 1;i<24;i++)
    {
        gotoxy(hout,10,2+i);
        cout << "│";
        gotoxy(hout,38,2+i);
        cout << "│";
    }
    gotoxy(hout,10,2+i);
    cout << "◎─────────────◎";
    setcolor(hout,0,13);
    gotoxy(hout,1,10);
    cout << "SCORE:";
    gotoxy(hout,43,10);
    cout << "NEXT:";
    gotoxy(hout,1,15);
    cout << "LEVEL:";
    setcolor(hout,0,7);
}
void display_one(int x,int y )
{
    setcolor(hout,0,11);
    gotoxy(hout,12 + 2*x,3+y);
    cout << "◎" ;
    setcolor(hout,0,7);
}
void clear_one(int x,int y )
{
    gotoxy(hout,12 + 2*x,3+y);
    cout << "  " ;
}
void clearall()
{
    int i = 0;
    int j = 0;
    for(i = 0;i<23;i++)
    {
        for(j = 0;j<13;j++)
        {
            if(board[i][j]==0)
            {
                clear_one(j,i);
            }
        }
    }
}
void displayall()
{
    int i = 0;
    int j = 0;
    clearall();
    for(i = 0;i<23;i++)
    {
        for(j = 0;j<13;j++)
        {
            if(board[i][j]!=0)
            {
                display_one(j,i);
            }
        }
    }
}
void refreshsl()
{
    setcolor(hout,0,13);
    gotoxy(hout,3,12);
    cout << score;
    gotoxy(hout,3,17);
    if(level > 10)
    {
        level = 10;
    }
    cout << level;
    setcolor(hout,0,7);
}
void shownext(int next_type)
{
    int i = 0;
    for(i = 0;i < 4 ;i++)
    {
        gotoxy(hout,40,12+i);
        cout << "        ";
    }
    for(i = 0;i<4;i++)
    {
        display_one(16 +  type[next_type][0][i][0] , 9 + type[next_type][0][i][1]);
    }
}
bool creatnewblock(int cur_type,int next_type)
{
    int i = 0;
    for(i = 0;i<4;i++)
    {
        if(board[0 + type[cur_type][0][i][1]][6 + type[cur_type][0][i][0]] != 1)
        {
            board[0 + type[cur_type][0][i][1]][6 + type[cur_type][0][i][0]] = 1;
            cur_block[i][0] = 6 + type[cur_type][0][i][0];
            cur_block[i][1] = type[cur_type][0][i][1];
            cur_change =  0;
        }
        else
            return 0;
    }
    shownext(next_type);
    return 1;
}
void swaptwo(int i ,int j)
{
    int i1 = 0;
    bool temp = 0;
    for(i1 = 0; i1 < 13;i1++)
    {
        temp = board[i][i1];
        board[i][i1] = board[j][i1];
        board[j][i1] = temp ;
    }
}
int xiaochu()
{
    int i = 0;
    int j = 0;
    int man[23] = {0};
    int count = 0;
    int temp_score = 0;
    int max_kong = 0;
    for (i = 0;i < 23;i++)
    {
        count = 0;
        for( j = 0;j < 13;j++)
        {
            if (board[i][j] == 0)
            {
                count ++ ;
            }
        }
        if(count == 13)
        {
            man[i] = 0;
            max_kong++;
        }
        else if(count == 0)
        {
            man[i] = 1;
            for(j = 0;j<13;j++)
            {
                board[i][j] = 0;
            }
            temp_score++;
        }
        else
        {
            man[i] = 2;
        }
    }
    for(i = 22;i>max_kong;i--)
    {
        for(j =  max_kong; j < i ; j++)
        {
            if(man[j] == 2 && man[j+1] == 1)
            {
                swaptwo(j,j+1);
                man[j+1] = 2;
                man[j] = 1;
            }
        }
    }
    for( i = max_kong ;i < 23;i++)
    {
        if(man[i] == 1)
        {
            for( j = 0;j<13;j++)
            {
                board[i][j] = 0;
            }
        }
    }
    return temp_score;
}
bool checkconflict()
{
    int i = 0;
    for(i = 0;i<4;i++)
    {
        if(board[temp_block[i][1]][temp_block[i][0]] == 1)
        {
            return 1;
        }
    }
    return 0;
}
bool keydown()
{
    int i = 0;
    int temp_score  = 0 ;
    for(i = 0;i<4;i++)
    {
        if(cur_block[i][1]==22)
        {
            cur_type = next ;
            next = rand()%7;
            temp_score = xiaochu();
            for(;temp_score>0;temp_score--)
            {
                score += temp_score;
            }
            gameend = creatnewblock(cur_type,next);
            level = 1 + score/10;
            refreshsl();
            return 1;
        }
    }
    for(i = 0;i<4;i++)
    {
        temp_block[i][0] = cur_block[i][0] ;
        temp_block[i][1] = cur_block[i][1] + 1;
        board[cur_block[i][1]][cur_block[i][0]] = 0;
    }
    if(checkconflict())
    {
        for(i = 0; i<4; i++)
        {
            board[cur_block[i][1]][cur_block[i][0]] = 1;
        }
        temp_score = xiaochu();
        for(; temp_score>0; temp_score--)
        {
            score += temp_score;
        }
        cur_type = next;
        next = rand()%7;
        level = 1 + score/10;
        refreshsl();
        gameend = creatnewblock(cur_type,next);
    }
    else
    {
        for(i = 0; i<4; i++)
        {
            cur_block[i][1] += 1;
            board[cur_block[i][1]][cur_block[i][0]] = 1;
        }
    }
    return 1;
}
bool keyright()
{
    int i = 0;
    for(i = 0;i<4;i++)
    {
        if(cur_block[i][0]==12)
        {
            return 1;
        }
    }
    for(i = 0;i<4;i++)
    {
        temp_block[i][0] = cur_block[i][0]  + 1;
        temp_block[i][1] = cur_block[i][1] ;
        board[cur_block[i][1]][cur_block[i][0]] = 0;
    }
    if(checkconflict())
    {
        for(i = 0; i<4; i++)
        {
            board[cur_block[i][1]][cur_block[i][0]] = 1;
        }
    }
    else
    {
        for(i = 0; i<4; i++)
        {
            cur_block[i][0] += 1;
            board[cur_block[i][1]][cur_block[i][0]] = 1;
        }
    }
    return 1;
}
bool keyleft()
{
    int i = 0;
    for(i = 0;i<4;i++)
    {
        if(cur_block[i][0]==0)
        {
            return 1;
        }
    }
    for(i = 0;i<4;i++)
    {
        temp_block[i][0] = cur_block[i][0]  - 1;
        temp_block[i][1] = cur_block[i][1] ;
        board[cur_block[i][1]][cur_block[i][0]] = 0;
    }
    if(checkconflict())
    {
        for(i = 0; i<4; i++)
        {
            board[cur_block[i][1]][cur_block[i][0]] = 1;
        }
    }
    else
    {
        for(i = 0; i<4; i++)
        {
            cur_block[i][0] -= 1;
            board[cur_block[i][1]][cur_block[i][0]] = 1;
        }
    }
    return 1;
}
bool keyup()
{
    int i = 0;
    int temp_tpye = (cur_change + 1) % 4;
    for(i = 0;i < 4;i++)
    {
        temp_block[i][0] = cur_block[0][0] + type[cur_type][temp_tpye][i][0] - type[cur_type][temp_tpye][0][0];
        temp_block[i][1] = cur_block[0][1] + type[cur_type][temp_tpye][i][1] - type[cur_type][temp_tpye][0][1];
        if(temp_block[i][0] < 0||temp_block[i][0]>12||temp_block[i][1]>22||temp_block[i][1] < 0)
        {
            return 0;
        }
    }
    for(i = 0;i<4;i++)
    {
        board[cur_block[i][1]][cur_block[i][0]] = 0;
    }
    if(checkconflict())
    {
        for(i = 0; i<4; i++)
        {
            board[cur_block[i][1]][cur_block[i][0]] = 1;
        }
    }
    else
    {
        for(i = 0; i<4; i++)
        {
            cur_block[i][0] = temp_block[i][0];
            cur_block[i][1] = temp_block[i][1];
            board[cur_block[i][1]][cur_block[i][0]] = 1;
        }
        cur_change = temp_tpye;
    }
    return 1;
}
int main()
{
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hout,&cci);
    cci.bVisible=false;
    SetConsoleCursorInfo(hout,&cci);
    system("mode con cols=50 lines=40");
    printboard();
    srand(time(NULL));
    cur_type =rand()%7;
    next = rand()%7;
    creatnewblock(cur_type,next);
    displayall();
    refreshsl();
    int begin,end;
    begin = clock();
    int temp_time = 0;
    while(gameend)
    {
        int ch = 0;
        if(kbhit())
        {
            ch = getch();
            if(ch == 224)
            {
                ch = getch();
                switch(ch)
                {
                    case 72:
                    keyup();
                    break;
                    case 80:
                    keydown();
                    displayall();
                    break;
                    case 75:
                    keyleft();
                    break;
                    case 77:
                    keyright();
                    break;
                }
            }
        }
        end = clock();
        temp_time = end - begin;
        if (temp_time > 1000 - level*100)
        {
            keydown();
            begin = clock();
        }
        Sleep(1);
        displayall();
    }
    gotoxy(hout,0,35);
    return 0;
}
