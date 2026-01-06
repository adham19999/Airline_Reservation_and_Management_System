#ifndef SEAT_SERVICE_H
#define SEAT_SERVICE_H

#include <memory>
#include <string>
#include <vector>
#include "../../repository/FlightRepository.h"
#include "../../domain/flight/Seat.h"

using namespace std;

class SeatService {
private:
    FlightRepository& flightRepo;

public:
    SeatService(FlightRepository& fRepo);

    bool reserveSeat(const string& flightId, const string& seatId);
    bool releaseSeat(const string& flightId, const string& seatId);
    bool isSeatAvailable(const string& flightId, const string& seatId) const;
    
    shared_ptr<Seat> getSeatById(const string& flightId, const string& seatId) const;
    vector<shared_ptr<Seat>> getAvailableSeats(const string& flightId) const;
    vector<shared_ptr<Seat>> getAvailableSeatsByClass(const string& flightId, SeatClass seatClass) const;
    int getAvailableSeatCount(const string& flightId) const;
};

#endif