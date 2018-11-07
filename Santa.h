#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

class Santa{
    private:
        std::string name;
        std::string email;
        int age; 
        int santaId; 
        int gifterId;

        // details; everything after the email will be details. program usage will also be data.txt questions.txt
        // questions.txt will be the questions asked during the 
        std::vector<std::string> details; // either this or mutiple strings with things 

    public:
        // getters setters, others,
        // might but friend class main so wont need the setters and getters 
        
        // Constructors
        Santa();
        Santa(std::string, std::string, int);

        // Getters
        std::string name();
        std::string email();
        int age();
        int santaId();
        int gifterId();
        std::string details();

        // Setters
        std::string name(std::string);
        std::string email(std::string);
        int age(int);
        int santaId(int);
        int gifterId(int);
        std::string details(std::string);

        // Utility
        ~Santa();
        // get file data? 



};