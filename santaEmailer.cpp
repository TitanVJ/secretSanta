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

void sendEmails(string smtpServer, int port, string smtpEmail, string smtpPass, string fromEmail, string fromName, string msgContent, string subject, vector<vector<string>>& s){
    vector<string> santa;
    string attachName;
    ofstream emailRecord;
    ifstream attachFile;

    emailRecord.open("adminFiles/finalEmailRecord.txt");
    emailRecord << "Emails were sent to the following:" << endl;

    // Make the mailio objects
    try{
        mailio::smtps conn(smtpServer, port);
        conn.authenticate(smtpEmail, smtpPass, mailio::smtps::auth_method_t::LOGIN);

        for(auto i = s.begin(); i != s.end(); ++i){
            santa = *i;
            // santaId, gifterId, SFUid, Name, Email
            //    0   ,    1    ,   2  ,   3 ,   4
            attachName = "santaFiles/_" + santa[1] + ".txt";
            mailio::message msg;
            msg.from(mailio::mail_address(fromName, fromEmail));
            msg.add_recipient(mailio::mail_address(santa[3], santa[4]));
            msg.subject(subject);
            cout << endl;
            attachFile.open(attachName);
            msg.attach(attachFile, attachName, mailio::message::media_type_t::TEXT, "txt");

            msg.content(msgContent);
            conn.submit(msg);
            attachFile.close();

            // Make entry in email record fild
            emailRecord << "SantaId:\t" << santa[0] << "\tEmail:\t" << santa[4] << endl;
        }
        emailRecord.close();
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

int main(int argc, char* argv[]){
    // if(argc < 2){
    //     cout << "Usage: ./santaEmailer.o message.txt\nWhere message.txt is the file containing the message you want in the email sent." << endl;
    //     return;
    // }

    /*
        1. load in master list
        2. sort by santaId or index order == santaId order
        3. send emails

        sending emails:
            set recipient as dude from master list, 
            grab attachment file as recip's _gifterid.txt
            profit
        
    */
    vector<vector<string>> santas;
    vector<string> details;
    ifstream inFile;
    string line;
    cout << "Ho Ho Ho" << endl << "Welcome the Secret Santa Emailer" << endl << "Let\'s begin!!" << endl << endl;
    cout << "Reading Secret Santa Master list" << endl;

    inFile.open("adminFiles/_0_MasterSantaList.cvs");
    getline(inFile, line);
    line = "";

    while(getline(inFile, line)){
        Tokenize(line, details);
        santas.push_back(details);
        details.clear();
    }
    inFile.close();
    
    // Get mailio information
    string smtpServer, smtpEmail, smtpPass, fromEmail, fromName, msgFile, subject;
    string msgContent = "";
    int port;
    line = "";
    

    // Get email information
    cout << "This Test E-mail will contain info about the Secret Santa giftee." << endl;
    cout << "Enter filename for the file that contains the email's message (include path if not in current dir): ";
    cin >> msgFile;
    cout << endl;

    cout << "Note: Only the gmail SMTP Server will make a record of the email sent in your sent folder" << endl;
    cout << "You can you any SMTP Server as long as you have the credentials to login, and then set the from field to whatever you want" << endl;
    cout << "Consider the above note for the following prompts" << endl;
    cout << endl;

    cout << "Enter the SMTP Server: ";
    cin >> smtpServer;
    cout << "Enter the port number for the SMTP Server: "; 
    cin >> port;
    cout << endl;

    cout << "Enter your login email for the given SMTP Server: ";
    cin >> smtpEmail;
    cout << "Enter your login password for the given SMTP Server: "; 
    cin >> smtpPass;
    cout << endl;

    cout << "Enter the email to go in the \"FROM\" field of the email (this'll be the emails replies are sent to): "; 
    cin >> fromEmail;
    cout << "Enter name to appear in \"FROM\" field: ";
    cin >> fromName;
    cout << "Enter the email subject line: ";
    cin >> subject;

    ifstream emailMsg;
    emailMsg.open(msgFile);

    while(getline(emailMsg, line)){
        msgContent += line + "\r\n";
    }
    emailMsg.close();

    char test;
    while(true){
        cout << "Do you want to start sending the emails? (Y or N): ";
        cin >> test;
        if(test == 'y' || test == 'Y')
            break;
    }
    cout << endl << "Starting the email process. " << endl;

    sendEmails(smtpServer, port, smtpEmail, smtpPass, fromEmail, fromName, msgContent, subject, santas);
    

    // below nested loop is for testing
    for(auto i = santas.begin(); i != santas.end(); ++i){
        details = *i;
        cout << "=============================" << endl;
        for (auto j = details.begin(); j != details.end(); ++j){
            cout << *j << endl;
        }
        cout << "=============================" << endl;
    }

}