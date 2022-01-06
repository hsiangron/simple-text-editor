#include "head.h"

/***************************************************************************
  函数名称：_insert
  功    能：插入模式输入函数
***************************************************************************/
int Editor::_insert(char ch) {
	GetConsoleScreenBufferInfo(hOutput, &csbi);
	pos.X = csbi.dwCursorPosition.X;
	pos.Y = csbi.dwCursorPosition.Y;
	string str = GetText(pos.X, pos.Y);
	int i = pos.Y, j = pos.X;
	int k = 0;
	text[i][j] = ch;
	j++;
	for (; j < 80; j++) {
		text[i][j] = str[k];
		k++;
	}
	i++;
	for (; j < 80; j++)
		for (i = 0; i < 50; i++) {
			text[i][j] = str[k];
			k++;
		}

	history.push(_operator{ GetText(0,0),pos.X,pos.Y });
	Goto(pos.X, pos.Y);
	cout << ch;
	cout << str.substr(0, 3999 - pos.X - pos.Y * 80);
	Goto(pos.X, pos.Y);
	return 0;
}

/***************************************************************************
  函数名称：cct_setcursor
  功    能：设置光标状态（显示/不显示/全高/半高/横线等）
***************************************************************************/
int Editor::setcursor(const int options)
{
	CONSOLE_CURSOR_INFO cursor_info;
	BOOL CreateCaret(HWND hWnd, HBITMAP hBitmap, int nWidth, int nHeight);
	BOOL ShowCaret(HWND hWnd);

	switch (options) {
	case CURSOR_VISIBLE_FULL:
		cursor_info.bVisible = 1;
		cursor_info.dwSize = 100;
		break;
	case CURSOR_VISIBLE_HALF:
		cursor_info.bVisible = 1;
		cursor_info.dwSize = 50;
		break;
	case CURSOR_INVISIBLE:
		cursor_info.bVisible = 0;
		cursor_info.dwSize = 1;
		break;
	case CURSOR_VISIBLE_NORMAL:
	default: //缺省显示光标，横线
		cursor_info.bVisible = 1;
		cursor_info.dwSize = 25;
		break;
	}
	SetConsoleCursorInfo(hOutput, &cursor_info);
	return 0;
}

/***************************************************************************
  函数名称：Del
  功    能：删除
***************************************************************************/
int  Editor::Del(COORD pos) {
	int i, j, k;
	string str =GetText(pos.X+1, pos.Y);
	cout<<str;
	Goto(pos.X, pos.Y);
	k = 0; 
	i = pos.Y;
	for (j = pos.X; j < 80; j++, k++) text[i][j] = str[k];
	for (i++; i < 50; i++)
		for (j = 0; j < 80; j++, k++)
			text[i][j] = str[k];
	history.push({GetText(0,0),pos.X,pos.Y});
	return 0;
}

/***************************************************************************
  函数名称：Cancel
  功    能：撤销
***************************************************************************/
int Editor::Cancel() {
	GetConsoleScreenBufferInfo(hOutput, &csbi);
	if (!history.empty()) {
		Goto(0, 0);
		cout<< history.top().str;
		pos.X = history.top().x;
		pos.Y = history.top().y;
		Goto(history.top().x, history.top().y);
		history.pop();
	}
	return 0;
}

/***************************************************************************
  函数名称：cct_setconsoleborder
  功    能：设置窗口和缓冲区大小
***************************************************************************/
int Editor::cct_setconsoleborder(int set_cols, int set_lines, int set_buffer_cols, int set_buffer_lines)
{
	/* 取当前系统允许的窗口的行列最大值 */
	COORD max_coord;
	max_coord = GetLargestConsoleWindowSize(hOutput); /* .X 和 .Y 分别是窗口的列和行的最大值 */

	/* 处理设置窗口的行列的非法值 */
	if (set_cols <= 0 || set_lines <= 0)
		return 0;
	if (set_cols > max_coord.X)
		set_cols = max_coord.X;
	if (set_lines > max_coord.Y)
		set_lines = max_coord.Y;

	/* 设置窗口的行列大小（从0开始，0 ~ lines-1, 0 ~ cols-1）*/
	SMALL_RECT rect;
	rect.Top = 0;
	rect.Bottom = set_lines - 1;
	rect.Left = 0;
	rect.Right = set_cols - 1;

	/* 设置缓冲区的行列大小(缺省或小于窗口值则与窗口值一样) */
	COORD cr;
	cr.X = (set_buffer_cols == -1 || set_buffer_cols < set_cols) ? set_cols : set_buffer_cols;		//未给出或给出的值小于set_cols则用set_cols，未控制上限
	cr.Y = (set_buffer_lines == -1 || set_buffer_lines < set_lines) ? set_lines : set_buffer_lines;	//未给出或给出的值小于set_lines则用set_lines，未控制上限

	/* 取当前窗口及缓冲区的大小(就是getconsoleborder) */
	int cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines;
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(hOutput, &binfo);

	cur_cols = binfo.srWindow.Right - binfo.srWindow.Left + 1;	//可见窗口的列数
	cur_lines = binfo.srWindow.Bottom - binfo.srWindow.Top + 1;	//可见窗口的行数
	cur_buffer_cols = binfo.dwSize.X;							//缓冲区的列数
	cur_buffer_lines = binfo.dwSize.Y;							//缓冲区的行数

	cct_cls();
	/* 设置顺序(保证设置窗口大小时，现缓冲区的列值>窗口值) */
	if (cr.X <= cur_buffer_cols) {
		if (cr.Y <= cur_buffer_lines) {
			SetConsoleWindowInfo(hOutput, true, &rect);//设置窗口
			SetConsoleScreenBufferSize(hOutput, cr);//设置缓冲区
		}
		else { //cr.Y > cur_buffer_lines，先要让缓冲区的行数变大
			COORD tmpcr;
			tmpcr.X = cur_buffer_cols;
			tmpcr.Y = cr.Y;
			SetConsoleScreenBufferSize(hOutput, tmpcr);//设置缓冲区

			SetConsoleWindowInfo(hOutput, true, &rect);//设置窗口
			SetConsoleScreenBufferSize(hOutput, cr);//设置缓冲区
		}
	}
	else {//cr.X > cur_buffer_cols
		if (cr.Y >= cur_buffer_lines) {
			SetConsoleScreenBufferSize(hOutput, cr);//设置缓冲区
			SetConsoleWindowInfo(hOutput, true, &rect);//设置窗口
		}
		else {	//cr.Y < cur_buffer_lines
			COORD tmpcr;
			tmpcr.X = cr.X;
			tmpcr.Y = cur_buffer_lines;
			SetConsoleScreenBufferSize(hOutput, tmpcr);//设置缓冲区

			SetConsoleWindowInfo(hOutput, true, &rect);//设置窗口
			SetConsoleScreenBufferSize(hOutput, cr);//设置缓冲区
		}
	}
	return 0;
}
int Editor::cct_cls()
{
	COORD coord = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO binfo; /* to get buffer info */
	DWORD num;

	/* 取当前缓冲区信息 */
	GetConsoleScreenBufferInfo(hOutput, &binfo);
	/* 填充字符 */
	FillConsoleOutputCharacter(hOutput, (TCHAR)' ', binfo.dwSize.X * binfo.dwSize.Y, coord, &num);
	/* 填充属性 */
	FillConsoleOutputAttribute(hOutput, binfo.wAttributes, binfo.dwSize.X * binfo.dwSize.Y, coord, &num);

	/* 光标回到(0,0) */
	SetConsoleCursorPosition(hOutput, coord);
	return 0;
}
/***************************************************************************
  函数名称：GetPos
  功    能：取当前光标所在位置的坐标值
***************************************************************************/
int Editor::GetPos(int& x, int& y)
{
    CONSOLE_SCREEN_BUFFER_INFO binfo;
    GetConsoleScreenBufferInfo(hOutput, &binfo);
    x = binfo.dwCursorPosition.X;
    y = binfo.dwCursorPosition.Y;
    return 0;
}
/***************************************************************************
  函数名称：Goto
  功    能：定位函数
***************************************************************************/
int Editor::Goto(const int X, const int Y)
{
    COORD coord;
    coord.X = X;
    coord.Y = Y;
    SetConsoleCursorPosition(hOutput, coord);
    return 0;
}
/***************************************************************************
  函数名称：StoreFile
  功    能：储存文本到文件
***************************************************************************/
int Editor::StoreFile(string address) {
    ofstream outFile;
    outFile.open(address);
    string AllString = GetText(0, 0);
    int AllString_Size = AllString.size();
    for (int i = 0; i < AllString_Size; i++)
    {
        outFile << AllString[i];
    }
    outFile.close();
    return 0;
};
/***************************************************************************
  函数名称：OpenFile
  功    能：打开文件
***************************************************************************/
int Editor::OpenFile(string address) {
    ifstream in;
    in.open(address);        //以读的方式打开文件
    if (!in) {               //判断是否成功打开
		pos.X =73;
		if (pos.Y < 25)  pos.Y = 24;
		else pos.Y = 49;
		SetConsoleCursorPosition(hOutput, pos);
        cout << "ERROR!";
    }
	else {
		char ch[2 << 0];
		in >> ch;                //读取文件内字符串，还有其他读取方式，可根据需要使用
		in.close();              //关闭文件
		cout << ch << endl;
	}
    return 0;
}
/***************************************************************************
  函数名称：ShutDown
  功    能：退出程序
***************************************************************************/
int Editor::ShutDown() { 
    GetConsoleScreenBufferInfo(hOutput, &csbi);
    pos.X = 0;
    pos.Y = csbi.dwMaximumWindowSize.Y;
    SetConsoleCursorPosition(hOutput, pos);
    Sleep(100);
    exit(0);
}