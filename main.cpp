#include <iostream>
#include <fstream>
#include <vector> 
#include <algorithm>
#include <string>
#include <regex>
#include "Santa.h"
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

void getQuestions(vector<string>& q, string file){
    ifstream input;
    input.open(file);

    string line;

    while(getline(input, line)){
        int sLen = line.length();
        for(int i = 0; i < sLen; ++i){
            line[i] = toupper(line[i]);
        }
        q.push_back(line);
    }
}

void parseData(vector<vector<string>>& data, string file){
    // do something
    ifstream input;
    input.open(file);

    string line; 
    vector<string> details;
    regex re("@sfu\.ca$");

    // Remove first line; contains the question numbers
    getline(input, line);
    line = "";

    while(getline(input, line)){
        Tokenize(line, details);
        if(regex_search(details[2], re)){
            data.push_back(details);
        }
        details.clear();
    }
}

void genSantas(vector<vector<string>>& data, vector<Santa>& santas){
    int numberOfParticpants = data.size();
    int sfuId, age, innerSize;
    string name, email;
    vector<string> details;

    for(int i = 0; i < numberOfParticpants; ++i){
        sfuId = stoi(data[i][0]);
        name = data[i][1];
        email = data[i][2];
        age = stoi(data[i][3]);

        // everything after is the details
        innerSize = data[i].size();

        for(int j = 4; j < innerSize; ++j){
            details.push_back(data[i][j]);
        }

        // generate the santa profiles and push onto the santa vector
        santas.push_back(Santa(name, email, age, sfuId, details));
        details.clear();
    }
}

int main(int argc, char *argv[]){
    /*
        Plan:
            1. parse data
            2. parse questions & cull invalidate emails
            2.5 create Santa objects
            3. output files
            4. shuffle and randomize
            5. assign gifters
            6. output menu to terminal [show gifter/giftee pairings, send emails]
    */
   if(argc < 3){
       cout << "Usage: ./{name} data.txt questions.txt" << endl;
       cout << "data.txt is survey data file downloaded from websurvey" << endl;
       cout << "questions.txt is a text file containing the questions from the survey regarding the details asked about the participant" << endl;
       return 1;
   }

    // currently acting as placeholder for the all the data in the user profiles
    // once ready this vector will be holding Santa profiles
    vector<vector<string>> data;
    vector<string> questions;
    vector<Santa> santas;

    parseData(data, argv[1]);
    getQuestions(questions, argv[2]);
    genSantas(data, santas);

    
    
    // All code below is for testing data parsing
    int h = 0;
    cout << santas[0] << endl;
    for(auto i = data.begin(); i != data.end(); ++i){
        cout << "person " << h << endl;
        for(auto j = i->begin(); j != i->end(); ++j){
            cout << *j;
            auto k = j;
            k++;
            if(k != i->end())
                cout << " ";
            //here it'll populate the data structure for the participants        }
        }
        cout << endl;
        ++h;
    }

    for(auto i = questions.begin(); i != questions.end(); ++i){
        cout << *i << endl;
    }
}
