#ifndef CREW_MEMBER_H
#define CREW_MEMBER_H

#include <string>
using namespace std;

class CrewMember {
protected:
    string crewId;
    string name;

public:
    CrewMember(const string& crewId, const string& name);
    
    virtual ~CrewMember() = default;

    // Getters
    string getCrewId() const;
    string getName() const;


    // Pure virtual for polymorphism
    virtual string getCrewType() const = 0;
};

#endif