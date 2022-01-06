#pragma once
#ifndef _HEAD_H_ 
#define _HEAD_H_ 
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <iomanip>
#include <tchar.h>
using namespace std;
static const HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE); //取标准输出设备对应的句柄
static const HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);   //取标准输入设备对应的句柄

/* 定义光标的形态 */
#define CURSOR_VISIBLE_FULL					0	//光标显示，全高色块
#define CURSOR_VISIBLE_HALF					1	//光标显示，半高色块
#define CURSOR_VISIBLE_NORMAL				2	//光标显示，横线（缺省为此方式）
#define CURSOR_INVISIBLE					3	//光标不显示

class Editor {
private:
	COORD pos; //屏幕上的坐标1
	COORD loc; //屏幕上的坐标2   
	string line;//跟踪记录每一行的文本
	CONSOLE_SCREEN_BUFFER_INFO csbi;//控制台屏幕缓冲区信息                                                                                               x
	vector<string> text;//记录页面上的所有的文本
	struct _operator{
		string str;
		int x;
		int y;
	};                   
	stack<_operator> history;                                            //操作历史栈,记录页面历史

	int  generateBadChar(char* b, int m, int* badchar);            //BM搜索查找算法1
	int  generateGS(char* b, int m, int* suffix, bool* prefix);    //BM搜索查找算法2
	int moveByGS(int j, int m, int* suffix, bool* prefix);         //BM搜索查找算法3
	int str_bm(char* a, int n, char* b, int m);                    //BM搜索查找算法4
	int BM_Search(string str);                                       //BM搜索查找算法5
	string GetText(int i, int j);                                  //BM搜索查找算法6

	char Normal_GetInput();                                        //普通输入判断函数
	char Insert_GetInput();                                        //插入输入判断函数

	int OpenFile(string address);                                  //文件操作函数1
	int StoreFile(string address);                                 //文件操作函数2

	                                                               //其他操作
	int ShutDown();                                                //退出程序
	int Del(COORD pos);                                            //删除操作
	int Cancel();                                              //撤销操作
	int cct_setconsoleborder(int set_cols, int set_lines, int set_buffer_cols, int set_buffer_lines);//初始设置窗口操作
	int cct_cls(); 
	int Goto(const int X, const int Y);                       //定位函数1
	int GetPos(int& x, int& y);                               //定位函数2
	int setcursor(const int options);                         //设置光标状态
	int _insert(char ch);                                      //插入模式输入函数
public:
	Editor() {
		cct_setconsoleborder(80, 25, 80, 50);
		GetConsoleScreenBufferInfo(hOutput, &csbi);
		for (int i = 0; i < 50; i++) text.push_back(string(80, ' '));
	}

	int Normal_Pattern();                                                                                 //普通模式
	int Insert_Pattern();                                                                                 //插入模式
};
#endif
