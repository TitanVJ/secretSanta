#ifndef SANTA_H
#define SANTA_H

#include <string>
#include <vector>

class Santa{
    public:
        // Constructors
        Santa();
        Santa(std::string, std::string, int, int, std::vector<std::string>&);

        //Utility
        friend std::ostream& operator<<(std::ostream& output, const Santa&); 

    private:
        std::string name;
        std::string email;
        int age; 
        int sfuId;
        int santaId; 
        int gifterId;

        // details; everything after the email will be details. program usage will also be data.txt questions.txt
        // questions.txt will be the questions asked during the 
        std::vector<std::string> details; // either this or mutiple strings with things 

};

#endif