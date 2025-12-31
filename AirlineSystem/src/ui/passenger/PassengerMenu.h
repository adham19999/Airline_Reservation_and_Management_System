#ifndef PASSENGER_MENU_H
#define PASSENGER_MENU_H

#include "../Menu.h"
#include <memory>

class FlightService;
class AircraftService;

class PassengerMenu : public Menu {
private:
    bool running = true;
    FlightService& flightService;
    AircraftService& aircraftService;

    void searchFlights();
    void viewReservations();
    void checkIn();

public:
    PassengerMenu(std::shared_ptr<User> user, FlightService& fs, AircraftService& as);

    void display() override;
    bool handleInput(int choice) override;
};

#endif