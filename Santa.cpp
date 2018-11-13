#include "Santa.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Santa::Santa() : name(NULL), email(NULL), age(NULL), sfuId(NULL), details(NULL){}

Santa::Santa(string name, string email, int age, int sfuId, vector<string>& details){
    name = name;
    email = email; 
    age = age; 
    sfuId = sfuId;
    details = details;

    santaId = NULL;
    gifterId = NULL;
}

ostream& operator<<(ostream& out, const Santa& sObj){
    // Go through all the data and concatenate into a
    // nice looking string and output
    // include sfu id since the program will only be run by execs

    out << "Name:\t" << sObj.name << endl;
    out << "Email\t" << sObj.email << endl;
    out << "Age:\t" << sObj.age << endl;
    out << "SFU Id:\t" << sObj.sfuId << endl;
    out << "Santa Id:\t" << sObj.santaId << endl;
    out << "Gifter Id:\t" << sObj.gifterId << endl;
    out << "Details:\tLook at file[" << sObj.santaId << "]. in admin/" << endl;

    return out;
}