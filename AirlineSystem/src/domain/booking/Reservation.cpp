#include "Reservation.h"

Reservation::Reservation(const string& reservationId,
                         shared_ptr<Flight> flight,
                         shared_ptr<Passenger> passenger,
                         const string& seatNumber,
                         shared_ptr<Payment> payment)
    : reservationId(reservationId),
      flight(flight),
      passenger(passenger),
      seatNumber(seatNumber),
      status(ReservationStatus::Confirmed),
      payment(payment) {}

string Reservation::getReservationId() const {
    return reservationId;
}

shared_ptr<Flight> Reservation::getFlight() const {
    return flight;
}

shared_ptr<Passenger> Reservation::getPassenger() const {
    return passenger;
}

string Reservation::getSeatNumber() const {
    return seatNumber;
}

ReservationStatus Reservation::getStatus() const {
    return status;
}

void Reservation::setStatus(ReservationStatus status) {
    this->status = status;
}

shared_ptr<Payment> Reservation::getPayment() const {
    return payment;
}
