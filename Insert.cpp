#include "head.h"

int Editor::Insert_Pattern() {
    char ch;
    pos.X = 0;
    pos.Y = 0;
    setcursor(2);
    while (1)  ch = Insert_GetInput();
    return 0;
}

char Editor::Insert_GetInput() {
    char ch;
    ch = _getch();
    GetConsoleScreenBufferInfo(hOutput, &csbi);
    pos.X = csbi.dwCursorPosition.X;
    pos.Y = csbi.dwCursorPosition.Y;

    switch (ch) {
    case 0x48:
        if (pos.Y != 0)  pos.Y--;
        SetConsoleCursorPosition(hOutput, pos);
        break;

    case 0x50:
        pos.Y++;
        SetConsoleCursorPosition(hOutput, pos);
        break;

    case 0x4B:
        if (pos.X != 0) pos.X--;
        else {
            pos.X = 79;
            pos.Y--;
        }
        SetConsoleCursorPosition(hOutput, pos);
        break;

    case 0x4D:
        if (pos.X != 79)  pos.X++;
        else {
            pos.X = 0;
            pos.Y++;
        }
        SetConsoleCursorPosition(hOutput, pos);
        break;

    case '\r':
        pos.X = 0;
        pos.Y++;
        SetConsoleCursorPosition(hOutput, pos);
        //   keyword search 
        if (text[pos.Y - 1][0] == '\\') {
            string str = line.substr(1, (int)line.size() - 2);
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

    case 0x1B:
        Normal_Pattern();
        break;

    case '\\':
        GetConsoleScreenBufferInfo(hOutput, &csbi);
        loc.X = csbi.dwCursorPosition.X;
        loc.Y = csbi.dwCursorPosition.Y;
        pos.X = 0;
        if (pos.Y < 25)  pos.Y = 24;
        else pos.Y = 49;
        SetConsoleCursorPosition(hOutput, pos);
        text[pos.Y][pos.X] = ch;
        cout << ch;
        break;

    default:
        if (ch != -32) _insert(ch);
    }
    return ch;
}