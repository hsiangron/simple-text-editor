#include "head.h"

int Editor::Normal_Pattern() {
    char ch;
    pos.X = 0;
    pos.Y = 0;
    setcursor(0);
    while (1)  ch = Normal_GetInput();
    return 0;
}

char Editor::Normal_GetInput() {
    char ch;
    ch = _getch();
    GetConsoleScreenBufferInfo(hOutput, &csbi);
    pos.X = csbi.dwCursorPosition.X;        
    pos.Y = csbi.dwCursorPosition.Y;

    switch (ch) {
    case 'k':
        if (pos.Y != 0)  pos.Y--;
        SetConsoleCursorPosition(hOutput, pos);
        break;

    case 'j':
        pos.Y++;
        SetConsoleCursorPosition(hOutput, pos);
        break;

    case 'h':
        if (pos.X != 0) pos.X--;
        else {
            pos.X = csbi.dwMaximumWindowSize.X - 1;
            pos.Y--;
        }
        SetConsoleCursorPosition(hOutput, pos);
        break;

    case 'l':
        if (pos.X != 0) pos.X--;
        else {
            pos.X = 79;
            pos.Y--;
        }
        SetConsoleCursorPosition(hOutput, pos);
        break;

    case '\r':
        pos.X = 0;
        pos.Y++;
        SetConsoleCursorPosition(hOutput, pos);
        //   w filename
        if (text[pos.Y - 1][0] == 'w' && text[pos.Y - 1][1] == ' ') {
            string  filename = text[pos.Y - 1].substr(2, (int)text[pos.Y - 1].size() - 3);
            StoreFile(filename);
        }
        //   open filename
        if (text[pos.Y - 1][0] == 'o' && text[pos.Y - 1][1] == 'p' && text[pos.Y - 1][2] == 'e' && text[pos.Y - 1][3] == 'n' && text[pos.Y - 1][4] == ' ') {
            string filename = text[pos.Y - 1].substr(5, (int)text[pos.Y - 1].size() - 6);
            OpenFile(filename);
        }
        //   keyword search
        if (text[pos.Y - 1][0] == '\\') {
            int i = 1; string str;
            for (; text[pos.Y - 1][i] != ' '; i++);
            str = text[pos.Y - 1].substr(1, i - 1);
            SetConsoleCursorPosition(hOutput, loc);
            BM_Search(str);
        }
        text[pos.Y - 1] = text[pos.Y - 1];
        break;

    case 'x':
        GetConsoleScreenBufferInfo(hOutput, &csbi);
        pos.X = csbi.dwCursorPosition.X;
        pos.Y = csbi.dwCursorPosition.Y;
        if (pos.X == 0 && pos.Y > 1) {
            pos.X = 79;
            pos.Y--;
        }
            Del(pos);
        break;

    case 0x08:
        break;

    case 'u':
        Cancel();
        break;

    case 'q':
        ShutDown();
        break;

    case 'i':
        Insert_Pattern();
        break;

    default:
        text[pos.Y][pos.X] = ch;
        cout << ch;
        break;
    }

    return ch;
}