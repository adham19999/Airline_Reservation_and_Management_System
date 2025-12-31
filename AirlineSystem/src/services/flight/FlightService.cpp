#include "FlightService.h"
#include "../../domain/flight/Seat.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

FlightService::FlightService(FlightRepository& fRepo, AircraftRepository& aRepo)
    : flightRepo(fRepo), aircraftRepo(aRepo) {}

shared_ptr<Flight> FlightService::createFlight(const string& flightNumber, const string& departure,
                                               const string& arrival, const string& depTime,
                                               const string& arrTime, const string& aircraftId) {
    // Validate aircraft exists
    auto aircraft = aircraftRepo.findByAircraftId(aircraftId);
    if (!aircraft) {
        throw runtime_error("Aircraft not found: " + aircraftId);
    }

    // Generate new flight ID
    const auto& allFlights = flightRepo.getAllFlights();
    string flightId = "F" + to_string(allFlights.size() + 1);

    // Create new flight with 9 parameters (including totalSeats from aircraft)
    auto flight = make_shared<Flight>(
        flightId,                          // param 1
        flightNumber,                      // param 2
        departure,                         // param 3
        arrival,                           // param 4
        depTime,                           // param 5
        arrTime,                           // param 6
        aircraftId,                        // param 7
        aircraft->getTotalSeats(),         // param 8 - NEW: totalSeats
        FlightStatus::Scheduled            // param 9
    );

    flightRepo.addFlight(flight);
    
    // Generate seats for the flight
    generateSeatsForFlight(flight->getFlightId());

    return flight;
}

shared_ptr<Flight> FlightService::getFlightByNumber(const string& flightNumber) const {
    return flightRepo.findByFlightNumber(flightNumber);
}

const vector<shared_ptr<Flight>>& FlightService::getAllFlights() const {
    return flightRepo.getAllFlights();
}

void FlightService::updateFlightStatus(const string& flightId, FlightStatus status) {
    auto flight = flightRepo.findByFlightId(flightId);
    if (!flight) {
        throw runtime_error("Flight not found: " + flightId);
    }

    flight->setStatus(status);
    flightRepo.updateFlight(flight);
}

void FlightService::deleteFlight(const string& flightId) {
    flightRepo.deleteFlight(flightId);
}

void FlightService::generateSeatsForFlight(const string& flightId) {
    auto flight = flightRepo.findByFlightId(flightId);
    if (!flight) {
        throw runtime_error("Flight not found: " + flightId);
    }

    auto aircraft = aircraftRepo.findByAircraftId(flight->getAircraftId());
    if (!aircraft) {
        throw runtime_error("Aircraft not found for flight: " + flightId);
    }

    // Generate business class seats
    int seatNumber = 1;
    for (int i = 0; i < aircraft->getBusinessSeats(); i++) {
        string seatId = "SEAT" + flightId + "_" + to_string(seatNumber);
        auto seat = make_shared<Seat>(
            seatId,
            to_string(seatNumber),
            SeatClass::Business,
            SeatStatus::Available,
            flightId
        );
        flight->addSeat(seat);
        seatNumber++;
    }

    // Generate economy class seats
    for (int i = 0; i < aircraft->getEconomySeats(); i++) {
        string seatId = "SEAT" + flightId + "_" + to_string(seatNumber);
        auto seat = make_shared<Seat>(
            seatId,
            to_string(seatNumber),
            SeatClass::Economy,
            SeatStatus::Available,
            flightId
        );
        flight->addSeat(seat);
        seatNumber++;
    }

    flightRepo.updateFlight(flight);
}

vector<shared_ptr<Flight>> FlightService::searchFlights(const string& origin, 
                                                        const string& destination, 
                                                        const string& departureDate) {
    vector<shared_ptr<Flight>> results;
    const auto& allFlights = flightRepo.getAllFlights();

    for (const auto& flight : allFlights) {
        // Partial matching for origin and destination
        bool matchesOrigin = flight->getDepartureCity().find(origin) != string::npos;
        bool matchesDestination = flight->getArrivalCity().find(destination) != string::npos;
        bool matchesDate = flight->getDepartureTime().find(departureDate) != string::npos;

        // Only return scheduled flights that match all criteria
        if (matchesOrigin && matchesDestination && matchesDate && 
            flight->getStatus() == FlightStatus::Scheduled) {
            results.push_back(flight);
        }
    }

    return results;
}