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
    Reserved,
    Occupied
};

class Seat {
private:
    string seatId;       // Primary key: "SEAT_F001_12A"
    string seatNumber;   // Display only: "12A"
    SeatClass seatClass;
    SeatStatus status;
    string flightId;     // Owning flight

public:
    Seat(const string& seatId,
         const string& seatNumber,
         SeatClass seatClass,
         SeatStatus status,
         const string& flightId);

    // Getters
    string getSeatId() const;
    string getSeatNumber() const;
    SeatClass getSeatClass() const;
    SeatStatus getStatus() const;
    string getFlightId() const;

    // State checks
    bool isAvailable() const;

    // State transitions (controlled by SeatService)
    void reserve();
    void occupy();
    void release();
};

#endif