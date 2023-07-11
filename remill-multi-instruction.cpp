#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


void combine_instructions(vector<string> instructions, string dst)
{
    //get everything up and including to the "define" line

    //combine all the code between the "define" line and the 
}

int main(int argc, char* argv[]){
    string instruction;
    vector<string> instructions;
    vector<string> translatedInstructions;
    string translateCmd = "remill-lift-16 --arch amd64 --ir_out /dev/stdout --bytes ";
    string logCmd = " > output.txt";

    //get input file
    ifstream instructionsInputFile (argv[1]);
    if(instructionsInputFile.is_open())
    {
        //Isolate each instructions
        while(getline (instructionsInputFile, instruction))
        {
            instructions.push_back(instruction);
        }

        //translate each instruction 
        for(int i = 0; i < instructions.size(); i++)
        {
            //checks if the last character is a carriage return and removes it if it is
            if(instructions[i][instructions[i].length()-1] == '\r')
            {
                instructions[i].pop_back();
            }

            //translates current instruction and saves result to output.txt
            cout << (translateCmd + instructions[i]).c_str() << endl;
            system((translateCmd + instructions[i] + logCmd).c_str());

            //Adds translated instruction to a vector of strings
            ifstream translatedInstruction ("output.txt");
            translatedInstructions.push_back("");
            string currentLine;
            while(getline (translatedInstruction, currentLine))
            {
                translatedInstructions[i] = translatedInstructions[i] + currentLine + "\n";
            }
            translatedInstruction.close();
        }

        //combine instructions
        combine_instructions(translatedInstructions, "output.txt");

        //close input file
        instructionsInputFile.close();
    }
    else
    {
        cout <<"Unable to open file" << endl;
    }
    

    

    
}
