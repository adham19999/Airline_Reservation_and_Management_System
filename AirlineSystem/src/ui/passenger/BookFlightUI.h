#ifndef BOOK_FLIGHT_UI_H
#define BOOK_FLIGHT_UI_H

#include <memory>
#include "../../services/booking/BookingService.h"
#include "../../services/flight/FlightService.h"
#include "../../services/booking/SeatService.h"
#include "../../domain/user/Passenger.h"
#include "../../domain/flight/Flight.h"

using namespace std;

class BookFlightUI {
private:
    BookingService& bookingService;
    FlightService& flightService;
    SeatService& seatService;
    shared_ptr<Passenger> currentPassenger;

    void displayAvailableSeats(shared_ptr<Flight> flight) const;
    string selectSeat(shared_ptr<Flight> flight);
    shared_ptr<PaymentStrategy> selectPaymentMethod();

public:
    BookFlightUI(BookingService& bs, FlightService& fs, SeatService& ss, shared_ptr<Passenger> passenger);
    
    void display();
};

#endif