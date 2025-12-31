#ifndef FLIGHT_SERVICE_H
#define FLIGHT_SERVICE_H

#include <memory>
#include <vector>
#include <string>
#include "../../repository/FlightRepository.h"
#include "../../repository/AircraftRepository.h"
#include "../../domain/flight/Flight.h"

using namespace std;

class FlightService {
private:
    FlightRepository& flightRepo;
    AircraftRepository& aircraftRepo;

public:
    FlightService(FlightRepository& fRepo, AircraftRepository& aRepo);
    
    // Flight management
    shared_ptr<Flight> createFlight(const string& flightNumber, const string& departure,
                                   const string& arrival, const string& depTime,
                                   const string& arrTime, const string& aircraftId);
    
    shared_ptr<Flight> getFlightByNumber(const string& flightNumber) const;
    const vector<shared_ptr<Flight>>& getAllFlights() const;
    
    void updateFlightStatus(const string& flightId, FlightStatus status);
    void deleteFlight(const string& flightId);
    
    // Seat generation
    
    void generateSeatsForFlight(const string& flightId);
 // Search functionality
    vector<shared_ptr<Flight>> searchFlights(const string& origin, 
                                             const string& destination, 
                                             const string& departureDate);
};

#endif