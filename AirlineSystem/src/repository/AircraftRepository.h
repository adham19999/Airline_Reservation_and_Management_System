#ifndef AIRCRAFT_REPOSITORY_H
#define AIRCRAFT_REPOSITORY_H

#include <vector>
#include <memory>
#include <string>
#include "../domain/flight/Aircraft.h"

using namespace std;

class AircraftRepository {
private:
    vector<shared_ptr<Aircraft>> aircraft;
    string dataFilePath;
    
    void loadFromJson();
    void saveToJson() const;
    void parseAircraftObject(const string& aircraftJson);

public:
    AircraftRepository(const string& dataPath);

    void load();
    
    void addAircraft(shared_ptr<Aircraft> aircraft);
    void updateAircraft(shared_ptr<Aircraft> aircraft);
    void deleteAircraft(const string& aircraftId);
    
    shared_ptr<Aircraft> findByAircraftId(const string& aircraftId) const;
    shared_ptr<Aircraft> findByRegistration(const string& registration) const;
    const vector<shared_ptr<Aircraft>>& getAllAircraft() const;
};


#endif