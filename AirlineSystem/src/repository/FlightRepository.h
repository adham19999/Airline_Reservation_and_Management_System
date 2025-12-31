#ifndef FLIGHT_REPOSITORY_H
#define FLIGHT_REPOSITORY_H

#include <vector>
#include <memory>
#include <string>
#include "../domain/flight/Flight.h"

using namespace std;

class FlightRepository {
private:
    vector<shared_ptr<Flight>> flights;
    string dataFilePath;
    
    void loadFromJson();
    void saveToJson() const;
    void parseFlightObject(const string& flightJson);
    string flightStatusToString(FlightStatus status) const;
    FlightStatus stringToFlightStatus(const string& statusStr) const;

public:
    FlightRepository(const string& dataPath);
    
    void load();
    shared_ptr<Flight> findByFlightNumber(const string& flightNumber) const;
    shared_ptr<Flight> findByFlightId(const string& flightId) const;
    const vector<shared_ptr<Flight>>& getAllFlights() const;
    void addFlight(shared_ptr<Flight> flight);
    void updateFlight(shared_ptr<Flight> flight);
    void deleteFlight(const string& flightId);
};

#endif