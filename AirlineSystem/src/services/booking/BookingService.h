#ifndef BOOKING_SERVICE_H
#define BOOKING_SERVICE_H

#include <memory>
#include <string>
#include <vector>
#include "SeatService.h"
#include "../../repository/FlightRepository.h"
#include "../../repository/ReservationRepository.h"
#include "../../domain/booking/Reservation.h"
#include "../../domain/booking/Payment.h"
#include "../../domain/booking/PaymentStrategy.h"

using namespace std;

class BookingService {
private:
    FlightRepository& flightRepo;
    ReservationRepository& reservationRepo;
    SeatService& seatService;
    
    string generateReservationId();
    string getCurrentDateTime();  // ✅ Added missing declaration
    double calculatePrice(SeatClass seatClass) const;

public:
    BookingService(FlightRepository& fRepo, 
                   ReservationRepository& resRepo,
                   SeatService& sService);

    shared_ptr<Reservation> bookFlight(shared_ptr<Passenger> passenger,
                                       const string& flightId,
                                       const string& seatId,
                                       shared_ptr<PaymentStrategy> paymentStrategy);

    bool cancelReservation(const string& reservationId);
    bool modifyReservation(const string& reservationId, const string& newSeatId);
    
    shared_ptr<Reservation> getReservation(const string& reservationId) const;
    vector<shared_ptr<Reservation>> getPassengerReservations(const string& passengerId) const;
};

#endif