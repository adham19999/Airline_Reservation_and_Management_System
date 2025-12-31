#ifndef SEAT_H
#define SEAT_H

#include <string>

using namespace std;

enum class SeatClass {
    Economy,
    Business
};

enum class SeatStatus {
    Available,
    Occupied,
    Reserved
};

class Seat {
private:
    string seatId;
    string seatNumber;  
    SeatClass seatClass;
    SeatStatus status;
    string flightId;

public:
    Seat(const string& seatId, const string& seatNumber, SeatClass seatClass, 
         SeatStatus status, const string& flightId);

    string getSeatId() const;
    string getSeatNumber() const;
    SeatClass getSeatClass() const;
    SeatStatus getStatus() const;
    string getFlightId() const;
    
    bool isAvailable() const;
    void occupy();
    void release();
    void setStatus(SeatStatus newStatus);
};

#endif
