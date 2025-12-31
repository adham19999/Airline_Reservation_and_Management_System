#ifndef PILOT_H
#define PILOT_H

#include "CrewMember.h"
#include <string>
using namespace std;

class Pilot : public CrewMember {
private:
    string licenseNumber;
    double totalFlightHours;

public:
    Pilot(const string& crewId, const string& name,
          const string& licenseNumber, double flightHours);

    string getLicenseNumber() const;
    double getTotalFlightHours() const;
    void addFlightHours(double hours);

    string getCrewType() const override;
};

#endif