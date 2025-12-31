#include "FlightAttendant.h"

FlightAttendant::FlightAttendant(const string& crewId, const string& name,
                                 const string& certificationLevel)
    : CrewMember(crewId, name),
      certificationLevel(certificationLevel) {}

string FlightAttendant::getCertificationLevel() const {
    return certificationLevel;
}

string FlightAttendant::getCrewType() const {
    return "FlightAttendant";
}