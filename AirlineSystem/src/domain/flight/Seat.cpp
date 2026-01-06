#include "Seat.h"

Seat::Seat(const string& seatId,
           const string& seatNumber,
           SeatClass seatClass,
           SeatStatus status,
           const string& flightId)
    : seatId(seatId),
      seatNumber(seatNumber),
      seatClass(seatClass),
      status(status),
      flightId(flightId) {}

string Seat::getSeatId() const { return seatId; }
string Seat::getSeatNumber() const { return seatNumber; }
SeatClass Seat::getSeatClass() const { return seatClass; }
SeatStatus Seat::getStatus() const { return status; }
string Seat::getFlightId() const { return flightId; }

bool Seat::isAvailable() const {
    return status == SeatStatus::Available;
}

void Seat::reserve() {
    if (status == SeatStatus::Available) {
        status = SeatStatus::Reserved;
    }
}

void Seat::occupy() {
    if (status == SeatStatus::Reserved || status == SeatStatus::Available) {
        status = SeatStatus::Occupied;
    }
}

void Seat::release() {
    status = SeatStatus::Available;
}