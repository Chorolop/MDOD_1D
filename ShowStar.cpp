#include "graphics.h"
#include <time.h>
#include <string>
#include <conio.h>
#include <windows.h>
#include <cmath>
#include <iostream>
using namespace std;

int height[640] = { 0 };    // 此处储存生成的每个点的数值
double H = 0.3;             // H 即粗糙度常数 范围[0,1]
int point_number = 630;     // 横向采样数
int L = -100;               // 初始增益范围 [L,R]
int R = +300;

int main()
{
    srand((unsigned)time(nullptr));	// 更新随机种子（即Seed）
    initgraph(640, 480);			// 初始化图形环境

    IMAGE bg;
    loadimage(&bg, _T(".\\BG.png"));
    putimage(0, 0, &bg);

    height[0] = 100;                // 初始线段两端数值为100
    height[point_number - 1] = 100;
    double facts = pow(2, -H);      // 根据粗糙度常数计算缩减比例
    for (int i = 1; i < point_number - 1; i++) height[i] = -10;
    for (int num = 2; num < point_number;) {    // 当生成的随机数数量达到预计采样数时结束
        for (int i = 0, LL = -1, RR = -1; i < point_number; i++) {  // 遍历计算各点的数值
            if (height[i] < 0) continue;  // LL和RR为需要细分处理的线段端点 合格的线段中间必有空缺
            else if (LL == -1) {
                LL = i;
            }
            else if (LL < i - 1) {
                RR = i;
                if (R == L) R = 2;
                int rdnum = rand() % (R - L) + L; // 计算随机增益 计算各线段中点数值
                height[(LL + RR) / 2] = (height[LL] + height[RR]) / 2 + rdnum;
                if (height[(LL + RR) / 2] < 0) height[(LL + RR) / 2] = -height[(LL + RR) / 2]; // 为了显示效果对负数取反
                num++; 
                LL = -1; RR = -1;
            }
            else if (LL == i - 1) LL = i;
        }
        L = facts * L;  // 缩减随机范围
        R = facts * R;
    }

    int gap = 640 / (point_number-1);  // 图形生成模块
    moveto(0, 480 - 100);
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID);
    for (int i = 1; i < point_number-1; i++) {
        lineto(i * gap, 480 - height[i]);
        printf("#%d:(%d,%d)\n", i, i * gap, 480 - height[i]);
    }
    lineto(640, 480 - height[point_number - 1]);
    lineto(640, 480);
    lineto(0, 480);
    lineto(0, 480 - 100);

    floodfill(1, 479, BLACK);


    for (int i = 0; i < point_number; i++) {
        printf("%d ", height[i]);
    }

    while (!_kbhit());
    closegraph();					// 关闭绘图窗口
}

// By Lev 2020.6.22