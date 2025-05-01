#pragma once

#include <iostream>
#include <fstream>

void saveHighScore(int highscore)
{
    std::ofstream outFile("save/highscore.txt");
    if (outFile.is_open())
    {
        outFile << highscore;
        outFile.close();
    }
}

int loadHighScore()
{
    std::ifstream inFile("save/highscore.txt"); //
    int highscore = 0;
    if (inFile.is_open())
    {
        inFile >> highscore;
        inFile.close();
    }

    return highscore;
}