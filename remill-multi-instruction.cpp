#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "extract-instructions.cpp"

using namespace std;


void combine_instructions(vector<string> instructions, string dst)
{
    //Find repeated code before the function and add it to output file
    int repeatedBlockIndex = instructions[0].find("define");
    ofstream outputFile(dst);
    outputFile << instructions[0].substr(0, repeatedBlockIndex);
    vector<string> postFunctionInstructions;

    for (int i = 0; i < instructions.size(); i++)
    {
        string currentInstruction = instructions[i];
        string codeAfterFunction = "";

        //remove repeated code from all instructions
        currentInstruction = currentInstruction.substr(repeatedBlockIndex);

        //find where function ends
        int functionNumberIndex = currentInstruction.find("@sub") + 5;
        int functionEndIndex = currentInstruction.find("}") + 2;
        int firstFunctionEndIndex;
        int functionNumberEndIndex = currentInstruction.find("(");

        //check for multiple functions
        string multiFunctionsCheckString = currentInstruction.substr(functionEndIndex);
        int secondFunctionIndex = multiFunctionsCheckString.find("define ");
        if(secondFunctionIndex != string::npos)
        {
            //Change index variables to account for second function
            firstFunctionEndIndex = functionEndIndex;
            codeAfterFunction += currentInstruction.substr(functionEndIndex, secondFunctionIndex);
            string secondFunction = multiFunctionsCheckString.substr(secondFunctionIndex);
            secondFunctionIndex += functionEndIndex;
            functionNumberIndex = secondFunction.find("@sub_") + 5 + secondFunctionIndex;
            functionNumberEndIndex = secondFunction.find("(") + secondFunctionIndex;
            functionEndIndex = secondFunction.find("}") + 2 + secondFunctionIndex; 
        }

        //find repeated code after function
        codeAfterFunction += currentInstruction.substr(functionEndIndex);
        
        string line = "";

        //separate lines after the function
        for(int j = 0; j < codeAfterFunction.length(); j++)
        {
            line += codeAfterFunction[j];
            if(codeAfterFunction[j] == '\n' && line != "\n")
            {
                if(find(postFunctionInstructions.begin(), postFunctionInstructions.end(), line) == postFunctionInstructions.end())
                {
                    postFunctionInstructions.push_back(line);
                }
                line = "";
            }
            else if(line == "\n")
            {
                //ignore empty lines
                line = "";
            }
        }

        //change each functions name to corespond to index(sub_0, sub_1, sub_2, etc)
        if(secondFunctionIndex == string::npos)
        {
            currentInstruction = currentInstruction.substr(0, functionNumberIndex) +
                to_string(i) + currentInstruction.substr(functionNumberEndIndex, functionEndIndex - functionNumberEndIndex);
        }
        else
        {
            cout << "----------------------"<<endl;
            cout<< currentInstruction.substr(secondFunctionIndex, functionNumberIndex - secondFunctionIndex)<<endl;
            currentInstruction = currentInstruction.substr(0, firstFunctionEndIndex) + 
                currentInstruction.substr(secondFunctionIndex, functionNumberIndex - secondFunctionIndex) +
                to_string(i) + currentInstruction.substr(functionNumberEndIndex, functionEndIndex - functionNumberEndIndex);
            
        }
       
        //write to output.txt
        outputFile << currentInstruction + "\n";

    }

    for(int i = 0; i < postFunctionInstructions.size(); i++)
    {
        outputFile << postFunctionInstructions[i];
    }

    outputFile.close();
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

        //remove("temp.txt");

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
