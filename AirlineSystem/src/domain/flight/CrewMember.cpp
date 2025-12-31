#include "CrewMember.h"

CrewMember::CrewMember(const string& crewId, const string& name)
    : crewId(crewId), name(name) {}

string CrewMember::getCrewId() const { return crewId; }
string CrewMember::getName() const { return name; }
