#include "Reservation.h"
#include <ctime>
#include <sstream>
#include <iomanip>

Reservation::Reservation(const string& reservationId,
                         shared_ptr<Flight> flight,
                         shared_ptr<Passenger> passenger,
                         shared_ptr<Seat> seat,
                         shared_ptr<Payment> payment,
                         const string& bookingDate)
    : reservationId(reservationId),
      flight(flight),
      passenger(passenger),
      seat(seat),
      payment(payment),
      bookingDate(bookingDate),
      status(ReservationStatus::Confirmed) {}

// Getters
string Reservation::getReservationId() const { return reservationId; }
shared_ptr<Flight> Reservation::getFlight() const { return flight; }
shared_ptr<Passenger> Reservation::getPassenger() const { return passenger; }
shared_ptr<Seat> Reservation::getSeat() const { return seat; }
shared_ptr<Payment> Reservation::getPayment() const { return payment; }
string Reservation::getBookingDate() const { return bookingDate; }
ReservationStatus Reservation::getStatus() const { return status; }

// Convenience getter - extracts seat number from seat object
string Reservation::getSeatNumber() const {
    if (seat) {
        return seat->getSeatNumber();
    }
    return "N/A";
}

// Setters
void Reservation::setStatus(ReservationStatus newStatus) {
    status = newStatus;
}

// Sets the seat OBJECT (not string)
void Reservation::setSeat(shared_ptr<Seat> newSeat) {
    seat = newSeat;
}

// State management
void Reservation::cancel() {
    status = ReservationStatus::Cancelled;
    if (seat) {
        seat->release();
    }
    if (payment) {
        payment->refund();
    }
}

void Reservation::checkIn() {
    if (status == ReservationStatus::Confirmed) {
        status = ReservationStatus::CheckedIn;
        if (seat) {
            seat->occupy();
        }
    }
}
