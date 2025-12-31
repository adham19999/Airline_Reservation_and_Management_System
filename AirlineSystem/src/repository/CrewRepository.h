#ifndef CREW_REPOSITORY_H
#define CREW_REPOSITORY_H

#include <vector>
#include <memory>
#include <string>
#include "../domain/flight/CrewMember.h"
#include "../domain/flight/Pilot.h"
#include "../domain/flight/FlightAttendant.h"

using namespace std;

class CrewRepository {
private:
    vector<shared_ptr<CrewMember>> crewMembers;
    string dataFilePath;

    void loadFromJson();
    void saveToJson() const;
    void parseCrewObject(const string& crewJson);

public:
    CrewRepository(const string& dataPath);

    void load();
    
    void addCrewMember(shared_ptr<CrewMember> crew);
    void updateCrewMember(shared_ptr<CrewMember> crew);
    void deleteCrewMember(const string& crewId);
    
    shared_ptr<CrewMember> findByCrewId(const string& crewId) const;
    vector<shared_ptr<CrewMember>> getAllCrew() const;
    vector<shared_ptr<Pilot>> getAllPilots() const;
    vector<shared_ptr<FlightAttendant>> getAllAttendants() const;
};

#endif