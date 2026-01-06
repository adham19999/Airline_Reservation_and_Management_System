#ifndef FLIGHT_REPOSITORY_H
#define FLIGHT_REPOSITORY_H

#include <vector>
#include <memory>
#include <string>
#include "../domain/flight/Flight.h"
#include "AircraftRepository.h"

using namespace std;

class FlightRepository {
private:
    vector<shared_ptr<Flight>> flights;
    string dataFilePath;
    
    void loadFromJson(AircraftRepository& aircraftRepo);
    void saveToJson() const;
    void parseFlightObject(const string& flightJson, AircraftRepository& aircraftRepo);
    string flightStatusToString(FlightStatus status) const;
    FlightStatus stringToFlightStatus(const string& statusStr) const;
    
public:
    FlightRepository(const string& dataPath);
    
    void load(AircraftRepository& aircraftRepo);
    void addFlight(shared_ptr<Flight> flight);
    void updateFlight(shared_ptr<Flight> flight);
    void deleteFlight(const string& flightId);
    
    shared_ptr<Flight> findByFlightId(const string& flightId) const;
    shared_ptr<Flight> findByFlightNumber(const string& flightNumber) const;
    vector<shared_ptr<Flight>> searchFlights(const string& origin, const string& destination, const string& date) const;
    const vector<shared_ptr<Flight>>& getAllFlights() const;
};

#endif