#include <iostream>
#include <fstream>
#include <vector> 
#include <algorithm>
#include <string>
// smtp library here 

using namespace std; 

void Tokenize(string line, vector<string>& tokens, string delimiters = "\t") {
	string token = "";
	string OneCharString = " ";
    int delimEncounters = 0;
	for (int i = 0; i < line.size(); i++)
		if (find(delimiters.begin(), delimiters.end(), line[i]) != delimiters.end()) // line[i] is one of the delimiter characters
		{
            // This'll skip the date and the consent answer
            if(delimEncounters < 2) {
                delimEncounters++;
                token = "";
                continue;
            }
            else {
                tokens.push_back(token);
                token = "";
            }
		}
		else
		{
			OneCharString[0] = line[i];
			token += OneCharString;
		}

	if (token != "")
		tokens.push_back(token);
}

void parseDate(vector<vector<string>>& data){
    // do something
    string line; 
    vector<string> details; 

    while(getline(cin, line)){
        Tokenize(line, details);
        data.push_back(details);
        details.clear();

    }
}

int main(int argc, char *argv[]){

    /* Plan: 
    1: data parsing
    2: shuffle 
    3: randomize
    4: email

    part of emailing will be a file containing the template
    email with keywords for things to replace, basically their names 

    gonna need a shit ton of functions make a person class or struct 
    will be used for an oop design. maybe future things as this just needs to work
    */

    /*
    Menu entries: 

    */
    string temp = "";
    getline(cin, temp); // The first line is the question #'s

    for(auto i = list.begin(); i != list.end(); ++i){
        for(auto j = i->begin(); j != i->end(); ++j){
            cout << *j << " ";
            //here it'll populate the data structure for the participants        }
        cout << endl;
    }
}