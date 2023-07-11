#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char* argv[]){
    string instruction;
    vector<string> instructions;
    string translateCmd = "remill-lift-16 --arch amd64 --ir_out /dev/stdout --bytes ";
    string logCmd = " | tee -a output.txt";

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
            cout << (translateCmd + instructions[i]).c_str() << endl;
            cout << instructions[i].length() << endl;
            system((translateCmd + instructions[i] + logCmd).c_str());
        }

        //combine instructions?

        //output translated instructions to output file

        //close input file
        instructionsInputFile.close();
    }
    else
    {
        cout <<"Unable to open file" << endl;
    }
    

    

    
}