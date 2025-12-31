#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <vector>
#include <memory>
#include "Seat.h"
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
    string aircraftId;
    int totalSeats;
    FlightStatus status;
    vector<shared_ptr<Seat>> seats;

public:
    Flight(const string& flightId,
           const string& flightNumber,
           const string& departureCity,
           const string& arrivalCity,
           const string& departureTime,
           const string& arrivalTime,
           const string& aircraftId,
           int totalSeats,
           FlightStatus status);

    // Getters
    string getFlightId() const;
    string getFlightNumber() const;
    string getDepartureCity() const;
    string getArrivalCity() const;
    string getDepartureTime() const;
    string getArrivalTime() const;
    string getAircraftId() const;
    int getTotalSeats() const;
    FlightStatus getStatus() const;
    const vector<shared_ptr<Seat>>& getSeats() const;

    // Mutators
    void setStatus(FlightStatus s);
    void addSeat(shared_ptr<Seat> seat);
};

#endif
