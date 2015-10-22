// 1452214 高昕宇 计算机一班
#include <iostream>
#include <fstream>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <dir.h>
#include <dirent.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <mmsystem.h>			//包含多媒体函数库
using namespace std;






int board[9][9] = {0};
bool boarderror[9][9] = {0};//0表示存在没有问题，1表示出错了。
bool firstboard[9][9] = {0};
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);


void print(int ,int);
struct step
{
    int loc[2];
    int value[2]; // 0是输入的值，1是以前的值.
    step *preview;
};


void gotoxy(HANDLE hout, const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(hout,coord);
}
int read_dir(char *dirname,char name[][20])
{
    int count = 0;
    DIR           *dir;
    struct dirent *node;
    struct stat    st;
    string full_filename;


    /* 打开目录失败则直接返回 */
	if ((dir = opendir(dirname))==NULL)
	    return -1;

    /* 循环扫描该目录下的所有文件 */
    while((node=readdir(dir))!=NULL) {
	    memset(&st, 0, sizeof(struct stat));

        full_filename  = dirname;
        full_filename += "\\";
        full_filename += node->d_name;

	    stat(full_filename.c_str(), &st);

	    /* 忽略所有目录 */
	    if (S_ISDIR(st.st_mode)) {
	        continue;
            }

        strcpy(name[count],node->d_name);
        count++;//cout << "文件---" << node->d_name << endl;
        }

	closedir(dir);
	return count;
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

bool readfile()
{
    int i = 0;
    int j = 0;
    int cur_loc = 0;
    char name[20][20];
    int ch = 0;
    int num_name = read_dir(".",name);
    int maxlen = 0;
    for(i = 0;i<num_name;i++)
    {
        int temp_len = strlen(name[i]);
        if(maxlen<temp_len)
        {
            maxlen = temp_len;
        }
    }
    gotoxy(hout,90,1);
    cout<< "请选择数独题目文件名:";
    gotoxy(hout,90,2);
    cout << "╔";
    for(i = 0;i<maxlen/2+1;i++)
    {
        cout << "═";
    }
    cout << "╗";
    for(i = 0;i<num_name;i++)
    {
        gotoxy(hout,90,3+i);
        cout << "║";
        cout << setiosflags(ios::left)<<setw(maxlen+2) << name[i];
        cout << "║";
    }
    gotoxy(hout,90,3+num_name);
    cout << "╚";
    for(i = 0;i<maxlen/2+1;i++)
    {
        cout << "═";
    }
    cout << "╝";
    while(1)
    {
        setcolor(hout,10,0);
        gotoxy(hout,92,3+cur_loc);
        cout << setiosflags(ios::left)<<setw(maxlen+2) << name[cur_loc];
        setcolor(hout,15,0);
        ch = getch();
        if(ch==224)
        {
            ch = getch();
            switch(ch)
                {
                case 72:
                    gotoxy(hout,92,3+cur_loc);
                    cout << setiosflags(ios::left)<<setw(maxlen+2) << name[cur_loc];
                    if(cur_loc==0)
                        cur_loc = num_name;
                    cur_loc  = (cur_loc-1)%num_name;
                    break;
                case 80:
                    gotoxy(hout,92,3+cur_loc);
                    cout << setiosflags(ios::left)<<setw(maxlen+2) << name[cur_loc];
                    cur_loc  = (cur_loc+1)%num_name;
                    break;
                }
        }
        else if(ch == 13)
        {
            break;
        }
    }
    ifstream in;
    in.open(name[cur_loc],ios::in);
    if(in ==NULL)
    {
        gotoxy(hout,90,4+num_name);
        cout << "文件打开失败。"<<endl;
        return 1;
    }
    else
    {
        for(i = 0; i<9; i++)
        {
            for(j = 0; j<9; j++)
            {
                in >> board[i][j];
                if(board[i][j]!=0)
                {
                    firstboard[i][j] = 1;
                }
            }
        }
        in.close();
    }
    return 0;
}
void firstprintboard()
{
    int i = 0;
    int j = 0;
    gotoxy(hout,5,1);
    for(i = 0;i<9;i++)
    {
        cout << "   "<<char('a' + i) <<"    ";
    }
    for(i = 0;i<9;i++)
    {
        gotoxy(hout,3,3+3*i);
        cout << i+1;
        for(j = 0;j<9;j++)
        {
            print(i,j);
        }
    }
}
void print(int locx , int locy)
{
    if(boarderror[locx][locy] == 1)
    {
        if(firstboard[locx][locy]==1)
        {
            setcolor(hout,2,11);
        }
        else if(board[locx][locy]!=0)
        {
            setcolor(hout,2,9);
        }
        else
        {
            setcolor(hout,2,13);
        }
    }
    else
    {
        if(firstboard[locx][locy]==1)
        {
            setcolor(hout,15,11);
        }
        else if(board[locx][locy]!=0)
        {
            setcolor(hout,15,9);
        }
        else
        {
            setcolor(hout,15,13);
        }
    }
    gotoxy(hout,5+locy*8,2+locx*3);
    cout << "╔═╗";
    gotoxy(hout,5+locy*8,2+locx*3+1);
    cout << "║"<<board[locx][locy]<<" ║";
    gotoxy(hout,5+locy*8,2+locx*3+2);
    cout << "╚═╝";
    setcolor(hout,15,0);
}
int finderror(int locx,int locy)
{
    int j = 0;
    bool need  = 0;
    int error = 0;
    bool num[9] = {0};
    for(j = 0,need = 0; j<9; j++)
    {
        if(board[locx][j]>0)
        {
            if(num[board[locx][j]-1]==1)
            {
                need  = 1;
                error ++;
                break;
            }
            else
            {
                num[board[locx][j]-1] = 1;
            }
        }
    }
    if(need == 1)
    {
        for(j = 0; j<9; j++)
        {
            boarderror[locx][j] = 1;
        }
    }
    for(j = 0;j<9;j++)
        num[j] = 0;
    for(j = 0,need = 0; j<9; j++)
    {
        if(board[j][locy]>0)
        {
            if(num[board[j][locy]-1]==1)
            {
                need  = 1;
                error ++;
                break;
            }
            else
            {
                num[board[j][locy]-1] = 1;
            }
        }
    }
    if(need == 1)
    {
        for(j = 0; j<9; j++)
        {
            boarderror[j][locy] = 1;
        }
    }
    return error;
}
int finderrornb(int locx,int locy)
{
    bool num[9] = {0};
    int i  = 0;
    int j  = 0;
    int error = 0;
    bool need = 0;
    for(i = 0; i<3; i++)
    {
        for (j = 0; j<3; j++)
        {
            if(board[locx+i][locy+j]>0)
            {
                if(num[board[locx+i][locy+j]-1]==1)
                {
                    need  = 1;
                    error ++;
                    break;
                }
                else
                {
                    num[board[locx+i][locy+j]-1]=1;
                }
            }
        }
    }
    if(need == 1)
    {
        for(i = 0; i<3; i++)
        {
            for (j = 0; j<3; j++)
            {
                boarderror[locx+i][locy+j] = 1;
            }
        }
    }
    return error;
}
int findallerror()
{
    int i = 0;
    int j = 0;
    int error = 0;
    for(i = 0;i<9;i++)
    {
        for(j = 0;j<9;j++)
        {
            boarderror[i][j] = 0;
        }
    }
    for(i = 0; i<9; i++)
    {
        error+= finderror(i,i);
    }
    for(i = 0; i<9; i+=3)
    {
        for(j = 0; j<9; j+=3)
        {
            error+=finderrornb(i,j);
        }
    }
    return error;
}
step *shuru(int locx ,int locy ,int value_b,step *last,int count)
{
    if( count == 0)
    {
        last->loc[0] = locx;
        last->loc[1] = locy;
        last->value[0] = value_b;
        last->value[1] = board[last->loc[0]][last->loc[1]];
        board[last->loc[0]][last->loc[1]] = last->value[0];
        last->preview = NULL;
        return last;
    }
    step *newone = new step;
    newone->loc[0] = locx;
    newone->loc[1] = locy;
    newone->value[0] = value_b;
    newone->value[1] = board[newone->loc[0]][newone->loc[1]];
    board[newone->loc[0]][newone->loc[1]] = newone->value[0];
    newone->preview = last;
    return newone;
}
bool gameover()
{
    int i = 0;
    int j = 0;
    for(i = 0;i<9;i++)
    {
        for(j = 0;j<9;j++)
        {
            if(board[i][j] == 0)
            {
                return 0;
            }
        }
    }
    return 1;
}
bool backkey(step *tail)
{
    step *p = tail;
    if(tail->preview == NULL)
    {
        return 0;
    }
    board[tail->loc[0]][tail->loc[1]] = tail->value[1];
    tail = tail->preview;
    delete p;
    return 1;
}
bool findnextboard(int &locx,int &locy)
{
    for(;locx<9;locx++)
    {
        for(;locy<9;locy++)
        {
            if(board[locx][locy] == 0 && firstboard[locx][locy] == 0)
                return 1;
        }
        locy %= 9;
    }
    return 0;
}
bool search_able(int locx,int locy,step *tail)
{
    static int count = 0;
    int i = 0;
    int next_locx = locx;
    int next_locy = locy;
    bool last = findnextboard(next_locx,next_locy);
    gotoxy(hout,90,25);
    cout << "正在运算第"<<setw(2)<<count+1<< "格";
    for(i = 0;i<9;i++)
    {
        tail = shuru(locx,locy,i+1,tail,count);
        count++;
        if(!findallerror())
        {
            if(!last)
            {
                return 1;
            }
            else if(search_able(next_locx,next_locy,tail))
            {
                return 1;
            }
            else
            {
                count--;
                backkey(tail);
            }
        }
        else
        {
            count--;
            backkey(tail);
        }
    }
    return 0;
}
int main()
{
    system("color F0");
    system("mode con cols=120 lines=40");
    firstprintboard();
    if(readfile())
        return -1;
    firstprintboard();
    int num_error = findallerror();
    step *tail,*p;
    tail = new step;
    if(num_error)
    {
        gotoxy(hout,80,20);
        cout << "共发现" << num_error <<"个错误。";
        firstprintboard();
        gotoxy(hout,80,21);
        cout << "源文件有问题，请检查后重新来一遍。";
        gotoxy(hout,0,38);
        system("PAUSE");
        return -1;
    }
    int first_locx = 0;
    int first_locy = 0;
    findnextboard(first_locx,first_locy);
    if(search_able(first_locx,first_locy,tail))
    {
        gotoxy(hout,0,37);
        cout << "游戏成功。"<<endl;
    }
    else
    {
        gotoxy(hout,0,37);
        cout << "游戏失败。"<<endl;
    }
    firstprintboard();
    for(;tail!=NULL;)
    {
        p = tail->preview;
        delete tail;
        tail = p;
    }
    gotoxy(hout,0,38);
    system("PAUSE");
    return 0;
}
