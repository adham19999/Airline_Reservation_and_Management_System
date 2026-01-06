#include "BookingService.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>

using namespace std;

BookingService::BookingService(FlightRepository& fRepo, 
                               ReservationRepository& resRepo,
                               SeatService& sService)
    : flightRepo(fRepo), reservationRepo(resRepo), seatService(sService) {}

string BookingService::generateReservationId() {
    static int counter = 1;
    stringstream ss;
    ss << "R" << counter++;
    return ss.str();
}

string BookingService::getCurrentDateTime() {
    time_t now = time(nullptr);
    
    // ✅ FIXED: Use localtime_s (thread-safe, MSVC-compatible)
    tm ltm;
    #ifdef _WIN32
        localtime_s(&ltm, &now);
    #else
        localtime_r(&now, &ltm);
    #endif
    
    stringstream ss;
    ss << (1900 + ltm.tm_year) << "-"
       << setfill('0') << setw(2) << (1 + ltm.tm_mon) << "-"
       << setfill('0') << setw(2) << ltm.tm_mday << " "
       << setfill('0') << setw(2) << ltm.tm_hour << ":"
       << setfill('0') << setw(2) << ltm.tm_min;
    return ss.str();
}

double BookingService::calculatePrice(SeatClass seatClass) const {
    if (seatClass == SeatClass::Business) {
        return 500.00;
    }
    return 250.00;
}

shared_ptr<Reservation> BookingService::bookFlight(shared_ptr<Passenger> passenger,
                                                   const string& flightId,
                                                   const string& seatId,
                                                   shared_ptr<PaymentStrategy> paymentStrategy) {
    // Validate flight
    auto flight = flightRepo.findByFlightId(flightId);
    if (!flight) {
        cerr << "Error: Flight not found: " << flightId << "\n";
        return nullptr;
    }

    // Get and validate seat
    auto seat = seatService.getSeatById(flightId, seatId);
    if (!seat) {
        cerr << "Error: Seat " << seatId << " not found.\n";
        return nullptr;
    }

    if (!seat->isAvailable()) {
        cerr << "Error: Seat " << seat->getSeatNumber() << " is already occupied.\n";
        return nullptr;
    }

    // Calculate price and process payment
    double price = calculatePrice(seat->getSeatClass());
    
    if (!paymentStrategy->processPayment(price)) {
        cerr << "Error: Payment failed.\n";
        return nullptr;
    }

    // Reserve the seat
    if (!seatService.reserveSeat(flightId, seatId)) {
        cerr << "Error: Could not reserve seat.\n";
        return nullptr;
    }

    // Create payment record
    string paymentId = "PAY" + generateReservationId();
    auto payment = make_shared<Payment>(paymentId, price, paymentStrategy);
    payment->setStatus(PaymentStatus::Completed);

    // Create reservation - ✅ FIXED: Pass seat OBJECT, not string
    string reservationId = generateReservationId();
    string bookingDate = getCurrentDateTime();
    
    auto reservation = make_shared<Reservation>(
        reservationId, 
        flight, 
        passenger, 
        seat,           // ✅ Pass shared_ptr<Seat>, not seat->getSeatNumber()
        payment,
        bookingDate
    );

    reservationRepo.addReservation(reservation);

    cout << "\n========================================\n";
    cout << "Booking successful!\n";
    cout << "========================================\n";
    cout << "Reservation ID: " << reservationId << "\n";
    cout << "Passenger: " << passenger->getFullName() << "\n";
    cout << "Flight: " << flight->getFlightNumber() << " from " 
         << flight->getDepartureCity() << " to " << flight->getArrivalCity() << "\n";
    cout << "Seat: " << seat->getSeatNumber() << "\n";
    cout << "Total Cost: $" << price << "\n";
    cout << "Payment Method: " << paymentStrategy->getPaymentMethodName() << "\n";
    cout << "========================================\n";

    return reservation;
}

bool BookingService::cancelReservation(const string& reservationId) {
    auto reservation = reservationRepo.findByReservationId(reservationId);
    if (!reservation) {
        cerr << "Error: Reservation " << reservationId << " not found.\n";
        return false;
    }

    // Get seat from reservation and release it
    auto seat = reservation->getSeat();
    auto flight = reservation->getFlight();
    
    if (seat && flight) {
        seatService.releaseSeat(flight->getFlightId(), seat->getSeatId());
    }

    // Process refund
    auto payment = reservation->getPayment();
    if (payment && payment->refund()) {
        cout << "Refund of $" << payment->getAmount() << " processed.\n";
    }

    reservation->setStatus(ReservationStatus::Cancelled);
    reservationRepo.updateReservation(reservation);

    cout << "Reservation " << reservationId << " has been cancelled.\n";
    return true;
}

bool BookingService::modifyReservation(const string& reservationId, const string& newSeatId) {
    auto reservation = reservationRepo.findByReservationId(reservationId);
    if (!reservation) {
        cerr << "Error: Reservation not found.\n";
        return false;
    }

    auto flight = reservation->getFlight();
    
    // Check new seat availability
    if (!seatService.isSeatAvailable(flight->getFlightId(), newSeatId)) {
        cerr << "Error: New seat is not available.\n";
        return false;
    }

    // Release old seat
    auto oldSeat = reservation->getSeat();
    if (oldSeat) {
        seatService.releaseSeat(flight->getFlightId(), oldSeat->getSeatId());
    }

    // Reserve new seat
    seatService.reserveSeat(flight->getFlightId(), newSeatId);

    // Get new seat object and update reservation
    auto newSeat = seatService.getSeatById(flight->getFlightId(), newSeatId);
    reservation->setSeat(newSeat);  // ✅ FIXED: Use setSeat(), not setSeatNumber()
    reservationRepo.updateReservation(reservation);

    cout << "Reservation modified. New seat: " << newSeat->getSeatNumber() << "\n";
    return true;
}

shared_ptr<Reservation> BookingService::getReservation(const string& reservationId) const {
    return reservationRepo.findByReservationId(reservationId);
}

vector<shared_ptr<Reservation>> BookingService::getPassengerReservations(const string& passengerId) const {
    return reservationRepo.findByPassengerId(passengerId);
}