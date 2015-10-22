// 1452214 ����� �����һ��
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
#include <mmsystem.h>			//������ý�庯����
using namespace std;






int board[9][9] = {0};
bool boarderror[9][9] = {0};//0��ʾ����û�����⣬1��ʾ�����ˡ�
bool firstboard[9][9] = {0};
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);


void print(int ,int);
struct step
{
    int loc[2];
    int value[2]; // 0�������ֵ��1����ǰ��ֵ.
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


    /* ��Ŀ¼ʧ����ֱ�ӷ��� */
	if ((dir = opendir(dirname))==NULL)
	    return -1;

    /* ѭ��ɨ���Ŀ¼�µ������ļ� */
    while((node=readdir(dir))!=NULL) {
	    memset(&st, 0, sizeof(struct stat));

        full_filename  = dirname;
        full_filename += "\\";
        full_filename += node->d_name;

	    stat(full_filename.c_str(), &st);

	    /* ��������Ŀ¼ */
	    if (S_ISDIR(st.st_mode)) {
	        continue;
            }

        strcpy(name[count],node->d_name);
        count++;//cout << "�ļ�---" << node->d_name << endl;
        }

	closedir(dir);
	return count;
}

/* ����ָ������ɫ

   ��ɫ��ȡֵ�ɱ���ɫ+ǰ��ɫ��ɣ���16��
	   fg_color��0-7    ��   ��   ��   ��   ��   ��   ��   ��
	             8-15 ���� ���� ���� ���� ���� ���� ���� ����
	   bg_color��0-7    ��   ��   ��   ��   ��   ��   ��   ��
		         8-15 ���� ���� ���� ���� ���� ���� ���� ����
   ���յ���ɫΪ ����ɫ*16+ǰ��ɫ */
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
    cout<< "��ѡ��������Ŀ�ļ���:";
    gotoxy(hout,90,2);
    cout << "�X";
    for(i = 0;i<maxlen/2+1;i++)
    {
        cout << "�T";
    }
    cout << "�[";
    for(i = 0;i<num_name;i++)
    {
        gotoxy(hout,90,3+i);
        cout << "�U";
        cout << setiosflags(ios::left)<<setw(maxlen+2) << name[i];
        cout << "�U";
    }
    gotoxy(hout,90,3+num_name);
    cout << "�^";
    for(i = 0;i<maxlen/2+1;i++)
    {
        cout << "�T";
    }
    cout << "�a";
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
        cout << "�ļ���ʧ�ܡ�"<<endl;
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
    cout << "�X�T�[";
    gotoxy(hout,5+locy*8,2+locx*3+1);
    cout << "�U"<<board[locx][locy]<<" �U";
    gotoxy(hout,5+locy*8,2+locx*3+2);
    cout << "�^�T�a";
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
    cout << "���������"<<setw(2)<<count+1<< "��";
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
        cout << "������" << num_error <<"������";
        firstprintboard();
        gotoxy(hout,80,21);
        cout << "Դ�ļ������⣬�����������һ�顣";
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
        cout << "��Ϸ�ɹ���"<<endl;
    }
    else
    {
        gotoxy(hout,0,37);
        cout << "��Ϸʧ�ܡ�"<<endl;
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
