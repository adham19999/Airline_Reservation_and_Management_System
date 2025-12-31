#ifndef FLIGHT_ATTENDANT_H
#define FLIGHT_ATTENDANT_H

#include "CrewMember.h"
#include <string>
using namespace std;

class FlightAttendant : public CrewMember {
private:
    string certificationLevel;

public:
    FlightAttendant(const string& crewId, const string& name,
                   const string& certificationLevel);

    string getCertificationLevel() const;
    
    string getCrewType() const override;
};

#endif