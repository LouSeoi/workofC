#include <iostream>
#include <string.h>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>			//包含多媒体函数库
using namespace std;
#define WIDTH 37
#define LENGTH 37

//alpha^n对应的值
const short alpha[256]={1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1};
//alpha^n值对应的n
const short n_alpha[256]={ 0, 0, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75, 4, 100, 224, 14, 52, 141, 239, 129, 28, 193, 105, 248, 200, 8, 76, 113, 5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240, 18, 130, 69, 29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166, 6, 191, 139, 98, 102, 221, 48, 253, 226, 152, 37, 179, 16, 145, 34, 136, 54, 208, 148, 206, 143, 150, 219, 189, 241, 210, 19, 92, 131, 56, 70, 64, 30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250, 133, 186, 61, 202, 94, 155, 159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87, 7, 112, 192, 247, 140, 128, 99, 13, 103, 74, 222, 237, 49, 197, 254, 24, 227, 165, 153, 119, 38, 184, 180, 124, 17, 68, 146, 217, 35, 32, 137, 46, 55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97, 242, 86, 211, 171, 20, 42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162, 31, 45, 67, 216, 183, 123, 164, 118, 196, 23, 73, 236, 127, 12, 111, 246, 108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134, 177, 187, 204, 62, 90, 203, 89, 95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215, 79, 174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80, 88, 175};
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
bool code[864] = {0};
char ori_code[109] = {'\0'};
bool eccode[26*8] = {0};
bool qrcode[WIDTH][LENGTH] = {0};
bool unchange[WIDTH][LENGTH] = {0};
bool finalcode[1072] = {0};
void error_correction()
{
    int j = 0;
	int temp[108 + 26] = {0};
	for(int i=0;i < 108;i++)
	{
		temp[i] = code[i*8] * 128 + code[i*8 + 1] * 64 + code[i*8 + 2] * 32 + code[i*8 + 3] * 16 + code[i*8 + 4] * 8 + code[i*8 + 5] * 4 + code[i*8 + 6] *2 + code[i*8 + 7];
	}

	//纠错生成码对应alpha的指数
	int gen26_n[26]={173,125,158,2,103,182,118,17,145,201,111,28,165,53,161,21,245,142,13,102,48,227,153,145,218,70};
	for(int i=0;i<108;i++)
	{
		for(int j=0;j<26;j++)//进行乘和XOR操作
		{
			temp[i+j+1]=temp[i+j+1]^alpha[(gen26_n[j]+n_alpha[temp[i]])%255];
		}
		temp[i]=0;
	}
	for(int i=0;i<26;i++)
	{
		for(j = 0;j<8;j++)
        {
            eccode[(i+1)*8 - j -1] = (temp[108 + i]>>j)%2;
        }
	}
}
void gotoxy(HANDLE hout, const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(hout,coord);
}
void getfinalcode()
{
    int i = 0;
    for(i = 0;i<864;i++)
    {
        finalcode[i] = code[i];
    }
    for(i = 0;i<208;i++)
    {
        finalcode[864+i] = eccode[i];
    }
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

void showch(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color, const char ch, const int n)
{
    int i;
	gotoxy(hout, X, Y);
	setcolor(hout, bg_color, fg_color);

	/* 循环n次，打印字符ch */
	for (i=0; i<n; i++)
	    putchar(ch);
    setcolor(hout, 15, 0);

}
void pedding()
{
    int i = 0;
    int count = 0;
    int temp = 0;
    int j = 0;
    int length = strlen(ori_code);
    for(i = length + 2; i<108; i++)
    {
        if(count % 2 == 0)
        {
            temp = 236;
                for(j = 0; j<8; j++)
                {
                    code[(i+1)*8 - j -1] = (temp>>j)%2;
                }
        }
        else
        {
            temp = 17;
                for(j = 0; j<8; j++)
                {
                    code[(i+1)*8 - j -1] = (temp>>j)%2;
                }
        }
        count++;
    }
}
void oritobyte()
{
    int i = 0;
    int j = 0;
    code[0] = 0;
    code[1] = 1;
    code[2] = 0;
    code[3] = 0;
    for(i = 0;ori_code[i] != '\0';i++)
    {
        if(i == 0)
        {
            for(j = 0; j<8; j++)
            {
                code[4 + (i+1)*8 - j -1] = (strlen(ori_code)>>j)%2;
            }
            for(j = 0; j<8; j++)
            {
                code[4 + (i+2)*8 - j -1] = (ori_code[i]>>j)%2;
            }
        }
        else
        {
            for(j = 0; j<8; j++)
            {
                code[4 + (i+2)*8 - j -1] = (ori_code[i]>>j)%2;
            }
        }
    }
    code[4+8*i+8] = 0;
    code[4+8*i+1+8] = 0;
    code[4+8*i+2+8] = 0;
    code[4+8*i+3+8] = 0;
}
void location_one(int x,int y)
{
    int i = 0;
    int j = 0;
    for(i = x;i< x+7;i++)
    {
        qrcode[y][i] = 1;
    }
    qrcode[y+1][x] = 1;
    qrcode[y+1][x + 6] = 1;
    for(i = y+2;i<y+5;i++)
    {
        qrcode[i][x+0] = 1;
        qrcode[i][x+2] = 1;
        qrcode[i][x+3] = 1;
        qrcode[i][x+4] = 1;
        qrcode[i][x+6] = 1;
    }
    qrcode[y+5][x] = 1;
    qrcode[y+5][x + 6] = 1;
    for(i = x ;i<x +7;i++)
    {
        qrcode[y+6][i] = 1;
    }
    for(i = 0;i<7;i++)
    {
        for(j = 0;j<7;j++)
        {
            unchange[i+y][j+x] = 1;
        }
    }
}
void location()
{
    location_one(0,0);
    location_one(LENGTH-7,0);
    location_one(0,LENGTH-7);
    int i = 0;
    for(i = 0;i<8;i++)
    {
        unchange[7][i] = 1;
        unchange[7][i+29] = 1;
        unchange[i][7] = 1;
        unchange[i+29][7] = 1;
        unchange[29][i] = 1;
        unchange[i][29] = 1;
    }
}
void Alignment(int x,int y)
{
    int i = 0;
    qrcode[y][x] = 1;
    qrcode[y][x-2] = 1;
    qrcode[y][x+2] = 1;
    qrcode[y+1][x-2] = 1;
    qrcode[y-1][x+2] = 1;
    qrcode[y-1][x-2] = 1;
    qrcode[y+1][x+2] = 1;
    for(i = 0;i<5;i++)
    {
        qrcode[y-2][x+i-2] = 1;
        qrcode[y+2][x+i-2] = 1;
    }
    int j  = 0 ;
    for(i = 0;i<5;i++)
    {
        for(j = 0;j<5;j++)
        {
            unchange[y+i-2][x+j-2] = 1;
        }
    }

}
void Timing()
{
    int i = 0;
    for(i = 8;i<37-8;i++)
    {
        if(i % 2 == 0)
        {
            qrcode[6][i] = 1;
            qrcode[i][6] = 1;
        }
        unchange[i][6] = 1;
        unchange[6][i] = 1;
    }
}
bool formatcode[15] = {1,1,1,1,0,0,0,1,0,0,1,1,0,1,0};
//bool formatcode[15] = {0,1,0,1,1,0,0,1,0,0,0,1,1,1,1};
void format()
{
    formatcode[14] ^= 1;
    formatcode[13] ^= 0;
    formatcode[12] ^= 1;
    formatcode[11] ^= 0;
    formatcode[10] ^= 1;
    formatcode[9] ^= 0;
    formatcode[8] ^= 0;
    formatcode[7] ^= 0;
    formatcode[6] ^= 0;
    formatcode[5] ^= 0;
    formatcode[4] ^= 1;
    formatcode[3] ^= 0;
    formatcode[2] ^= 0;
    formatcode[1] ^= 1;
    formatcode[0] ^= 0;
    int i = 0;
    int j = 0;
    int count = 0;
    int count2 = 0;
    int count3 = 7;
    qrcode[29][8] = 1;
    unchange[29][8] = 1;
    for(i = 0;i<9;i++)
    {
        if(qrcode[i][8] == 0)
        {
            qrcode[i][8] = formatcode[count];
            qrcode[8][i] = formatcode[14-count];
            unchange[i][8] = 1;
            unchange[8][i] = 1;
            count++;
        }
        if(i<8)
        {
            if(qrcode[29+i][8] == 0)
            {
                qrcode[29+i][8] = formatcode[8+count2];
                unchange[29+i][8] = 1;
                count2++;
            }
            qrcode[8][29+i] = formatcode[count3];
            unchange[8][29+i] = 1;
            count3--;
        }

    }
}
void putincode()
{
    int i = LENGTH-1;
    int j = WIDTH -1;
    int count = 0;
    bool type = 0;// 0 上升 1下降
    for(;j>=0;j-=2)
    {
        if(j==6)
        {
            j+=1;
            continue;
        }
        if(type==0)
        {
            for(i=36;i>=0;i--)
            {
                if(unchange[i][j]==0)
                {
                    qrcode[i][j] = finalcode[count];
                    count++;
                }
                if(unchange[i][j-1]==0)
                {
                    qrcode[i][j-1] = finalcode[count];
                    count++;
                }
            }
        }
        if(type==1)
        {
            for(i=0;i<37;i++)
            {
                if(unchange[i][j]==0)
                {
                    qrcode[i][j] = finalcode[count];
                    count++;
                    if(count==1072)
                    {
                        break;
                    }
                }
                if(unchange[i][j-1]==0)
                {
                    qrcode[i][j-1] = finalcode[count];
                    count++;
                    if(count==1072)
                    {
                        break;
                    }
                }
            }
        }
        type = !type;
    }
    qrcode[28][0] = 1;
    qrcode[27][1] = 1;
    qrcode[26][0] = 1;
}
bool maskingcode[37][37] = {0};
int masking()
{
    int i = 0;
    int j = 0;
    int count = 0;
    for(i = 0; i<37; i++)
    {
        for(j= 0; j<37; j++)
        {
            if((i+j)%3==0)
            {
                if(unchange[i][j] == 0)
                {
                    qrcode[i][j] ^= 1;
                    maskingcode[i][j] = 1;

                }
            }
            if(unchange[i][j] == 0)
                {
                    count++;

                }
        }
    }
    return count;
}
void display()
{
    system("cls");
    system("mode con cols=90 lines=50");
    system("color f0");
    int i = 0;
    int j = 0;
    for (i = 0;i<37;i++)
    {
        for(j = 0;j<37;j++)
        {
            if(qrcode[i][j] == 1)
            {
                showch(hout,5+2*j,3+i,0,0,' ',2);
            }
        }
    }
}
void displaycode(bool (*p)[37])
{
    system("cls");
    system("mode con cols=90 lines=50");
    system("color f0");
    int i = 0;
    int j = 0;
    for (i = 0;i<37;i++)
    {
        for(j = 0;j<37;j++)
        {
            if(p[i][j] == 1)
            {
                showch(hout,5+2*j,3+i,15,0,p[i][j]+'0',1);
                cout << ' ';
            }
        }
    }
}
void displayunchange()
{
    system("cls");
    system("mode con cols=90 lines=50");
    system("color f0");
    int i = 0;
    int j = 0;
    for (i = 0;i<37;i++)
    {
        for(j = 0;j<37;j++)
        {
            if(unchange[i][j] == 1)
            {
                showch(hout,5+2*j,3+i,0,0,' ',2);
            }
        }
    }
}
void displaymasking()
{
    system("cls");
    system("mode con cols=90 lines=50");
    system("color f0");
    int i = 0;
    int j = 0;
    for (i = 0;i<37;i++)
    {
        for(j = 0;j<37;j++)
        {
            if(maskingcode[i][j] == 1)
            {
                showch(hout,5+2*j,3+i,0,0,' ',2);
            }
        }
    }
}
int main()
{
    cout << "请输入要转为二维码的串：" ;
    cin >> ori_code;
    pedding();
    oritobyte();
    error_correction();
    getfinalcode();
    int i = 0;
    for(i = 0;i<108*8;i++)
    {
        if(i % 8 == 0)
            cout << endl;
        cout << code[i] << ' ';
    }
    cout <<endl;
    cout << "纠错码段为:";
    for(i = 0;i<26*8;i++)
    {
        if(i % 8 == 0)
            cout << endl;
        cout << eccode[i] << ' ';
    }
    getch();
    Alignment(30,30);
    location();
    Timing();
    format();
    putincode();
    masking();
    //displaycode(maskingcode);
    display();
    getch();
    return 0;
}
