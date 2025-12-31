#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <memory>
#include "../flight/Flight.h"
#include "../user/Passenger.h"
#include "Payment.h"
using namespace std;

enum class ReservationStatus {
    Confirmed,
    Canceled,
    CheckedIn
};

class Reservation {
private:
    string reservationId;
    shared_ptr<Flight> flight;
    shared_ptr<Passenger> passenger;
    string seatNumber;
    ReservationStatus status;
    shared_ptr<Payment> payment;

public:
    Reservation(const string& reservationId,
                shared_ptr<Flight> flight,
                shared_ptr<Passenger> passenger,
                const string& seatNumber,
                shared_ptr<Payment> payment);

    string getReservationId() const;
    shared_ptr<Flight> getFlight() const;
    shared_ptr<Passenger> getPassenger() const;
    string getSeatNumber() const;

    ReservationStatus getStatus() const;
    void setStatus(ReservationStatus status);

    shared_ptr<Payment> getPayment() const;
};

#endif
