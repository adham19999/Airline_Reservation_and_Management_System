#include "Flight.h"

Flight::Flight(const string& flightId,
               const string& flightNumber,
               const string& departureCity,
               const string& arrivalCity,
               const string& departureTime,
               const string& arrivalTime,
               const string& aircraftId,
               int totalSeats,
               FlightStatus status)
    : flightId(flightId),
      flightNumber(flightNumber),
      departureCity(departureCity),
      arrivalCity(arrivalCity),
      departureTime(departureTime),
      arrivalTime(arrivalTime),
      aircraftId(aircraftId),
      totalSeats(totalSeats),
      status(status) {}

string Flight::getFlightId() const { return flightId; }
string Flight::getFlightNumber() const { return flightNumber; }
string Flight::getDepartureCity() const { return departureCity; }
string Flight::getArrivalCity() const { return arrivalCity; }
string Flight::getDepartureTime() const { return departureTime; }
string Flight::getArrivalTime() const { return arrivalTime; }
string Flight::getAircraftId() const { return aircraftId; }
int Flight::getTotalSeats() const { return totalSeats; }
FlightStatus Flight::getStatus() const { return status; }
const vector<shared_ptr<Seat>>& Flight::getSeats() const { return seats; }

void Flight::setStatus(FlightStatus s) { status = s; }
void Flight::addSeat(shared_ptr<Seat> seat) {
    if (seat) seats.push_back(seat);
}
