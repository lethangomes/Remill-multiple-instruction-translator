#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "extract-instructions.cpp"
using namespace std;


void combine_instructions(vector<string> instructions, string dst)
{
    //Find repeated code and add it to output file
    int repeatedBlockIndex = instructions[0].find("define");
    ofstream outputFile(dst);
    outputFile << instructions[0].substr(0, repeatedBlockIndex);

    int functionNumberIndex = instructions[0].find("@sub_0") - repeatedBlockIndex + 5;

    for (int i = 0; i < instructions.size(); i++)
    {
        //remove repeated code from all instructions
        instructions[i] = instructions[i].substr(repeatedBlockIndex);

        //change each functions name to corespond to index(sub_0, sub_1, sub_2, etc)
        if(i != 0)
        {
            instructions[i]= instructions[i].substr(0, functionNumberIndex) +
                to_string(i) + instructions[i].substr(functionNumberIndex +1);
        }

        //write to output.txt
        outputFile << instructions[i] + "\n";
    }
}

int main(int argc, char* argv[]){
    string instruction;
    vector<string> instructions;
    vector<string> translatedInstructions;
    string commandString = "remill-lift-16 --arch amd64 --ir_out /dev/stdout --bytes ";
    string logCmd = " > temp.txt";

    //get input file
    ifstream instructionsInputFile (argv[1]);
    if(instructionsInputFile.is_open())
    {
        
        //change command based on arguments from user
        if(argc >= 3)
        {
            commandString = "";
            for(int i = 2; i < argc; i++)
            {
                commandString = commandString + argv[i] + " ";
            }
        }
        //extract instructions from file
        instructions = extract_instructions(instructionsInputFile);
        
        //translate each instruction 
        for(int i = 0; i < instructions.size(); i++)
        {
            //checks if the last character is a carriage return or newline and removes it if it is
            if(instructions[i][instructions[i].length()-1] == '\r' ||
                instructions[i][instructions[i].length()-1] == '\n')
            {
                instructions[i].pop_back();
            }

            //translates current instruction and saves result to output.txt
            cout << (commandString + instructions[i]).c_str() << endl;
            system((commandString + instructions[i] + logCmd).c_str());

            //Adds translated instruction to a vector of strings
            ifstream translatedInstruction ("temp.txt");
            translatedInstructions.push_back("");
            string currentLine;
            while(getline (translatedInstruction, currentLine))
            {
                translatedInstructions[i] = translatedInstructions[i] + currentLine + "\n";
            }
            
            //close temp file
            translatedInstruction.close();
        }

        remove("temp.txt");

        //combine instructions
        combine_instructions(translatedInstructions, "output.txt");

        //close input file
        instructionsInputFile.close();

        cout << "Total number of instructions: " + to_string(instructions.size()) << endl;
    }
    else
    {
        cout <<"Unable to open file" << endl;
    }
    
}
