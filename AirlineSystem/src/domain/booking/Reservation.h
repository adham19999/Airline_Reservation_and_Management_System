#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <memory>
#include "../flight/Flight.h"
#include "../flight/Seat.h"
#include "../user/Passenger.h"
#include "Payment.h"

using namespace std;

enum class ReservationStatus {
    Confirmed,
    Cancelled,
    CheckedIn,
    Completed
};

class Reservation {
private:
    string reservationId;
    shared_ptr<Flight> flight;
    shared_ptr<Passenger> passenger;
    shared_ptr<Seat> seat;           // ✅ Seat OBJECT (not string)
    shared_ptr<Payment> payment;
    string bookingDate;
    ReservationStatus status;

public:
    Reservation(const string& reservationId,
                shared_ptr<Flight> flight,
                shared_ptr<Passenger> passenger,
                shared_ptr<Seat> seat,
                shared_ptr<Payment> payment,
                const string& bookingDate);

    // Getters
    string getReservationId() const;
    shared_ptr<Flight> getFlight() const;
    shared_ptr<Passenger> getPassenger() const;
    shared_ptr<Seat> getSeat() const;
    shared_ptr<Payment> getPayment() const;
    string getBookingDate() const;
    ReservationStatus getStatus() const;

    // ✅ Convenience method - returns seat number as string
    string getSeatNumber() const;

    // Setters
    void setStatus(ReservationStatus newStatus);
    void setSeat(shared_ptr<Seat> newSeat);  // ✅ Changed from setSeatNumber

    // State management
    void cancel();
    void checkIn();
};

#endif
