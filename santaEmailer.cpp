#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){
    if(argc < 2){
        cout << "Usage: ./santaEmailer.o message.txt\nWhere message.txt is the file containing the message you want in the email sent." << endl;
        return;
    }

    /*
        -read in the master list
        -present menu with option: test email, final email
        -if test, saying to remove failed entries from original data then to delete all data generated from the santa parser before rerunning it with the newly ammended data
        - after that to rerun this program once the final emails are to be sent.
    */
}