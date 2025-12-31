#include "Pilot.h"

Pilot::Pilot(const string& crewId, const string& name,
             const string& licenseNumber, double flightHours)
    : CrewMember(crewId, name),
      licenseNumber(licenseNumber),
      totalFlightHours(flightHours) {}

string Pilot::getLicenseNumber() const {
    return licenseNumber;
}

double Pilot::getTotalFlightHours() const {
    return totalFlightHours;
}

void Pilot::addFlightHours(double hours) {
    totalFlightHours += hours;
}

string Pilot::getCrewType() const {
    return "Pilot";
}