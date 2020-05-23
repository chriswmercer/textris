//
//  main.cpp
//  textris
//
//  Created by Chris Mercer on 23/05/2020.
//  Copyright © 2020 Chris Mercer. All rights reserved.
//

#include <iostream>
#include <curses.h>

std::wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char *pField = nullptr;

int nScreenHeight = 80;
int nScreenWidth = 30;

int Rotate(int px, int py, int r)
{
    switch (r % 4)
    {
        case 0: return py * 4 + px; //0 deg
        case 1: return 12 + py - (px * 4); //90 deg
        case 2: return 15 - (py * 4) - px; //180
        case 3: return 3 - py + (px * 4); //270
    }
    return 0;
}

int main()
{
    //create assets
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    
    tetromino[1].append(L"....");
    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");
    tetromino[1].append(L"..X.");
    
    tetromino[2].append(L"....");
    tetromino[2].append(L".X..");
    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    
    tetromino[3].append(L"....");
    tetromino[3].append(L"..X.");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L".X..");
    
    tetromino[4].append(L"....");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"..X.");
    tetromino[4].append(L"..X.");
    
    tetromino[5].append(L"....");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L".X..");
    tetromino[5].append(L".X..");
    
    tetromino[6].append(L"....");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L"....");
    
    pField = new unsigned char[nFieldWidth * nFieldHeight];
    for (int x = 0; x < nFieldWidth; x++)
    {
        for (int y = 0; y < nFieldHeight; y++)
        {
            pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight ) ? 9 : 0;
        }
    }
    
    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
    for(int pos = 0; pos < nScreenWidth * nScreenHeight; pos++) screen[pos] = L' ';
    
    initscr();    /* Start curses mode         */
    cbreak();    /* Line buffering disabled. Pass on every thing */
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    noecho();

    wclear(stdscr);
    
    bool bGameOver = false;
    
    while (!bGameOver)
    {
        
    }
    

    endwin();        /* End curses mode          */
    return 0;
}
