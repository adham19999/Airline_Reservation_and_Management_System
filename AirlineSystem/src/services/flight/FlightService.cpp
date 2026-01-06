#include "FlightService.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>

using namespace std;

FlightService::FlightService(FlightRepository& repo, AircraftRepository& aircraftRepo)
    : flightRepo(repo), aircraftRepo(aircraftRepo) {}

void FlightService::addFlight(const string& flightId,
                              const string& flightNumber,
                              const string& departureCity,
                              const string& arrivalCity,
                              const string& departureTime,
                              const string& arrivalTime,
                              const string& aircraftId,
                              FlightStatus status) {
    
    auto aircraft = aircraftRepo.findByAircraftId(aircraftId);
    if (!aircraft) {
        throw runtime_error("Aircraft not found: " + aircraftId);
    }

    auto flight = make_shared<Flight>(
        flightId,
        flightNumber,
        aircraft,
        departureCity,
        arrivalCity,
        departureTime,
        arrivalTime,
        status
    );

    // ✅ Generate seats for new flight
    if (flight->getSeats().empty()) {
        generateSeatsForFlight(flight, aircraft);
    }

    flightRepo.addFlight(flight);
}

void FlightService::updateFlight(const string& flightId,
                                 const string& flightNumber,
                                 const string& departureCity,
                                 const string& arrivalCity,
                                 const string& departureTime,
                                 const string& arrivalTime,
                                 const string& aircraftId,
                                 FlightStatus status) {
    
    auto flight = flightRepo.findByFlightId(flightId);
    if (!flight) {
        throw runtime_error("Flight not found: " + flightId);
    }

    auto aircraft = aircraftRepo.findByAircraftId(aircraftId);
    if (!aircraft) {
        throw runtime_error("Aircraft not found: " + aircraftId);
    }

    auto updatedFlight = make_shared<Flight>(
        flightId,
        flightNumber,
        aircraft,
        departureCity,
        arrivalCity,
        departureTime,
        arrivalTime,
        status
    );

    // ✅ Preserve seats if same aircraft, regenerate if changed
    if (aircraft->getAircraftId() == flight->getAircraft()->getAircraftId()) {
        updatedFlight->setSeats(flight->getSeats());
    } else {
        generateSeatsForFlight(updatedFlight, aircraft);
    }

    flightRepo.updateFlight(updatedFlight);
}

void FlightService::updateFlightStatus(const string& flightId, FlightStatus newStatus) {
    auto flight = flightRepo.findByFlightId(flightId);
    if (!flight) {
        throw runtime_error("Flight not found: " + flightId);
    }

    flight->setStatus(newStatus);
    flightRepo.updateFlight(flight);
}

void FlightService::deleteFlight(const string& flightId) {
    flightRepo.deleteFlight(flightId);
}

shared_ptr<Flight> FlightService::getFlightById(const string& flightId) const {
    return flightRepo.findByFlightId(flightId);
}

shared_ptr<Flight> FlightService::getFlightByNumber(const string& flightNumber) const {
    return flightRepo.findByFlightNumber(flightNumber);
}

vector<shared_ptr<Flight>> FlightService::searchFlights(const string& origin,
                                                        const string& destination,
                                                        const string& date) const {
    return flightRepo.searchFlights(origin, destination, date);
}

vector<shared_ptr<Flight>> FlightService::getAllFlights() const {
    return flightRepo.getAllFlights();
}

void FlightService::ensureAllFlightsHaveSeats() {
    auto allFlights = flightRepo.getAllFlights();
    
    for (auto& flight : allFlights) {
        if (flight->getSeats().empty()) {
            auto aircraft = flight->getAircraft();
            if (aircraft) {
                cout << "  → Generating seats for flight " << flight->getFlightNumber() << "...\n";
                generateSeatsForFlight(flight, aircraft);
                flightRepo.updateFlight(flight);
            }
        }
    }
}

// ✅ ADD THIS MISSING IMPLEMENTATION
void FlightService::generateSeatsForFlight(shared_ptr<Flight> flight, shared_ptr<Aircraft> aircraft) {
    vector<shared_ptr<Seat>> seats;
    
    int businessSeats = aircraft->getBusinessSeats();
    int economySeats = aircraft->getEconomySeats();
    
    // Generate Business Class seats (rows 1-5, columns A-F)
    int seatCounter = 1;
    for (int row = 1; row <= (businessSeats / 6) && seatCounter <= businessSeats; ++row) {
        for (char col = 'A'; col <= 'F' && seatCounter <= businessSeats; ++col) {
            string seatNumber = to_string(row) + col;
            string seatId = "SEAT_" + flight->getFlightId() + "_" + seatNumber;
            
            auto seat = make_shared<Seat>(
                seatId, 
                seatNumber, 
                SeatClass::Business, 
                SeatStatus::Available,
                flight->getFlightId()
            );
            seats.push_back(seat);
            seatCounter++;
        }
    }
    
    // Generate Economy Class seats (rows 10+, columns A-F)
    seatCounter = 1;
    int startRow = 10;
    for (int row = startRow; seatCounter <= economySeats; ++row) {
        for (char col = 'A'; col <= 'F' && seatCounter <= economySeats; ++col) {
            string seatNumber = to_string(row) + col;
            string seatId = "SEAT_" + flight->getFlightId() + "_" + seatNumber;
            
            auto seat = make_shared<Seat>(
                seatId, 
                seatNumber, 
                SeatClass::Economy, 
                SeatStatus::Available,
                flight->getFlightId()
            );
            seats.push_back(seat);
            seatCounter++;
        }
    }
    
    flight->setSeats(seats);
    
    cout << "✓ Generated " << seats.size() << " seats (" 
         << businessSeats << " Business, " 
         << economySeats << " Economy)\n";
}
