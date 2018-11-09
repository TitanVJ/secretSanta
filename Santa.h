#include <string>
#include <vector>

class Santa{
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

    public:
        // getters setters, others,
        // might but friend class main so wont need the setters and getters 
        
        // Constructors
        Santa();
        Santa(std::string, std::string, int, int, int, std::vector<std::string>&);

        // Utility
        ~Santa();
        // get file data? 

};