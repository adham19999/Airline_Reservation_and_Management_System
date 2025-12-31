#ifndef ADMIN_MENU_H
#define ADMIN_MENU_H

#include "../Menu.h"
#include <memory>

class FlightService;
class AircraftService;
class CrewService;

class AdminMenu : public Menu {
private:
    bool running = true;
    FlightService& flightService;
    AircraftService& aircraftService;
    CrewService& crewService;

    void manageFlights();
    void manageAircraft();
    void manageCrew();
    void manageUsers();
    void generateReports();

public:
    AdminMenu(std::shared_ptr<User> user,
              FlightService& fs,
              AircraftService& as,
              CrewService& cs);

    void display() override;
    bool handleInput(int choice) override;
};

#endif