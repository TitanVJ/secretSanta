#include <iostream>
#include <fstream>
#include <vector> 
#include <algorithm>
#include <string>
// smtp library here 

using namespace std; 

vector<string> questions;

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

void getQuestions(string file){
    ifstream input;
    input.open(file);

    string line;

    while(getline(input, line)){
        questions.push_back(line);
    }
}

void parseData(vector<vector<string>>& data, string file){
    // do something
    ifstream input;
    input.open(file);

    string line; 
    vector<string> details; 

    while(getline(input, line)){
        Tokenize(line, details);
        data.push_back(details);
        details.clear();

    }
}

int main(int argc, char *argv[]){
    /*
        Plan:
            1. parse data
            2. parse questions
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

    vector<vector<string>> data;
    parseData(data, argv[1]);
    getQuestions(argv[2]);

    for(auto i = data.begin(); i != data.end(); ++i){
        for(auto j = i->begin(); j != i->end(); ++j){
            cout << *j;
            auto k = j;
            k++;
            if(k != i->end())
                cout << " ";
            //here it'll populate the data structure for the participants        }
        }
        cout << endl;
    }

    for(auto i = questions.begin(); i != questions.end(); ++i){
        cout << *i << endl;
    }
}
