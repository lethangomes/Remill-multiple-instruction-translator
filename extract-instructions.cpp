#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

vector<string> extract_instructions(ifstream& inputFile)
{
    vector<string> out;
    ofstream outFile("extracted_instructions.txt");
    //find where instructions start
    string currentLine;
    while(getline(inputFile, currentLine))
    {
        if(currentLine.find("Instructions") != string::npos)
        {
            break;
            cout << "it worked" << endl;
        }
    }

    //extract instructions
    while(getline(inputFile, currentLine))
    {
        if(currentLine.find("RelocInfo") == string::npos)
        {
            if(currentLine.substr(0,2) == "0x")
            {
                out.push_back(currentLine.substr(22, currentLine.find(" "))+ "\r");
                outFile << currentLine.substr(22, currentLine.find(" "))+ "\r";
            }
            else
            {
                continue;
            }
            
        }
        else
        {
            break;
        }
        
    }

    return out;
}
