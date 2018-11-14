#include <iostream>
#include <fstream>
#include <vector> 
#include <algorithm>
#include <string>
#include <regex>
#include "Santa.h"
#include <mailio/message.hpp>
#include <mailio/smtp.hpp>

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
    input.close();
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
    input.close();
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

int myRand(int i){
    return rand()%i;
}

void shuffleAndRandomize(vector<Santa>& s){
    /*  Shuffles and randomizes the participants of secret santa
        and assigns everyone's santa and gifter id's
    */

    int size = s.size();
    vector<int> santaIndex;
    
    // This init the vector with index's 
    // for the santa vector, it'll be used
    // to randomize and shuffle them
    for(int i = 0; i < size; ++i){
        santaIndex.push_back(i);
    }

    // Randomize setup
    srand(unsigned(time(0)));

    // Random Shuffle
    random_shuffle(santaIndex.begin(), santaIndex.end(), myRand);

    // Assign santa and gifter Id's
    for(int i = 0; i < size - 1; ++i){
        s[i].santaId = santaIndex[i];
        s[i].gifterId = santaIndex[i + 1];
    }

    // Assign the last person in the index
    // since it wrap around 
    s[size - 1].santaId = santaIndex[size - 1];
    s[size - 1].gifterId = santaIndex[0];

}

void genSantaFiles(vector<Santa>& s, vector<string>& q){
    string fileName, pubFileName, name, email;
    int age, sfuId, santaId, gifterId;
    int numOfQs = q.size();
    vector<string> details;
    ofstream adminFile, publicFile, masterFile;

    system("exec mkdir adminFiles");
    system("exec mkdir santaFiles");
    
    // This will be the master list of secret santa entrants
    // it'll be csv file with santaId, gifterId, sfuId, name, email
    masterFile.open("adminFiles/_0_MasterSantaList.cvs");
    masterFile << "santaId,gifterId,sfuId,Name,Email" << endl;

    for(auto i = s.begin(); i != s.end(); ++i){
        Santa santa = *i;

        // Grab santa's data
        name = santa.name;
        email = santa.email; 
        age = santa.age;
        sfuId = santa.sfuId;
        santaId = santa.santaId; 
        gifterId = santa.gifterId;
        details = santa.details;
        
        fileName = string("_") + to_string(santa.santaId) + "_" + santa.name + ".txt";
        pubFileName = "_" + to_string(santa.santaId) + ".txt";
        adminFile.open("adminFiles/" + fileName);
        publicFile.open("publicFiles/" + pubFileName);

        // Master file entry
        masterFile << santaId << "," << gifterId << "," << sfuId << "," << name << "," << email << endl;

        // Admin file output
        adminFile << "Name:\n" << name << endl << endl;
        adminFile << "Email:\n" << email << endl << endl;
        adminFile << "Age:\n" << age << endl << endl;
        adminFile << "SFU Id:\n" << sfuId << endl << endl;
        adminFile << "Santa Id" << santaId << endl << endl;
        adminFile << "Gifter Id" << gifterId << endl << endl;
        adminFile << "For details look to file publicFiles/" << pubFileName << endl << endl;

        // Public files output
        publicFile << "Name:\n" << name << endl << endl;
        publicFile << "Email:\n" << email << endl << endl;
        publicFile << "Age:\n" << age << endl << endl;

        // Details output for public files
        for(int j = 0; j < numOfQs; ++j){
            publicFile << q[j] << endl;
            publicFile << details[j] << endl << endl;
        }
    }
    masterFile.close();
    adminFile.close();
    publicFile.close();

}

void sendTestEmails(vector<Santa>& s){
    string emailFileName, smtpServer, userEmail, userPass, fromField, name, subject, line;
    string emailMsg = "";
    int port;
    ifstream emailMessage;

    // Get email information
    cout << "This Test E-mail will act as a confirmation email to participants" << endl;
    cout << "Enter filename for the file that contains the email's message (include path if not in current dir): ";
    cin >> emailFileName;
    cout << endl;

    cout << "Note: Only the gmail SMTP Server will make a record of the email sent in you sent folder" << endl;
    cout << "You can you any SMTP Server as long as you have the credentials to login, and then set the from field to whatever you want" << endl;
    cout << "Consider the above note for the following prompts" << endl;
    cout << endl;

    cout << "Enter the SMTP Server: ";
    cin >> smtpServer;
    cout << "Enter the port number for the SMTP Server: "; 
    cin >> port;
    cout << endl;

    cout << "Enter your login email for the given SMTP Server: ";
    cin >> userEmail;
    cout << "Enter your login password for the given SMTP Server: "; 
    cin >> userPass;
    cout << endl;
    
    cout << "Enter the email to go in the \"FROM\" field of the email (this'll be the emails replies are sent to): "; 
    cin >> fromField;
    cout << "Enter name to appear in \"FROM\" field: ";
    cin >> name;
    cout << "Enter the email subject line: ";
    cin >> subject;


    emailMessage.open(emailFileName);
    while(getline(emailMessage, line)){
        emailMsg += line;
    }
    emailMessage.close();

    // Make the mailio objects
    try{
        mailio::smtps conn(smtpServer, port);
        conn.authenticate(userEmail, userPass, mailio::smtps::auth_method_t::LOGIN);

        for(auto i = s.begin(); i != s.end(); ++i){
            Santa santa = *i;
            mailio::message msg;
            msg.from(mailio::mail_address(name, fromField));
            msg.add_recipient(mailio::mail_address(santa.name, santa.email));
            msg.subject(subject);
            msg.content(emailMsg);
            conn.submit(msg);

        }
    }
    catch (mailio::smtp_error& exc)
    {
        cout << exc.what() << endl;
    }
    catch (mailio::dialog_error& exc)
    {
        cout << exc.what() << endl;
    }

}

void removeEntries(vector<Santa>& s){
    string name, yN;
    bool cont = true;
    bool found = false;

    while(cont){
        cout << "Enter name of participant to remove";
        cin >> name;

        for (auto i = s.begin(); i != s.end(); ++i){
            Santa santa = *i; 
            if (santa.name == name){
                // delete entry in vector
                cout << "Entrant " << name << " found and removed" << endl;
                break;
            }
        }
        if(!found)
            cout << "Entrant" << name << " not found in list, make sure you typed the name correctly." << endl;

        cout << "Do you have more entrants to delete? (Y or N): "; 
        cin >> yN;
        if(yN != "Y"){
            cont = false;
        }
    }
}

int main(int argc, char *argv[]){
    /*
        Plan:
            1. parse data
            2. parse questions & cull invalidate emails
            3. create Santa objects

            OUTPUT MENU[test email, remove entry, shuffle and randomize]
            have multiple menu's only give shuffle and randomize after test email and removal menu
            maybe just auto shuffle after they exit first menu and ouput all the files


            3.5 verify emails [remove non sfu and test email]
            4. shuffle and randomize
            5. assign santa id and gifter id's
            6. output files both admin and public
            7. output menu to terminal
            menu options: 
                - view data
                - send emails
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

    //output menu
    int choice;
    bool menu = true;
    while(menu){
        cout << "Menu:" << endl;
        cout << "1. Send Test Email" << endl;
        cout << "2. Remove Secret Santa Entry" << endl;
        cout << "3. Exit menu" << endl;
        cout << "Enter the number matching your menu choice:";
        cin >> choice;

        switch(choice){
            case 1:
                sendTestEmails(santas);
                cout << endl << "Check your email to see if any emails bounced back and then use menu option 2 if some did" << endl;
                break;
            case 2:
                removeEntries(santas);
                break;
            case 3:
                menu = false;
            default: 
                continue;
        }
        choice = NULL;
    }
    // shuffleAndRandomize(santas);
    // genSantaFiles(santas, questions);
    
    
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

    for(auto i = santas.begin(); i != santas.end(); ++i){cout << *i <<endl;}
}
