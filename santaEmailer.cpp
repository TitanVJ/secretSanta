#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <mailio/message.hpp>
#include <mailio/smtp.hpp>

using namespace std;

void Tokenize(string line, vector<string>& tokens, string delimiters = ",") {
	string token = "";
	string OneCharString = " ";
	for (int i = 0; i < line.size(); i++)
		if (find(delimiters.begin(), delimiters.end(), line[i]) != delimiters.end()) // line[i] is one of the delimiter characters
		{
			tokens.push_back(token);
			token = "";
		}
		else
		{
			OneCharString[0] = line[i];
			token += OneCharString;
		}

	if (token != "")
		tokens.push_back(token);
}

int main(int argc, char* argv[]){
    if(argc < 2){
        cout << "Usage: ./santaEmailer.o message.txt\nWhere message.txt is the file containing the message you want in the email sent." << endl;
        return;
    }

    /*
        1. load in master list
        2. sort by santaId or index order == santaId order
        3. send emails

        sending emails:
            set recipient as dude from master list, 
            grab attachment file as recip's _gifterid.txt
            profit
        
    */
   cout << "Ho Ho Ho" << endl << "Welcome the Secret Santa Emailer" << endl << "Let\'s begin!!" << endl;
   cout << "Reading Secret Santa Master list" << endl;

}