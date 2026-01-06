#ifndef FLIGHT_SERVICE_H
#define FLIGHT_SERVICE_H

#include <memory>
#include <vector>
#include <string>
#include "../../repository/FlightRepository.h"
#include "../../repository/AircraftRepository.h"
#include "../../domain/flight/Flight.h"
#include "../../domain/flight/Seat.h"

using namespace std;

class FlightService {
private:
    FlightRepository& flightRepo;
    AircraftRepository& aircraftRepo;

    // ✅ Single source of truth for seat generation
    void generateSeatsForFlight(shared_ptr<Flight> flight, shared_ptr<Aircraft> aircraft);

public:
    FlightService(FlightRepository& repo, AircraftRepository& aircraftRepo);

    // Business operations
    void addFlight(const string& flightId,
                   const string& flightNumber,
                   const string& departureCity,
                   const string& arrivalCity,
                   const string& departureTime,
                   const string& arrivalTime,
                   const string& aircraftId,
                   FlightStatus status);

    void updateFlight(const string& flightId,
                      const string& flightNumber,
                      const string& departureCity,
                      const string& arrivalCity,
                      const string& departureTime,
                      const string& arrivalTime,
                      const string& aircraftId,
                      FlightStatus status);

    void updateFlightStatus(const string& flightId, FlightStatus newStatus);
    void deleteFlight(const string& flightId);

    // ✅ NEW: Ensures business invariant "All flights have seats"
    void ensureAllFlightsHaveSeats();

    // Query operations
    shared_ptr<Flight> getFlightById(const string& flightId) const;
    shared_ptr<Flight> getFlightByNumber(const string& flightNumber) const;
    vector<shared_ptr<Flight>> searchFlights(const string& origin,
                                            const string& destination,
                                            const string& date) const;
    vector<shared_ptr<Flight>> getAllFlights() const;
};

#endif