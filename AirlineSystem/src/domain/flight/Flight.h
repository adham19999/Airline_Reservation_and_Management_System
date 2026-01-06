#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <vector>
#include <memory>
#include "Seat.h"
#include "Aircraft.h"
using namespace std;

enum class FlightStatus {
    Scheduled,
    InFlight,
    Completed,
    Cancelled,
    Delayed 
};

class Flight {
private:
    string flightId;
    string flightNumber;
    string departureCity;
    string arrivalCity;
    string departureTime;
    string arrivalTime;
    shared_ptr<Aircraft> aircraft;
    FlightStatus status;
    vector<shared_ptr<Seat>> seats;

public:
    Flight(const string& flightId,
           const string& flightNumber,
           shared_ptr<Aircraft> aircraft,
           const string& departureCity,
           const string& arrivalCity,
           const string& departureTime,
           const string& arrivalTime,
           FlightStatus status);

    // Getters
    string getFlightId() const;
    string getFlightNumber() const;
    string getDepartureCity() const;
    string getArrivalCity() const;
    string getDepartureTime() const;
    string getArrivalTime() const;
    shared_ptr<Aircraft> getAircraft() const;
    string getAircraftId() const;  // ✅ ADD THIS - Convenience method
    FlightStatus getStatus() const;
    const vector<shared_ptr<Seat>>& getSeats() const;

    // Setters
    void setStatus(FlightStatus status);
    void setSeats(const vector<shared_ptr<Seat>>& newSeats);
    void addSeat(shared_ptr<Seat> seat);
};

#endif
