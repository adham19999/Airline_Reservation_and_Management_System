#ifndef PASSENGER_MENU_H
#define PASSENGER_MENU_H

#include "../Menu.h"
#include <memory>

// Forward declarations
class FlightService;
class AircraftService;
class BookingService;
class SeatService;

class PassengerMenu : public Menu {
private:
    bool running = true;
    FlightService& flightService;
    AircraftService& aircraftService;
    BookingService& bookingService;
    SeatService& seatService;

    void searchFlights();
    void bookFlight();
    void viewReservations();
    void checkIn();

public:
    PassengerMenu(std::shared_ptr<User> user, 
                  FlightService& fs, 
                  AircraftService& as,
                  BookingService& bs,
                  SeatService& ss);

    void display() override;
    bool handleInput(int choice) override;
};

#endif