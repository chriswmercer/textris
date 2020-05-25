//
//  main.cpp
//  textris
//
//  Created by Chris Mercer on 23/05/2020.
//  Copyright © 2020 Chris Mercer. All rights reserved.
//

#include <iostream>
#include <thread>
#include <vector>
using namespace std;

#include <stdio.h>
#include <Windows.h>

int nScreenWidth = 80;			// Console Screen Size X (columns)
int nScreenHeight = 30;			// Console Screen Size Y (rows)
wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr;

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

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
    for(auto px = 0; px < 4; px++)
    {
	    for(auto py = 0; py < 4; py++)
	    {
		    //get index ito piece
            auto pi = Rotate(px, py, nRotation);

	    	//get index into field
            auto fi = (nPosY + py) * nFieldWidth + (nPosX + px);

	    	if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
	    	{
	    		if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
	    		{
                    if (tetromino[nTetromino][pi] != L'.' && pField[fi] != 0) return false;
	    		}
	    	}
	    }
    }
    return true;
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

    pField = new unsigned char[nFieldWidth * nFieldHeight]; // Create play field buffer
    for (auto x = 0; x < nFieldWidth; x++) // Board Boundary
    {
        for (auto y = 0; y < nFieldHeight; y++)
        {
            pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
        }
    }
    	
    // Create Screen Buffer
    auto* screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (auto i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
    const auto h_console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER,
                                                     NULL);
    SetConsoleActiveScreenBuffer(h_console);
    DWORD dwBytesWritten = 0;

	
    auto bGameOver = false;
    auto nCurrentPiece = 0;
    auto nCurrentRotation = 0;
    auto nCurrentX = nFieldWidth / 2;
    auto nCurrentY = 0;
    auto rotating = false;

    auto nSpeed = 20;
    auto nSpeedCounter = 0;
    bool bForceDown = false;
    int nPieceCount = 0;
    int score = 0;

    vector<int> vlines;
	
    bool bKey[4];

    while (!bGameOver)
    {
        // timing
        this_thread::sleep_for(50ms);
        nSpeedCounter++;
        if(nSpeedCounter >= nSpeed)
        {
            bForceDown = true;
            nSpeedCounter = 0;
        }
        else
        {
            bForceDown = false;
        }
    	
    	//input
        for (auto k = 0; k < 4; k++) bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
    	
    	//logic
        nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
        nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
        nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

		if(bKey[3])
		{
            nCurrentRotation += (!rotating && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
            rotating = true;
        }
        else 
        {
        	rotating = false;
        }

    	if (bForceDown)
    	{
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
            {
                nCurrentY++;
            }
            else
            {
	            //stick in place
            	for(auto px = 0; px < 4; px++)
            	{
            		for(auto py = 0; py < 4; py++)
            		{
            			if(tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
            			{
                            pField[(nCurrentY + py)* nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;
            			}
            		}
            	}

                nPieceCount++;
                if (nPieceCount % 10 == 0 && nSpeed >= 10) nSpeed--;

            	// check lines
                for (int py = 0; py < 4; py++)
                {
                    if (nCurrentY + py < nFieldHeight - 1)
                    {
                        bool bLine = true;
                        for (int px = 1; px < nFieldWidth - 1; px++)
                            bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

                        if (bLine)
                        {
                            // Remove Line, set to =
                            for (int px = 1; px < nFieldWidth - 1; px++)
                                pField[(nCurrentY + py) * nFieldWidth + px] = 8;
                            vlines.push_back(nCurrentY + py);
                        }
                    }
                }

                score += 25;
                if (!vlines.empty()) score += (1 << vlines.size()) * 100;
            	
            	// get next piece
                nCurrentX = nFieldWidth / 2;
                nCurrentY = 0;
                nCurrentRotation = 0;
                nCurrentPiece = rand() % 7;

            	//if piece does not fit, game over!
                bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1);
            }
    	}
        	
        // Draw Field
        for (auto x = 0; x < nFieldWidth; x++)
            for (auto y = 0; y < nFieldHeight; y++)
                screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];

        // Draw Current Piece
        for (auto px = 0; px < 4; px++)
            for (auto py = 0; py < 4; py++)
                if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                    screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;

        swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", score);
    	
    	//display frame
        if (!vlines.empty())
        {
            WriteConsoleOutputCharacter(h_console, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
            this_thread::sleep_for(400ms);

        	for(auto &v : vlines)
        	{
        		for(auto px = 1; px < nFieldWidth - 1; px++)
        		{
        			for(auto py = v; py > 0; py--)
        			{
                        pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
                        pField[px] = 0;
        			}
        		}
        	}

            vlines.clear();
        }

        WriteConsoleOutputCharacter(h_console, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
    }

    CloseHandle(h_console);
    cout << "Game over. Your score was: " << score << endl;
    system("pause");
    return 0;
}
