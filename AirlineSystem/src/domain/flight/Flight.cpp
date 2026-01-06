#include "Flight.h"

Flight::Flight(const string& flightId,
               const string& flightNumber,
               shared_ptr<Aircraft> aircraft,
               const string& departureCity,
               const string& arrivalCity,
               const string& departureTime,
               const string& arrivalTime,
               FlightStatus status)
    : flightId(flightId),
      flightNumber(flightNumber),
      aircraft(aircraft),
      departureCity(departureCity),
      arrivalCity(arrivalCity),
      departureTime(departureTime),
      arrivalTime(arrivalTime),
      status(status) {}

string Flight::getFlightId() const { return flightId; }
string Flight::getFlightNumber() const { return flightNumber; }
string Flight::getDepartureCity() const { return departureCity; }
string Flight::getArrivalCity() const { return arrivalCity; }
string Flight::getDepartureTime() const { return departureTime; }
string Flight::getArrivalTime() const { return arrivalTime; }
shared_ptr<Aircraft> Flight::getAircraft() const { return aircraft; }

// ✅ ADD THIS - Returns aircraftId from the Aircraft object
string Flight::getAircraftId() const {
    return aircraft ? aircraft->getAircraftId() : "";
}

FlightStatus Flight::getStatus() const { return status; }
const vector<shared_ptr<Seat>>& Flight::getSeats() const { return seats; }

void Flight::setStatus(FlightStatus s) { status = s; }

void Flight::setSeats(const vector<shared_ptr<Seat>>& newSeats) {
    seats = newSeats;
}

void Flight::addSeat(shared_ptr<Seat> seat) {
    if (seat) seats.push_back(seat);
}
